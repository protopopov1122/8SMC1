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
		this->speed = new wxTextCtrl(this, wxID_ANY, std::to_string(step->getSpeed()));
		this->relative = new wxCheckBox(this, wxID_ANY, "Relative");
		this->relative->SetValue(step->isRelative());
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "X coordinate:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(xCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Y coordinate:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(yCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Speed:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(speed);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(relative);
		
		Layout();
	}
	
	MoveTaskStep *CalxTaskLinearStepHandle::getTaskStep() {
		return this->step;
	}
	
	void CalxTaskLinearStepHandle::update() {
		motor_point_t pos = {xCoord->GetValue(), yCoord->GetValue()};
		step->setPosition(pos);
		double speed;
		this->speed->GetValue().ToDouble(&speed);
		step->setSpeed((float) speed);
		step->setRelative(relative->GetValue());
	}
}