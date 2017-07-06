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

#include "ui/coord/CalxCoordCtrl.h"

namespace CalXUI {

  void CalxCoordLinearCtrl::init() {
	std::string units = wxGetApp().getUnits();
	wxFlexGridSizer *sizer = new wxFlexGridSizer(3);
	SetSizer(sizer);

	this->xCoord = new wxSpinCtrlDouble(
		this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0, wxGetApp().getUnitPrecision());
	this->yCoord = new wxSpinCtrlDouble(
		this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0, wxGetApp().getUnitPrecision());
	this->speed =
		new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
							 wxDefaultSize, wxSP_ARROW_KEYS, 0,
							 wxGetApp()
								 .getSystemManager()
								 ->getConfiguration()
								 ->getEntry("units")
								 ->getReal("unit_speed", 4000.0),
							 wxGetApp()
								 .getSystemManager()
								 ->getConfiguration()
								 ->getEntry("units")
								 ->getReal("unit_speed", 4000.0),
							 wxGetApp().getSpeedPrecision());
	this->relative = new wxCheckBox(this, wxID_ANY, __("Relative"));
	this->moveButton = new wxButton(this, wxID_ANY, __("Move"));
	this->jumpButton = new wxButton(this, wxID_ANY, __("Jump"));

	sizer->Add(
		new wxStaticText(this, wxID_ANY, __("Destination") + std::string(":")));
	sizer->Add(new wxStaticText(this, wxID_ANY, ""));
	sizer->Add(new wxStaticText(this, wxID_ANY, ""));
	sizer->Add(new wxStaticText(this, wxID_ANY, _("x") + std::string(":")), 0,
			   wxALIGN_RIGHT | wxRIGHT, 10);
	sizer->Add(xCoord, 0, wxALL | wxEXPAND);
	sizer->Add(new wxStaticText(this, wxID_ANY, units));
	sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
			   wxALIGN_RIGHT | wxRIGHT, 10);
	sizer->Add(yCoord, 0, wxALL | wxEXPAND);
	sizer->Add(new wxStaticText(this, wxID_ANY, units));
	sizer->Add(new wxStaticText(this, wxID_ANY, __("Speed") + std::string(":")),
			   0, wxALIGN_RIGHT | wxRIGHT, 10);
	sizer->Add(speed, 0, wxEXPAND);
	sizer->Add(new wxStaticText(this, wxID_ANY, wxGetApp().getSpeedUnits()));
	sizer->Add(relative, 0, wxALIGN_CENTER);
	sizer->Add(new wxStaticText(this, wxID_ANY, ""));
	sizer->Add(new wxStaticText(this, wxID_ANY, ""));
	sizer->Add(moveButton);
	sizer->Add(jumpButton);
  }
}
