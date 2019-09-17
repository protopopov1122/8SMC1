/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ui/coord/CalxCoordPositionComponent.h"

namespace CalX::UI {

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

		SettingsConfiguration settingsConf(
		    wxGetApp().getSystemManager().getSettingsRepository(),
		    wxGetApp().getSystemManager().getConfiguration());
		ConfiguationFlatDictionary *confEntry =
		    wxGetApp().getSystemManager().getConfiguration().getEntry(
		        CalxConfiguration::RelativePosition);

		double xPosition =
		    settingsConf.getEntry(CalxConfiguration::RelativePosition)
		        ->getReal(CalxCoordPositionConfiguration::X, 0.5);
		double yPosition =
		    settingsConf.getEntry(CalxConfiguration::RelativePosition)
		        ->getReal(CalxCoordPositionConfiguration::Y, 0.5);

		this->xPos = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, 0, 1, xPosition,
		    confEntry->getReal(CalxCoordPositionConfiguration::XPrecision, 0.0001));
		this->yPos = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, 0, 1, yPosition,
		    confEntry->getReal(CalxCoordPositionConfiguration::YPrecision, 0.0001));
		this->speed = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, 0,
		    wxGetApp()
		        .getSystemManager()
		        .getConfiguration()
		        .getEntry(CalxConfiguration::Units)
		        ->getReal(CalxUnitConfiguration::UnitSpeed, 4000.0),
		    wxGetApp()
		        .getSystemManager()
		        .getConfiguration()
		        .getEntry(CalxConfiguration::Units)
		        ->getReal(CalxUnitConfiguration::UnitSpeed, 4000.0),
		    wxGetApp().getUnitProcessor().getSpeedPrecision());
		wxButton *moveButton = new wxButton(this, wxID_ANY, __("Move"));
		wxButton *configureButton = new wxButton(this, wxID_ANY, __("Configure"));
		wxButton *saveButton = new wxButton(this, wxID_ANY, __("Save"));

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
		sizer->Add(new wxStaticText(this, wxID_ANY,
		                            wxGetApp().getUnitProcessor().getSpeedUnits()));
		sizer->Add(moveButton);
		sizer->Add(configureButton);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(saveButton);

		moveButton->Bind(wxEVT_BUTTON, &CalxCoordPositionComponent::OnMoveClick,
		                 this);
		configureButton->Bind(wxEVT_BUTTON,
		                      &CalxCoordPositionComponent::OnConfigureClick, this);
		saveButton->Bind(wxEVT_BUTTON, &CalxCoordPositionComponent::OnSaveClick,
		                 this);
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

	void CalxCoordPositionComponent::OnSaveClick(wxCommandEvent &evt) {
		double xPosition = this->xPos->GetValue();
		double yPosition = this->yPos->GetValue();
		if (wxGetApp().getSystemManager().getSettingsRepository()) {
			auto &settings =
			    wxGetApp().getSystemManager().getSettingsRepository()->getSettings();
			settings.getEntry(CalxConfiguration::RelativePosition)
			    ->put(CalxCoordPositionConfiguration::X, xPosition);
			settings.getEntry(CalxConfiguration::RelativePosition)
			    ->put(CalxCoordPositionConfiguration::Y, yPosition);
		}
	}
}  // namespace CalX::UI
