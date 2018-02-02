/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#include "ui/coord/CalxCoordPositionComponent.h"

namespace CalXUI {

	CalxCoordComponent *CalxCoordPositionComponentFactory::newComponent(
	    wxWindow *win, CalxCoordController *ctrl) {
		return new CalxCoordPositionComponent(win, wxID_ANY, ctrl);
	}

	CalxCoordPositionComponent::CalxCoordPositionComponent(
	    wxWindow *win, wxWindowID id, CalxCoordController *controller)
	    : CalxCoordComponent::CalxCoordComponent(win, id),
	      controller(controller) {
		wxFlexGridSizer *sizer = new wxFlexGridSizer(3);
		SetSizer(sizer);

		std::shared_ptr<ConfigEntry> confEntry =
		    wxGetApp().getSystemManager()->getConfiguration().getEntry(
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
		                             .getEntry("units")
		                             ->getReal("unit_speed", 4000.0),
		                         wxGetApp()
		                             .getSystemManager()
		                             ->getConfiguration()
		                             .getEntry("units")
		                             ->getReal("unit_speed", 4000.0),
		                         wxGetApp().getUnitProcessor().getSpeedPrecision());
		wxButton *moveButton = new wxButton(this, wxID_ANY, __("Move"));
		wxButton *configureButton = new wxButton(this, wxID_ANY, __("Configure"));

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
		sizer->Add(new wxStaticText(this, wxID_ANY, wxGetApp().getUnitProcessor().getSpeedUnits()));
		sizer->Add(moveButton);
		sizer->Add(configureButton);

		moveButton->Bind(wxEVT_BUTTON, &CalxCoordPositionComponent::OnMoveClick,
		                 this);
		configureButton->Bind(wxEVT_BUTTON,
		                      &CalxCoordPositionComponent::OnConfigureClick, this);
	}

	void CalxCoordPositionComponent::OnMoveClick(wxCommandEvent &evt) {
		if (!this->controller->getHandle()->isMeasured()) {
			wxMessageBox(
			    __("Plane need to be measured before relative position change"),
			    __("Warning"), wxICON_WARNING);
			return;
		}

		coord_point_t dest = { this->xPos->GetValue(), this->yPos->GetValue() };
		double speed = this->speed->GetValue();
		this->controller->move(dest, speed);
	}

	void CalxCoordPositionComponent::OnConfigureClick(wxCommandEvent &evt) {
		coord_point_t dest = { this->xPos->GetValue(), this->yPos->GetValue() };
		double speed = this->speed->GetValue();
		this->controller->configure(dest, speed);
	}
}  // namespace CalXUI
