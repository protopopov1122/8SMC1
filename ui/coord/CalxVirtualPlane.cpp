/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ui/coord/CalxVirtualPlane.h"
#include <wx/dcbuffer.h>
#include <wx/sizer.h>
#include <wx/utils.h>

namespace CalX::UI {

	CalxPlaneTracker::CalxPlaneTracker(motor_point_t pos, motor_rect_t sz)
	    : VirtualCoordPlane::VirtualCoordPlane(pos, sz) {}

	CalxPlaneTracker::~CalxPlaneTracker() {}

	bool CalxPlaneTracker::jump(motor_point_t point, bool move) {
		this->path.push_back(std::pair<motor_point_t, bool>(point, move));
		return true;
	}

	std::vector<std::pair<motor_point_t, bool>> *CalxPlaneTracker::getPath() {
		return &this->path;
	}

	void CalxPlaneTracker::reset() {
		this->path.clear();
	}

	std::unique_ptr<CoordPlane> CalxPlaneTracker::clone(
	    std::shared_ptr<CoordPlane> plane) {
		return std::make_unique<CalxPlaneTracker>(this->getPosition(),
		                                          this->getSize());
	}

	CalxVirtualPlane::CalxVirtualPlane(wxWindow *win, wxWindowID id,
	                                   std::shared_ptr<CoordHandle> base,
	                                   wxSize min)
	    : wxWindow::wxWindow(win, id),
	      pointer_colour(255, 0, 0),
	      jump_colour(128, 128, 128),
	      move_colour(0, 0, 0) {
		this->tracker = std::make_shared<CalxPlaneTracker>(
		    base->getBase()->getPosition(), base->getBase()->getSize());
		this->stack = std::make_shared<CoordPlaneStack>(base->clone(this->tracker));
		std::shared_ptr<CoordPlane> shflt(base->getFloatPlane()->clone(stack));
		this->float_plane = std::dynamic_pointer_cast<FloatCoordPlane>(shflt);
		this->base = base;
		SetMinSize(min);
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
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxVirtualPlane::OnExit, this);
		this->Bind(wxEVT_PAINT, &CalxVirtualPlane::OnPaintEvent, this);
		this->Bind(wxEVT_SIZE, &CalxVirtualPlane::OnResizeEvent, this);
	}

	std::shared_ptr<CoordPlaneStack> CalxVirtualPlane::getPlane() {
		return this->stack;
	}

	std::shared_ptr<CalxPlaneTracker> CalxVirtualPlane::getTracker() {
		return this->tracker;
	}

	std::shared_ptr<FloatCoordPlane> CalxVirtualPlane::getFloatPlane() {
		return this->float_plane;
	}

	void CalxVirtualPlane::OnExit(wxCloseEvent &evt) {}

	void CalxVirtualPlane::OnPaintEvent(wxPaintEvent &evt) {
		wxPaintDC dc(this);
		wxBufferedDC buf(&dc, this->GetSize());
		render(buf);
	}

	void CalxVirtualPlane::OnResizeEvent(wxSizeEvent &evt) {
		Refresh();
	}

	void CalxVirtualPlane::repaint() {
		wxClientDC dc(this);
		wxBufferedDC buf(&dc, this->GetSize());
		render(buf);
	}

	void CalxVirtualPlane::render(wxDC &dc) {
		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();

		wxSize real_size = GetSize();
		motor_rect_t plane_size = this->tracker->getSize();
		motor_rect_t top_plane_size = this->stack->getSize();
		coord_rect_t float_plane_size = getFloatPlane()->getFloatSize();
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
		std::vector<std::pair<motor_point_t, bool>> *path =
		    this->tracker->getPath();
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

		wxPen pen(this->pointer_colour);
		wxBrush brush(this->pointer_colour);
		dc.SetPen(pen);
		dc.SetBrush(brush);
		motor_point_t point = this->base->getController()->getPosition();
		double _x = ((double) (point.x - plane_size.x) * scaleX);
		double _y = ((double) (plane_size.h + plane_size.y - point.y) * scaleY);
		dc.DrawRectangle((int) _x - 2, (int) _y - 2, 4, 4);
	}

	CalxVirtualPlaneDialog::CalxVirtualPlaneDialog(
	    wxWindow *win, wxWindowID id, std::shared_ptr<CoordHandle> base,
	    wxSize min)
	    : wxDialog::wxDialog(win, id, __("Preview"), wxDefaultPosition,
	                         wxDefaultSize,
	                         wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		this->mouseCoords = new wxStaticText(
		    this, wxID_ANY, __("Preview building may take some time."));
		sizer->Add(this->mouseCoords);
		this->plane = new CalxVirtualPlane(this, wxID_ANY, base, min);
		sizer->Add(this->plane, 1, wxALL | wxEXPAND, 5);

		okButton = new wxButton(this, wxID_OK, __("OK"));
		sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxVirtualPlaneDialog::OnOkClick, this);
		this->plane->Bind(wxEVT_MOTION, &CalxVirtualPlaneDialog::OnMouseMove, this);

		Layout();
		Fit();
	}

	std::shared_ptr<CoordPlaneStack> CalxVirtualPlaneDialog::getPlane() {
		return this->plane->getPlane();
	}

	std::shared_ptr<FloatCoordPlane> CalxVirtualPlaneDialog::getFloatPlane() {
		return this->plane->getFloatPlane();
	}

	void CalxVirtualPlaneDialog::setEnabled(bool en) {
		this->okButton->Enable(en);
	}

	void CalxVirtualPlaneDialog::OnOkClick(wxCommandEvent &evt) {
		Hide();
	}

	void CalxVirtualPlaneDialog::OnMouseMove(wxMouseEvent &evt) {
		if (evt.Leaving()) {
			this->mouseCoords->SetLabel("");
		}
		wxSize real_size = this->plane->GetSize();
		coord_rect_t plane_size = getFloatPlane()->getFloatSize();
		wxPoint mouse = this->plane->ScreenToClient(wxGetMousePosition());
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
