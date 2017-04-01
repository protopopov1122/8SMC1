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


#include "CalxTaskHandle.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalXUI {
	
	CalxTaskLinearStepHandle::CalxTaskLinearStepHandle(wxWindow *win, wxWindowID id)
		: CalxTaskStepHandle::CalxTaskStepHandle(win, id) {
		
		motor_point_t pos = {0, 0};
		this->step = new MoveTaskStep(pos, 1, false);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, pos.x);
		this->yCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, pos.y);
		this->speed = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1, 1, 0.01);
		this->relative = new wxCheckBox(this, wxID_ANY, "Relative");
		this->relative->SetValue(step->isRelative());
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Destination:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(xCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(yCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Speed:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(speed);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(relative);
		
		xCoord->Bind(wxEVT_SPINCTRL, &CalxTaskLinearStepHandle::OnFieldChange, this);
		yCoord->Bind(wxEVT_SPINCTRL, &CalxTaskLinearStepHandle::OnFieldChange, this);
		speed->Bind(wxEVT_SPINCTRLDOUBLE, &CalxTaskLinearStepHandle::OnFieldChange, this);
		relative->Bind(wxEVT_CHECKBOX, &CalxTaskLinearStepHandle::OnFieldChange, this);
		
		Layout();
	}
	
	MoveTaskStep *CalxTaskLinearStepHandle::getTaskStep() {
		return this->step;
	}
	
	void CalxTaskLinearStepHandle::update() {
		motor_point_t pos = {xCoord->GetValue(), yCoord->GetValue()};
		step->setPosition(pos);
		double speed = this->speed->GetValue();
		step->setSpeed((float) speed);
		step->setRelative(relative->GetValue());
	}
	
	void CalxTaskLinearStepHandle::OnFieldChange(wxCommandEvent &evt) {
		update();
	}
	
	CalxTaskLinearJumpStepHandle::CalxTaskLinearJumpStepHandle(wxWindow *win, wxWindowID id)
		: CalxTaskStepHandle::CalxTaskStepHandle(win, id) {
		
		motor_point_t pos = {0, 0};
		this->step = new JumpTaskStep(pos, 1, false);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, pos.x);
		this->yCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, pos.y);
		this->speed = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1, 1, 0.01);
		this->relative = new wxCheckBox(this, wxID_ANY, "Relative");
		this->relative->SetValue(step->isRelative());
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Destination:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(xCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(yCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Speed:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(speed);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(relative);
		
		xCoord->Bind(wxEVT_SPINCTRL, &CalxTaskLinearJumpStepHandle::OnFieldChange, this);
		yCoord->Bind(wxEVT_SPINCTRL, &CalxTaskLinearJumpStepHandle::OnFieldChange, this);
		speed->Bind(wxEVT_SPINCTRLDOUBLE, &CalxTaskLinearJumpStepHandle::OnFieldChange, this);
		relative->Bind(wxEVT_CHECKBOX, &CalxTaskLinearJumpStepHandle::OnFieldChange, this);
		
		Layout();
	}
	
	JumpTaskStep *CalxTaskLinearJumpStepHandle::getTaskStep() {
		return this->step;
	}
	
	void CalxTaskLinearJumpStepHandle::update() {
		motor_point_t pos = {xCoord->GetValue(), yCoord->GetValue()};
		step->setPosition(pos);
		double speed = this->speed->GetValue();
		step->setSpeed((float) speed);
		step->setRelative(relative->GetValue());
	}
	
	void CalxTaskLinearJumpStepHandle::OnFieldChange(wxCommandEvent &evt) {
		update();
	}
	
	CalxTaskArcStepHandle::CalxTaskArcStepHandle(wxWindow *win, wxWindowID id)
		: CalxTaskStepHandle::CalxTaskStepHandle(win, id) {
			
		motor_point_t pnt = {0, 0};
		this->step = new ArcTaskStep(pnt, pnt, 200, 1.0f, false);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->destXCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, pnt.x);
		this->destYCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, pnt.y);
		this->cenXCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, pnt.x);
		this->cenYCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, pnt.y);
		this->splitter = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, step->getSplitter());
		this->speed = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1, step->getSpeed(), 0.01);
		this->clockwise = new wxCheckBox(this, wxID_ANY, "Clockwise");
		clockwise->SetValue(step->isClockwise());
		this->relative = new wxCheckBox(this, wxID_ANY, "Relative");
		relative->SetValue(step->isRelative());
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Destination:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(destXCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(destYCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Center:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(cenXCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(cenYCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Speed:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(speed);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Splitter:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(splitter);
		sizer->Add(clockwise);
		sizer->Add(relative);		
		
		destXCoord->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange, this);
		destYCoord->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange, this);
		cenXCoord->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange, this);
		cenYCoord->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange, this);
		splitter->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange, this);
		speed->Bind(wxEVT_SPINCTRLDOUBLE, &CalxTaskArcStepHandle::OnFieldChange, this);
		clockwise->Bind(wxEVT_CHECKBOX, &CalxTaskArcStepHandle::OnFieldChange, this);
		relative->Bind(wxEVT_CHECKBOX, &CalxTaskArcStepHandle::OnFieldChange, this);
		
		
		Layout();
	}
	
	ArcTaskStep *CalxTaskArcStepHandle::getTaskStep() {
		return this->step;
	}
	
	void CalxTaskArcStepHandle::update() {
		motor_point_t dest = {destXCoord->GetValue(), destYCoord->GetValue()};
		motor_point_t cen = {cenXCoord->GetValue(), cenYCoord->GetValue()};
		float speed = this->speed->GetValue();
		int splitter = this->splitter->GetValue();
		bool cw = this->clockwise->GetValue();
		bool rel = this->clockwise->GetValue();
		
		step->setDestination(dest);
		step->setCenter(cen);
		step->setSpeed(speed);
		step->setSplitter(splitter);
		step->setClockwise(cw);
		step->setRelative(rel);
	}
	
	void CalxTaskArcStepHandle::OnFieldChange(wxCommandEvent &evt) {
		update();
	}
}