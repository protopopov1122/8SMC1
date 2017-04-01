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

namespace CalXUI {
	
	CalxPlaneTracker::CalxPlaneTracker(motor_point_t pos, motor_rect_t sz)
		: VirtualCoordPlane::VirtualCoordPlane(pos, sz) {
			
	}
	
	CalxPlaneTracker::~CalxPlaneTracker() {
		
	}
	
	void CalxPlaneTracker::jump(motor_point_t point, bool move) {
		this->path.push_back(point);
	}
	
	std::vector<motor_point_t>* CalxPlaneTracker::getPath() {
		return &this->path;
	}
	
	void CalxPlaneTracker::reset() {
		this->path.clear();
	}
	
	CalxVirtualPlane::CalxVirtualPlane(wxWindow *win, wxWindowID id,
		CoordPlane *base, wxSize min) 
		: wxWindow::wxWindow(win, id) {
		
		this->tracker = new CalxPlaneTracker(base->getPosition(), base->getSize());
		this->stack = new CoordPlaneStack(this->tracker);
		SetMinSize(min);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxVirtualPlane::OnExit, this);
		this->Bind(wxEVT_PAINT, &CalxVirtualPlane::OnPaintEvent, this);
	}
	
	CoordPlaneStack *CalxVirtualPlane::getPlane() {
		return this->stack;
	}
	
	void CalxVirtualPlane::OnExit(wxCloseEvent &evt) {
		delete this->stack;
	}
	
	void CalxVirtualPlane::OnPaintEvent(wxPaintEvent &evt) {
		wxPaintDC dc(this);
		render(dc);
	}
	
	void CalxVirtualPlane::repaint() {
		wxClientDC dc(this);
		render(dc);
	}
	
	void CalxVirtualPlane::render(wxDC &dc) {
		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();
		dc.SetPen(*wxBLACK_PEN);
		dc.SetBrush(*wxBLACK_BRUSH);
		std::vector<motor_point_t> *path = this->tracker->getPath();
		motor_rect_t plane_size = this->getPlane()->getSize();
		wxSize real_size = GetSize();
		double scaleX, scaleY;
		scaleX = ((double) real_size.x) / plane_size.w;
		scaleY = ((double) real_size.y) / plane_size.h;
		for (size_t i = 0; i < path->size(); i++) {
			motor_point_t point = path->at(i);
			double x = ((double) (point.x - plane_size.x)) * scaleX;
			double y = real_size.y - ((double) (point.y - plane_size.y)) * scaleY;
			dc.DrawRectangle((int) x, (int) y, 2, 2);
		}
	}
	
	CalxVirtualPlaneDialog::CalxVirtualPlaneDialog(wxWindow *win, wxWindowID id,
		CoordPlane *base, wxSize min)
		: wxDialog::wxDialog(win , id, "Preview") {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		this->plane = new CalxVirtualPlane(this, wxID_ANY, base, min);
		sizer->Add(this->plane, 0, wxALL, 5);
		
		wxButton *okButton = new wxButton(this, wxID_ANY, "OK");
		sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxVirtualPlaneDialog::OnOkClick, this);
		
		Layout();
		Fit();
	}
	
	CoordPlaneStack *CalxVirtualPlaneDialog::getPlane() {
		return this->plane->getPlane();
	}
	
	void CalxVirtualPlaneDialog::OnOkClick(wxCommandEvent &evt) {
		Hide();
	}
}