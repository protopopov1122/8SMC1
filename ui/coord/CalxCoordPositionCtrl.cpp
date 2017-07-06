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

#include "ui/coord/CalxCoordPositionCtrl.h"
#include "wx/sizer.h"

namespace CalXUI {

  CalxCoordPositionCtrl::CalxCoordPositionCtrl(wxWindow *win, wxWindowID id)
	  : wxPanel::wxPanel(win, id) {
	wxFlexGridSizer *sizer = new wxFlexGridSizer(3);
	SetSizer(sizer);

	ConfigEntry *confEntry =
		wxGetApp().getSystemManager()->getConfiguration()->getEntry(
			"relative_pos");
	this->xPos = new wxSpinCtrlDouble(
		this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0, 1, confEntry->getReal("x", 0.5),
		confEntry->getReal("x_prec", 0.0001));
	this->yPos = new wxSpinCtrlDouble(
		this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		wxSP_ARROW_KEYS, 0, 1, confEntry->getReal("y", 0.5),
		confEntry->getReal("y_prec", 0.0001));
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
	this->moveButton = new wxButton(this, wxID_ANY, __("Move"));
	this->configureButton = new wxButton(this, wxID_ANY, __("Configure"));

	sizer->Add(
		new wxStaticText(this, wxID_ANY, __("Destination") + std::string(":")));
	sizer->Add(new wxStaticText(this, wxID_ANY, ""));
	sizer->Add(new wxStaticText(this, wxID_ANY, ""));
	sizer->Add(new wxStaticText(this, wxID_ANY, _("x") + std::string(":")), 0,
			   wxALIGN_RIGHT | wxRIGHT, 10);
	sizer->Add(xPos, 0, wxALL | wxEXPAND);
	sizer->Add(new wxStaticText(this, wxID_ANY, ""));
	sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
			   wxALIGN_RIGHT | wxRIGHT, 10);
	sizer->Add(yPos, 0, wxALL | wxEXPAND);
	sizer->Add(new wxStaticText(this, wxID_ANY, ""));
	sizer->Add(new wxStaticText(this, wxID_ANY, __("Speed") + std::string(":")),
			   0, wxALIGN_RIGHT | wxRIGHT, 10);
	sizer->Add(speed, 0, wxEXPAND);
	sizer->Add(new wxStaticText(this, wxID_ANY, wxGetApp().getSpeedUnits()));
	sizer->Add(moveButton);
	sizer->Add(configureButton);
  }
}
