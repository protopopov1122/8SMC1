/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "CalxCoordPlaneWatcher.h"
#include "coord/CalxCoordPanel.h"
#include "ui/CalxUnitProcessor.h"
#include <wx/sizer.h>

namespace CalXUI {
	
	wxDEFINE_EVENT(wxEVT_WATCHER_APPEND_POINT, wxThreadEvent);
	
	CalxCoordPlaneWatcherTimer::CalxCoordPlaneWatcherTimer(CalxCoordPlaneWatcher *watcher, CoordHandle *handle)
		: wxTimer::wxTimer() {
		this->watcher = watcher;
		this->handle = handle;
	}
	
	void CalxCoordPlaneWatcherTimer::Notify() {
		std::pair<motor_point_t, bool> pair = std::make_pair(handle->getController()->getPosition(), true);
		wxThreadEvent evt(wxEVT_WATCHER_APPEND_POINT);
		evt.SetPayload(pair);
		wxPostEvent(watcher, evt);
		watcher->Refresh();
	}
	
	CalxCoordPlaneWatcherEvent::CalxCoordPlaneWatcherEvent(CalxCoordPlaneWatcher *w) {
		this->watcher = w;
	}
	
	CalxCoordPlaneWatcherEvent::~CalxCoordPlaneWatcherEvent() {
		
	}
	
	void CalxCoordPlaneWatcherEvent::moved(CoordMoveEvent &evnt) {      
		std::pair<motor_point_t, bool> pair = std::make_pair(evnt.destination, evnt.synchrone);
		wxThreadEvent evt(wxEVT_WATCHER_APPEND_POINT);
		evt.SetPayload(pair);
		wxPostEvent(watcher, evt);
	}
	
	CalxCoordPlaneWatcher::CalxCoordPlaneWatcher(wxWindow *win, wxWindowID id, wxSize min, CoordHandle *handle)
		: wxWindow::wxWindow(win, id) {
		
		this->handle = handle;
        wxGetApp().getMainFrame()->getPanel()->getCoords()->bindWatcher((device_id_t) handle->getID(), this);
		
		SetMinSize(min);
		
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxCoordPlaneWatcher::OnExit, this);
		this->Bind(wxEVT_PAINT, &CalxCoordPlaneWatcher::OnPaintEvent, this);
		this->Bind(wxEVT_SIZE, &CalxCoordPlaneWatcher::OnResizeEvent, this);
		this->Bind(wxEVT_WATCHER_APPEND_POINT, &CalxCoordPlaneWatcher::OnAppendEvent, this);
		
		this->history.push_back(std::make_pair(this->handle->getController()->getPosition(), false));
		
		int_conf_t interval = wxGetApp().getSystemManager()->getConfiguration()->getEntry("ui")->getInt("watcher_interval", 5);
		if (interval != -1) {
			this->timer = new CalxCoordPlaneWatcherTimer(this, this->handle);
            this->timer->Start((int) interval);
		} else {
			this->timer = nullptr;
		}

		this->listener = new CalxCoordPlaneWatcherEvent(this);
		this->handle->addEventListener(this->listener);
	}
	
	void CalxCoordPlaneWatcher::clear() {
		this->history.clear();
		this->history.push_back(std::make_pair(this->handle->getController()->getPosition(), false));
		Refresh();
	}
	
	void CalxCoordPlaneWatcher::add(motor_point_t point, bool sync) {
		if (!this->history.empty()) {
			motor_point_t last = this->history.at(this->history.size() - 1).first;
			if (last.x == point.x &&
				last.y == point.y) {
				return;	
			}
		}
		this->history.push_back(std::make_pair(point, sync));
	}

	void CalxCoordPlaneWatcher::OnPaintEvent(wxPaintEvent &evt) {
		wxPaintDC dc(this);
		render(dc);
	}
	
	void CalxCoordPlaneWatcher::OnResizeEvent(wxSizeEvent &evt) {
		Refresh();
	}
	
	void CalxCoordPlaneWatcher::OnExit(wxCloseEvent &evt) {
		if (this->timer != nullptr) {
			this->timer->Stop();
		}
        wxGetApp().getMainFrame()->getPanel()->getCoords()->unbindWatcher((device_id_t) handle->getID(), this);
		this->handle->removeEventListener(this->listener);
		Destroy();
	}
	
	void CalxCoordPlaneWatcher::OnAppendEvent(wxThreadEvent &evt) {
		std::pair<motor_point_t, bool> pair = evt.GetPayload<std::pair<motor_point_t, bool>>();
		this->add(pair.first, pair.second);
	}
	
	void CalxCoordPlaneWatcher::render(wxDC &dc) {
		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();
		
		wxSize real_size = GetSize();
		motor_rect_t plane_size = this->handle->getController()->getSize();
		motor_rect_t top_plane_size = this->handle->getSize();
		double scaleX, scaleY, top_scaleX, top_scaleY;
		scaleX = ((double) real_size.x) / plane_size.w;
		scaleY = ((double) real_size.y) / plane_size.h;
		top_scaleX = ((double) real_size.x) / top_plane_size.w;
		top_scaleY = ((double) real_size.y) / top_plane_size.h;
		dc.SetPen(*wxBLUE_PEN);
		dc.DrawLine(0, (wxCoord) ((top_plane_size.h + top_plane_size.y) * top_scaleY), real_size.x, (wxCoord) ((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.DrawLine((wxCoord) ((-top_plane_size.x) * top_scaleX), 0, (wxCoord) ((-top_plane_size.x) * top_scaleX), real_size.y);
		
		dc.SetPen(*wxBLACK_PEN);
		dc.SetBrush(*wxBLACK_BRUSH);
		double lastX = 0, lastY = 0;
		std::vector<std::pair<motor_point_t, bool>> *path = &this->history;
		for (size_t i = 0; i < path->size(); i++) {
			motor_point_t point = path->at(i).first;
			bool move = path->at(i).second;
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
		wxCoord x, y;
		dc.DrawText(std::string(wxGetApp().getUnitProcessor()->toUnitsStr(top_plane_size.x)), 0, (wxCoord) ((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.GetMultiLineTextExtent(std::string(wxGetApp().getUnitProcessor()->toUnitsStr(top_plane_size.x + top_plane_size.w)), &x, &y);
		dc.DrawText(std::string(wxGetApp().getUnitProcessor()->toUnitsStr(top_plane_size.x + top_plane_size.w)), real_size.x - x, (wxCoord) ((top_plane_size.h + top_plane_size.y) * top_scaleY));
		dc.GetMultiLineTextExtent(std::string(wxGetApp().getUnitProcessor()->toUnitsStr(-(top_plane_size.y + top_plane_size.h))), &x, &y);
		dc.DrawText(std::string(wxGetApp().getUnitProcessor()->toUnitsStr(top_plane_size.y)), (wxCoord) ((-top_plane_size.x) * top_scaleX) - x / 2, real_size.y - y);
		dc.GetMultiLineTextExtent(std::string(wxGetApp().getUnitProcessor()->toUnitsStr(-top_plane_size.y)), &x, &y);
		dc.DrawText(std::string(wxGetApp().getUnitProcessor()->toUnitsStr(top_plane_size.y + top_plane_size.h)), (wxCoord) ((-top_plane_size.x) * top_scaleX) - x / 2, 0);
		
		dc.SetPen(*wxRED_PEN);
		dc.SetBrush(*wxRED_BRUSH);
		motor_point_t point = this->handle->getController()->getPosition();
		double _x = ((double) (point.x - plane_size.x) * scaleX);
		double _y = ((double) (plane_size.h + plane_size.y - point.y) * scaleY);
		dc.DrawRectangle((int) _x - 2, (int) _y - 2, 4, 4);
	}
	
	CalxCoordPlaneWatcherDialog::CalxCoordPlaneWatcherDialog(wxWindow *win, wxWindowID id, CoordHandle *handle)
		: wxDialog::wxDialog(win, id, FORMAT(__("Coordinate plane #%s Watcher"), std::to_string(handle->getID())), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
			
		this->handle = handle;
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		this->mouseCoords = new wxStaticText(this, wxID_ANY, "");
		sizer->Add(this->mouseCoords);
		this->watcher = new CalxCoordPlaneWatcher(this, wxID_ANY, wxSize(500, 500), handle);
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
		clearButton->Bind(wxEVT_BUTTON, &CalxCoordPlaneWatcherDialog::OnClearClick, this);
		this->watcher->Bind(wxEVT_MOTION, &CalxCoordPlaneWatcherDialog::OnMouseMove, this);
		
		Layout();
		Fit();
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
		motor_rect_t plane_size = this->handle->getSize();
		wxPoint mouse = this->watcher->ScreenToClient(wxGetMousePosition());
		if (mouse.x >= 0 &&
			mouse.y >= 0 &&
			mouse.x < real_size.x &&
			mouse.y < real_size.y) {
			motor_coord_t rx = (motor_coord_t) mouse.x * plane_size.w / real_size.x + plane_size.x;
			motor_coord_t ry = (motor_coord_t) plane_size.h - mouse.y * plane_size.h / real_size.y + plane_size.y;
			std::string res = FORMAT(__("x: %s; y: %s"), wxGetApp().getUnitProcessor()->toTextUnits(rx),
				wxGetApp().getUnitProcessor()->toTextUnits(ry));
			this->mouseCoords->SetLabel(res);
		}
	}
}
