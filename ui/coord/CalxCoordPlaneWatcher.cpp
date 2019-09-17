/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ui/coord/CalxCoordPlaneWatcher.h"
#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>
#include <wx/sizer.h>

namespace CalX::UI {

	wxDEFINE_EVENT(wxEVT_WATCHER_UPDATE, wxThreadEvent);
	wxDEFINE_EVENT(wxEVT_WATCHER_APPEND_POINT, wxThreadEvent);

	CalxCoordPlaneWatcherTimer::CalxCoordPlaneWatcherTimer(
	    CalxCoordPlaneWatcher *watcher, std::shared_ptr<CoordHandle> handle)
	    : wxTimer::wxTimer() {
		this->watcher = watcher;
		this->handle = handle;
	}

	void CalxCoordPlaneWatcherTimer::Notify() {
		std::pair<motor_point_t, bool> pair =
		    std::make_pair(handle->getController()->getPosition(),
		                   handle->getStatus() == CoordPlaneStatus::Move);
		wxThreadEvent evt(wxEVT_WATCHER_APPEND_POINT);
		evt.SetPayload(pair);
		wxPostEvent(watcher, evt);
		watcher->Refresh();
	}

	CalxCoordPlaneWatcherRepaintTimer::CalxCoordPlaneWatcherRepaintTimer(
	    CalxCoordPlaneWatcher *watcher)
	    : wxTimer::wxTimer() {
		this->watcher = watcher;
	}

	void CalxCoordPlaneWatcherRepaintTimer::Notify() {
		if (!this->watcher->isRendering() && this->watcher->hasUpdates()) {
			this->watcher->Refresh();
		}
	}

	CalxCoordPlaneWatcherEvent::CalxCoordPlaneWatcherEvent(
	    CalxCoordPlaneWatcher *w, std::shared_ptr<CoordHandle> handle) {
		this->watcher = w;
		this->handle = handle;
	}

	CalxCoordPlaneWatcherEvent::~CalxCoordPlaneWatcherEvent() {}

	void CalxCoordPlaneWatcherEvent::moved(CoordMoveEvent &evnt) {
		std::pair<motor_point_t, bool> pair =
		    std::make_pair(handle->getController()->getPosition(), evnt.synchrone);
		wxThreadEvent evt(wxEVT_WATCHER_APPEND_POINT);
		evt.SetPayload(pair);
		wxPostEvent(watcher, evt);
	}

	CalxCoordPlaneWatcher::CalxCoordPlaneWatcher(
	    wxWindow *win, wxWindowID id, wxSize min,
	    std::shared_ptr<CoordHandle> handle, CalxWatcherPool *pool)
	    : wxWindow::wxWindow(win, id),
	      pointer_colour(255, 0, 0),
	      jump_colour(128, 128, 128),
	      move_colour(0, 0, 0) {
		this->handle = handle;
		this->pool = pool;
		pool->bindWatcher(this);

		ConfiguationFlatDictionary *colourEntry =
		    wxGetApp().getSystemManager().getConfiguration().getEntry(
		        CalxConfiguration::Watcher);
		this->pointer_colour =
		    wxColour(colourEntry->getInt(CalxWatcherConfiguration::PointerR, 255),
		             colourEntry->getInt(CalxWatcherConfiguration::PointerG, 0),
		             colourEntry->getInt(CalxWatcherConfiguration::PointerB, 0));
		this->jump_colour =
		    wxColour(colourEntry->getInt(CalxWatcherConfiguration::JumpR, 128),
		             colourEntry->getInt(CalxWatcherConfiguration::JumpG, 128),
		             colourEntry->getInt(CalxWatcherConfiguration::JumpB, 128));
		this->move_colour =
		    wxColour(colourEntry->getInt(CalxWatcherConfiguration::MoveR, 0),
		             colourEntry->getInt(CalxWatcherConfiguration::MoveG, 0),
		             colourEntry->getInt(CalxWatcherConfiguration::MoveB, 0));

		SetMinSize(min);

		this->Bind(wxEVT_CLOSE_WINDOW, &CalxCoordPlaneWatcher::OnExit, this);
		this->Bind(wxEVT_PAINT, &CalxCoordPlaneWatcher::OnPaintEvent, this);
		this->Bind(wxEVT_SIZE, &CalxCoordPlaneWatcher::OnResizeEvent, this);
		this->Bind(wxEVT_WATCHER_APPEND_POINT,
		           &CalxCoordPlaneWatcher::OnAppendEvent, this);
		this->Bind(wxEVT_WATCHER_UPDATE, &CalxCoordPlaneWatcher::OnUpdateEvent,
		           this);

		this->history.push_back(
		    std::make_pair(this->handle->getController()->getPosition(), false));

		int_conf_t interval =
		    wxGetApp()
		        .getSystemManager()
		        .getConfiguration()
		        .getEntry(CalxConfiguration::UserInterface)
		        ->getInt(CalxWatcherConfiguration::AppendInterval, 500);
		if (interval != -1) {
			this->timer = new CalxCoordPlaneWatcherTimer(this, this->handle);
			this->timer->Start((int) interval);
		} else {
			this->timer = nullptr;
		}

		interval = wxGetApp()
		               .getSystemManager()
		               .getConfiguration()
		               .getEntry(CalxConfiguration::UserInterface)
		               ->getInt(CalxWatcherConfiguration::RenderInterval, 50);
		if (interval != -1) {
			this->repaint_timer = new CalxCoordPlaneWatcherRepaintTimer(this);
			this->repaint_timer->Start((int) interval);
		} else {
			this->repaint_timer = nullptr;
		}

		this->bitmap.Create(GetSize().x, GetSize().y);
		this->rendering = false;
		this->has_updates = true;
		this->listener =
		    std::make_shared<CalxCoordPlaneWatcherEvent>(this, this->handle);
		this->handle->addEventListener(this->listener);
	}

	std::shared_ptr<CoordHandle> CalxCoordPlaneWatcher::getHandle() {
		return this->handle;
	}

	void CalxCoordPlaneWatcher::clear() {
		this->history.clear();
		this->history.push_back(
		    std::make_pair(this->handle->getController()->getPosition(), false));
		this->has_updates = true;
		renderBitmap();
		Refresh();
	}

	bool CalxCoordPlaneWatcher::isRendering() {
		return this->rendering;
	}

	bool CalxCoordPlaneWatcher::hasUpdates() {
		return this->has_updates;
	}

	void CalxCoordPlaneWatcher::add(motor_point_t point, bool sync) {
		// Append point to path
		if (!this->history.empty()) {
			motor_point_t last = this->history.at(this->history.size() - 1).first;
			if (last.x == point.x && last.y == point.y) {
				return;
			}
		}
		this->has_updates = true;
		this->history.push_back(std::make_pair(point, sync));
		// Draw it on bitmap
		motor_point_t last_point = this->history.at(this->history.size() - 2).first;
		wxMemoryDC dc;
		dc.SelectObject(this->bitmap);
		if (sync) {
			wxPen pen(this->move_colour);
			wxBrush brush(this->move_colour);
			dc.SetPen(pen);
			dc.SetBrush(brush);
		} else {
			wxPen pen(this->jump_colour);
			wxBrush brush(this->jump_colour);
			dc.SetPen(pen);
			dc.SetBrush(brush);
		}
		wxSize real_size = GetSize();
		motor_rect_t plane_size = this->handle->getController()->getSize();
		double scaleX, scaleY;
		scaleX = ((double) real_size.x) / plane_size.w;
		scaleY = ((double) real_size.y) / plane_size.h;
		double x = ((double) (point.x - plane_size.x) * scaleX);
		double y = ((double) (plane_size.h + plane_size.y - point.y) * scaleY);
		double lastX = ((double) (last_point.x - plane_size.x) * scaleX);
		double lastY =
		    ((double) (plane_size.h + plane_size.y - last_point.y) * scaleY);
		dc.DrawRectangle((wxCoord) x - 1, (wxCoord) y - 1, 2, 2);
		if (sync) {
			dc.DrawLine(static_cast<wxCoord>(lastX), static_cast<wxCoord>(lastY),
			            static_cast<wxCoord>(x), static_cast<wxCoord>(y));
		}
	}

	void CalxCoordPlaneWatcher::OnPaintEvent(wxPaintEvent &evt) {
		this->has_updates = false;
		// Render plane
		wxPaintDC dc(this);
		dc.DrawBitmap(this->bitmap, 0, 0);

		// Draw current position
		wxPen pen(this->pointer_colour);
		wxBrush brush(this->pointer_colour);
		dc.SetPen(pen);
		dc.SetBrush(brush);
		wxSize real_size = GetSize();
		motor_rect_t plane_size = this->handle->getController()->getSize();
		double scaleX, scaleY;
		scaleX = ((double) real_size.x) / plane_size.w;
		scaleY = ((double) real_size.y) / plane_size.h;
		motor_point_t point = this->handle->getController()->getPosition();
		double _x = ((double) (point.x - plane_size.x) * scaleX);
		double _y = ((double) (plane_size.h + plane_size.y - point.y) * scaleY);
		dc.DrawRectangle((int) _x - 2, (int) _y - 2, 4, 4);
	}

	void CalxCoordPlaneWatcher::OnResizeEvent(wxSizeEvent &evt) {
		if (GetSize().x > 0 && GetSize().y > 0) {
			this->bitmap.Create(GetSize().x, GetSize().y);
			renderBitmap();
			Refresh();
		}
	}

	void CalxCoordPlaneWatcher::update() {
		wxThreadEvent evt(wxEVT_WATCHER_UPDATE);
		wxPostEvent(this, evt);
	}

	void CalxCoordPlaneWatcher::OnExit(wxCloseEvent &evt) {
		this->pool->unbindWatcher(this);
		if (this->timer != nullptr) {
			this->timer->Stop();
		}
		if (this->repaint_timer) {
			this->repaint_timer->Stop();
		}
		this->handle->removeEventListener(this->listener);
		Destroy();
	}

	void CalxCoordPlaneWatcher::OnAppendEvent(wxThreadEvent &evt) {
		std::pair<motor_point_t, bool> pair =
		    evt.GetPayload<std::pair<motor_point_t, bool>>();
		this->add(pair.first, pair.second);
	}

	void CalxCoordPlaneWatcher::OnUpdateEvent(wxThreadEvent &evt) {
		this->bitmap.Create(GetSize().x, GetSize().y);
		renderBitmap();
		Refresh();
	}

	void CalxCoordPlaneWatcher::renderBitmap() {
		wxMemoryDC dc;
		dc.SelectObject(this->bitmap);
		render(dc);
	}

	void CalxCoordPlaneWatcher::render(wxDC &dc) {
		this->rendering = true;

		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();

		wxSize real_size = GetSize();
		motor_rect_t plane_size = this->handle->getController()->getSize();
		motor_rect_t top_plane_size = this->handle->getSize();
		coord_rect_t float_plane_size =
		    this->handle->getFloatPlane()->getFloatSize();
		double scaleX, scaleY, top_scaleX, top_scaleY;
		scaleX = ((double) real_size.x) / plane_size.w;
		scaleY = ((double) real_size.y) / plane_size.h;
		top_scaleX = ((double) real_size.x) / top_plane_size.w;
		top_scaleY = ((double) real_size.y) / top_plane_size.h;
		dc.SetPen(*wxBLUE_PEN);
		dc.DrawLine(0,
		            (wxCoord)((top_plane_size.h + top_plane_size.y) * top_scaleY),
		            real_size.x,
		            (wxCoord)((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.DrawLine((wxCoord)((-top_plane_size.x) * top_scaleX), 0,
		            (wxCoord)((-top_plane_size.x) * top_scaleX), real_size.y);

		wxPen move_pen(this->move_colour);
		wxBrush move_brush(this->move_colour);
		wxPen jump_pen(this->jump_colour);
		wxBrush jump_brush(this->jump_colour);
		double lastX = 0, lastY = 0;
		std::vector<std::pair<motor_point_t, bool>> *path = &this->history;
		for (std::size_t i = 0; i < path->size(); i++) {
			motor_point_t point = path->at(i).first;
			bool move = path->at(i).second;
			if (move) {
				dc.SetPen(move_pen);
				dc.SetBrush(move_brush);
			} else {
				dc.SetPen(jump_pen);
				dc.SetBrush(jump_brush);
			}
			double x = ((double) (point.x - plane_size.x) * scaleX);
			double y = ((double) (plane_size.h + plane_size.y - point.y) * scaleY);
			dc.DrawRectangle((wxCoord) x - 1, (wxCoord) y - 1, 2, 2);
			if (move) {
				dc.DrawLine(static_cast<wxCoord>(lastX), static_cast<wxCoord>(lastY),
				            static_cast<wxCoord>(x), static_cast<wxCoord>(y));
			}
			lastX = x;
			lastY = y;
		}

		dc.SetPen(*wxBLACK_PEN);
		std::string units = wxGetApp().getUnitProcessor().getUnits();
		if (!units.empty()) {
			units = " " + units;
		}
		wxCoord x, y;
		dc.DrawText(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.x),
		           units),
		    0, (wxCoord)((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.GetMultiLineTextExtent(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.x +
		                                                      float_plane_size.w),
		           units),
		    &x, &y);
		dc.DrawText(FORMAT("%s%s",
		                   wxGetApp().getUnitProcessor().formatDouble(
		                       float_plane_size.x + float_plane_size.w),
		                   units),
		            real_size.x - x,
		            (wxCoord)((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.GetMultiLineTextExtent(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.y),
		           units),
		    &x, &y);
		dc.DrawText(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.y),
		           units),
		    (wxCoord)((-top_plane_size.x) * top_scaleX) - x / 2, real_size.y - y);
		dc.GetMultiLineTextExtent(
		    FORMAT("%s%s",
		           wxGetApp().getUnitProcessor().formatDouble(float_plane_size.y +
		                                                      float_plane_size.h),
		           units),
		    &x, &y);
		dc.DrawText(FORMAT("%s%s",
		                   wxGetApp().getUnitProcessor().formatDouble(
		                       float_plane_size.y + float_plane_size.h),
		                   units),
		            (wxCoord)((-top_plane_size.x) * top_scaleX) - x / 2, 0);

		this->rendering = false;
	}

	CalxCoordPlaneWatcherDialog::CalxCoordPlaneWatcherDialog(
	    wxWindow *win, wxWindowID id, std::shared_ptr<CoordHandle> handle,
	    CalxWatcherPool *pool)
	    : wxDialog::wxDialog(win, id,
	                         FORMAT(__("Coordinate plane #%s Watcher"),
	                                std::to_string(handle->getID())),
	                         wxDefaultPosition, wxDefaultSize,
	                         wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
		this->handle = handle;
		this->pool = pool;

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		this->mouseCoords = new wxStaticText(this, wxID_ANY, "");
		sizer->Add(this->mouseCoords);
		this->watcher = new CalxCoordPlaneWatcher(this, wxID_ANY, wxSize(500, 500),
		                                          handle, pool);
		sizer->Add(this->watcher, 1, wxALL | wxEXPAND, 5);

		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALL | wxALIGN_CENTER, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_OK, __("OK"));
		buttonSizer->Add(okButton);
		wxButton *clearButton = new wxButton(buttonPanel, wxID_ANY, __("Clear"));
		buttonSizer->Add(clearButton);
		okButton->Bind(wxEVT_BUTTON, &CalxCoordPlaneWatcherDialog::OnOkClick, this);
		clearButton->Bind(wxEVT_BUTTON, &CalxCoordPlaneWatcherDialog::OnClearClick,
		                  this);
		this->watcher->Bind(wxEVT_MOTION, &CalxCoordPlaneWatcherDialog::OnMouseMove,
		                    this);

		Layout();
		Fit();
	}

	CalxCoordPlaneWatcher *CalxCoordPlaneWatcherDialog::getWatcher() {
		return this->watcher;
	}

	void CalxCoordPlaneWatcherDialog::OnOkClick(wxCommandEvent &evt) {
		this->watcher->Close(true);
		Destroy();
	}

	void CalxCoordPlaneWatcherDialog::OnClearClick(wxCommandEvent &evt) {
		this->watcher->clear();
	}

	void CalxCoordPlaneWatcherDialog::OnMouseMove(wxMouseEvent &evt) {
		if (evt.Leaving()) {
			this->mouseCoords->SetLabel("");
		}
		wxSize real_size = this->watcher->GetSize();
		coord_rect_t plane_size = this->handle->getFloatPlane()->getFloatSize();
		wxPoint mouse = this->watcher->ScreenToClient(wxGetMousePosition());
		if (mouse.x >= 0 && mouse.y >= 0 && mouse.x < real_size.x &&
		    mouse.y < real_size.y) {
			std::string units = wxGetApp().getUnitProcessor().getUnits();
			if (!units.empty()) {
				units = " " + units;
			}
			double rx = mouse.x * plane_size.w / real_size.x + plane_size.x;
			double ry =
			    plane_size.h - mouse.y * plane_size.h / real_size.y + plane_size.y;
			std::string res =
			    FORMAT(__("x: %s%s; y: %s%s"),
			           wxGetApp().getUnitProcessor().formatDouble(rx), units,
			           wxGetApp().getUnitProcessor().formatDouble(ry), units);
			this->mouseCoords->SetLabel(res);
		}
	}
}  // namespace CalX::UI
