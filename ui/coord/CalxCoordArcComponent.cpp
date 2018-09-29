/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#include "ui/coord/CalxCoordActions.h"
#include "ui/coord/CalxCoordArcComponent.h"

namespace CalXUI {

	CalxCoordComponent *CalxCoordArcComponentFactory::newComponent(
	    wxWindow *win, CalxCoordController *ctrl) {
		return new CalxCoordArcComponent(win, wxID_ANY, ctrl);
	}

	CalxCoordArcComponent::CalxCoordArcComponent(wxWindow *win, wxWindowID id,
	                                             CalxCoordController *controller)
	    : CalxCoordComponent::CalxCoordComponent(win, id),
	      controller(controller) {
		std::string units = wxGetApp().getUnitProcessor().getUnits();
		wxFlexGridSizer *sizer = new wxFlexGridSizer(3);
		SetSizer(sizer);

		this->xCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->yCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->cxCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->cyCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->speed =
		    new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                         wxDefaultSize, wxSP_ARROW_KEYS, 0,
		                         wxGetApp()
		                             .getSystemManager()
		                             ->getConfiguration()
		                             .getEntry("units")
		                             ->getReal("unit_speed", 4000.0),
		                         wxGetApp()
		                             .getSystemManager()
		                             ->getConfiguration()
		                             .getEntry("units")
		                             ->getReal("unit_speed", 4000.0),
		                         wxGetApp().getUnitProcessor().getSpeedPrecision());
		this->splitter =
		    new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 200);
		this->clockwise = new wxCheckBox(this, wxID_ANY, __("Clockwise"));
		this->relative = new wxCheckBox(this, wxID_ANY, __("Relative"));
		wxButton *moveButton = new wxButton(this, wxID_ANY, __("Move"));

		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Destination") + std::string(":")));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("x") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(xCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(yCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Center") + std::string(":")));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("x") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(cxCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(cyCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Speed") + std::string(":")));
		sizer->Add(speed, 0, wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY,
		                            wxGetApp().getUnitProcessor().getSpeedUnits()));
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Splitter") + std::string(":")));
		sizer->Add(splitter, 0, wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(clockwise);
		sizer->Add(relative);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(moveButton);

		moveButton->Bind(wxEVT_BUTTON, &CalxCoordArcComponent::OnMoveClick, this);
	}

	void CalxCoordArcComponent::OnMoveClick(wxCommandEvent &evt) {
		coord_point_t dest = { this->xCoord->GetValue(), this->yCoord->GetValue() };
		coord_point_t cen = { this->cxCoord->GetValue(),
			                    this->cyCoord->GetValue() };
		this->controller->arc(dest, cen, this->splitter->GetValue(),
		                      this->speed->GetValue(), this->clockwise->GetValue(),
		                      this->relative->GetValue());
	}
}  // namespace CalXUI
