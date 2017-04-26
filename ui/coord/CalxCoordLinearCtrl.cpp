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
	
	void CalxCoordLinearCtrl::init() {
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
        this->xCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
        this->yCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
        this->speed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                     wxDefaultSize, wxSP_ARROW_KEYS, 0,
                                     (int) wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("core")->getInt("maxspeed", 4000),
                                     (int) wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("core")->getInt("maxspeed", 4000));
		this->divisor = new wxChoice(this, wxID_ANY);
		this->divisor->Append("1");
		this->divisor->Append("2");
		this->divisor->Append("4");
		this->divisor->Append("8");
		this->divisor->SetSelection(3);
		this->relative = new wxCheckBox(this, wxID_ANY, __("Relative"));
		this->moveButton = new wxButton(this, wxID_ANY, __("Move"));
		this->jumpButton = new wxButton(this, wxID_ANY, __("Jump"));
		
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Destination") + std::string(":")));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, _("x") + std::string(":")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(xCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(yCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Speed") + std::string(":")));
		sizer->Add(speed, 0, wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Divisor") + std::string(":")));
		sizer->Add(divisor);
		sizer->Add(relative, 0, wxALIGN_CENTER);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(moveButton);
		sizer->Add(jumpButton);
	}
}
