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


#include "CalxVirtualPlane.h"
#include <wx/sizer.h>
#include <wx/utils.h>

namespace CalXUI {
	
	CalxPlaneTracker::CalxPlaneTracker(motor_point_t pos, motor_rect_t sz)
		: VirtualCoordPlane::VirtualCoordPlane(pos, sz) {
	}
	
	CalxPlaneTracker::~CalxPlaneTracker() {
		
	}
	
	void CalxPlaneTracker::jump(motor_point_t point, bool move) {
		this->path.push_back(std::pair<motor_point_t, bool>(point, move));
	}
	
	std::vector<std::pair<motor_point_t, bool>>* CalxPlaneTracker::getPath() {
		return &this->path;
	}
	
	void CalxPlaneTracker::reset() {
		this->path.clear();
	}
	
	CoordPlane *CalxPlaneTracker::clone(CoordPlane *plane) {
		return new CalxPlaneTracker(this->getPosition(), this->getSize());
	}
	
	CalxVirtualPlane::CalxVirtualPlane(wxWindow *win, wxWindowID id,
		CoordHandle *base, wxSize min) 
		: wxWindow::wxWindow(win, id) {
		
		this->tracker = new CalxPlaneTracker(base->getBase()->getPosition(), base->getBase()->getSize());
		this->stack = new CoordPlaneStack(base->clone(this->tracker));
		SetMinSize(min);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxVirtualPlane::OnExit, this);
		this->Bind(wxEVT_PAINT, &CalxVirtualPlane::OnPaintEvent, this);
		this->Bind(wxEVT_SIZE, &CalxVirtualPlane::OnResizeEvent, this);
	}
	
	CoordPlaneStack *CalxVirtualPlane::getPlane() {
		return this->stack;
	}

	CalxPlaneTracker *CalxVirtualPlane::getTracker() {
		return this->tracker;
	}
	
	void CalxVirtualPlane::OnExit(wxCloseEvent &evt) {
		delete this->stack;
		delete this->tracker;
	}
	
	void CalxVirtualPlane::OnPaintEvent(wxPaintEvent &evt) {
		wxPaintDC dc(this);
		render(dc);
	}
	
	void CalxVirtualPlane::OnResizeEvent(wxSizeEvent &evt) {
		Refresh();
	}
	
	void CalxVirtualPlane::repaint() {
		wxClientDC dc(this);
		render(dc);
	}
	
	void CalxVirtualPlane::render(wxDC &dc) {
		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();
		
		wxSize real_size = GetSize();
		dc.SetPen(*wxBLUE_PEN);
		dc.DrawLine(0, real_size.y / 2, real_size.x, real_size.y / 2);
		dc.DrawLine(real_size.x / 2, 0, real_size.x / 2, real_size.y);
		
		dc.SetPen(*wxBLACK_PEN);
		dc.SetBrush(*wxBLACK_BRUSH);
		double lastX = 0, lastY = 0;
		std::vector<std::pair<motor_point_t, bool>> *path = this->tracker->getPath();
		motor_rect_t plane_size = this->tracker->getSize();
		double scaleX, scaleY;
		scaleX = ((double) real_size.x) / plane_size.w;
		scaleY = ((double) real_size.y) / plane_size.h;
		for (size_t i = 0; i < path->size(); i++) {
			motor_point_t point = path->at(i).first;
			bool move = path->at(i).second;
			double x = ((double) (point.x - plane_size.x)) * scaleX;
			double y = real_size.y - ((double) (point.y - plane_size.y)) * scaleY;
			dc.DrawRectangle((int) x - 1, (int) y - 1, 2, 2);
			if (move) {
				dc.DrawLine(lastX, lastY, x, y);
			}
			lastX = x;
			lastY = y;
		}
		
		dc.SetPen(*wxBLACK_PEN);
		wxCoord x, y;
		dc.DrawText(std::to_string(plane_size.x), 0, real_size.y / 2);
		dc.GetMultiLineTextExtent(std::to_string(plane_size.x + plane_size.w), &x, &y);
		dc.DrawText(std::to_string(plane_size.x + plane_size.w), real_size.x - x, real_size.y / 2);
		dc.GetMultiLineTextExtent(std::to_string(plane_size.y), &x, &y);
		dc.DrawText(std::to_string(plane_size.y + plane_size.h), real_size.x / 2 - x / 2, 0);
		dc.GetMultiLineTextExtent(std::to_string(plane_size.y), &x, &y);
		dc.DrawText(std::to_string(plane_size.y), real_size.x / 2 - x / 2, real_size.y - y);
		
	}

	CalxVirtualPlaneDialog::CalxVirtualPlaneDialog(wxWindow *win, wxWindowID id,
		CoordHandle *base, wxSize min)
		: wxDialog::wxDialog(win , id, __("Preview"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		this->mouseCoords = new wxStaticText(this, wxID_ANY, __("Preview building may take some time."));
		sizer->Add(this->mouseCoords);
		this->plane = new CalxVirtualPlane(this, wxID_ANY, base, min);
		sizer->Add(this->plane, 1, wxALL | wxEXPAND, 5);
		
		wxButton *okButton = new wxButton(this, wxID_OK, __("OK"));
		sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxVirtualPlaneDialog::OnOkClick, this);
		this->plane->Bind(wxEVT_MOTION, &CalxVirtualPlaneDialog::OnMouseMove, this);
		
		Layout();
		Fit();
	}
	
	CoordPlaneStack *CalxVirtualPlaneDialog::getPlane() {
		return this->plane->getPlane();
	}
	
	void CalxVirtualPlaneDialog::OnOkClick(wxCommandEvent &evt) {
		Hide();
	}
	
	void CalxVirtualPlaneDialog::OnMouseMove(wxMouseEvent &evt) {
		if (evt.Leaving()) {
			this->mouseCoords->SetLabel("");
		}
		wxSize real_size = this->plane->GetSize();
		motor_rect_t plane_size = this->plane->getTracker()->getSize();
		wxPoint mouse = this->plane->ScreenToClient(wxGetMousePosition());
		if (mouse.x >= 0 &&
			mouse.y >= 0 &&
			mouse.x < real_size.x &&
			mouse.y < real_size.y) {
			motor_coord_t rx = (motor_coord_t) mouse.x * plane_size.w / real_size.x + plane_size.x;
			motor_coord_t ry = (motor_coord_t) plane_size.h - mouse.y * plane_size.h / real_size.y + plane_size.y;
			std::string res = __("x: ") + std::to_string(rx) + __("; y: ") + std::to_string(ry);
			this->mouseCoords->SetLabel(res);
		}
	}
	
}