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


#include "CalxCoordCtrl.h"

namespace CalXUI {
	
	
	void CalxCoordArcCtrl::init() {
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->cxCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->cyCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->speed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 4000, 4000);
		this->divisor = new wxChoice(this, wxID_ANY);
		this->divisor->Append("1");
		this->divisor->Append("2");
		this->divisor->Append("4");
		this->divisor->Append("8");
		this->divisor->SetSelection(3);
		this->splitter = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 200);
		this->clockwise = new wxCheckBox(this, wxID_ANY, "Clockwise");
		this->relative = new wxCheckBox(this, wxID_ANY, "Relative");
		wxButton *moveButton = new wxButton(this, wxID_ANY, "Move");
		moveButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnArcMoveClick, ctrl);
		
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Destination:"));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(xCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(yCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Center:"));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(cxCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(cyCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Speed:"));
		sizer->Add(speed, 0, wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Divisor:"));
		sizer->Add(divisor);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Splitter:"));
		sizer->Add(splitter, 0, wxEXPAND);
		sizer->Add(clockwise);
		sizer->Add(relative);
		sizer->Add(moveButton);
		
		
	}
}