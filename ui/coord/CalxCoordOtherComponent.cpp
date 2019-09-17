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

#include "calx/ui/coord/CalxCoordOtherComponent.h"
#include <wx/collpane.h>

namespace CalX::UI {

	CalxCoordComponent *CalxCoordOtherComponentFactory::newComponent(
	    wxWindow *win, CalxCoordController *ctrl) {
		return new CalxCoordOtherComponent(win, wxID_ANY, ctrl);
	}

	CalxCoordOtherComponent::CalxCoordOtherComponent(
	    wxWindow *win, wxWindowID id, CalxCoordController *controller)
	    : CalxCoordComponent::CalxCoordComponent(win, id),
	      controller(controller) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		if (controller->getHandle()->getController()->getInstrument() != nullptr) {
			this->enableInstrument =
			    new wxCheckBox(this, wxID_ANY, __("Enable instrument"));
			sizer->Add(enableInstrument, 0, wxALL, 5);
			enableInstrument->Bind(wxEVT_CHECKBOX,
			                       &CalxCoordOtherComponent::OnInstrumentEnableClick,
			                       this);
			this->enableInstrument->SetValue(controller->getHandle()
			                                     ->getController()
			                                     ->getInstrument()
			                                     ->isRunnable());
		}

		wxButton *curPosZero =
		    new wxButton(this, wxID_ANY, __("Current position as 0"));
		sizer->Add(curPosZero, 0, wxALL, 5);
		curPosZero->Bind(wxEVT_BUTTON,
		                 &CalxCoordOtherComponent::OnAdjustPositionClick, this);

		wxPanel *calibratePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(calibratePanel, 0, wxALL, 5);
		wxBoxSizer *calibrateSizer = new wxBoxSizer(wxHORIZONTAL);
		calibratePanel->SetSizer(calibrateSizer);
		wxButton *calibrateButton =
		    new wxButton(calibratePanel, wxID_ANY, __("Calibrate"));
		calibrateButton->Bind(wxEVT_BUTTON,
		                      &CalxCoordOtherComponent::OnCalibrateClick, this);
		calibrateSizer->Add(calibrateButton);
		calibrateSizer->Add(new wxStaticText(calibratePanel, wxID_ANY, __("to")), 0,
		                    wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		trailer = new wxChoice(calibratePanel, wxID_ANY);
		trailer->Append(__("trailer 1"));
		trailer->Append(__("trailer 2"));
		trailer->SetSelection(0);
		calibrateSizer->Add(trailer, 0, wxALL);

		wxPanel *measurePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(measurePanel, 0, wxBOTTOM | wxLEFT, 5);
		wxBoxSizer *measureSizer = new wxBoxSizer(wxHORIZONTAL);
		measurePanel->SetSizer(measureSizer);
		wxButton *measureButton =
		    new wxButton(measurePanel, wxID_ANY, __("Measure"));
		measureButton->Bind(wxEVT_BUTTON, &CalxCoordOtherComponent::OnMeasureClick,
		                    this);
		measureSizer->Add(measureButton);
		measureSizer->Add(new wxStaticText(measurePanel, wxID_ANY, __("to")), 0,
		                  wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		measureTrailer = new wxChoice(measurePanel, wxID_ANY);
		measureTrailer->Append(__("trailer 1"));
		measureTrailer->Append(__("trailer 2"));
		measureTrailer->SetSelection(0);
		measureSizer->Add(measureTrailer, 0, wxALL);

		Layout();
	}

	void CalxCoordOtherComponent::OnInstrumentEnableClick(wxCommandEvent &evt) {
		this->controller->getHandle()
		    ->getController()
		    ->getInstrument()
		    ->setRunnable(this->enableInstrument->GetValue());
	}

	void CalxCoordOtherComponent::OnAdjustPositionClick(wxCommandEvent &evt) {
		motor_point_t offset = this->controller->getHandle()->getPosition();
		motor_scale_t scale = this->controller->getScale();
		offset.x += this->controller->getOffset().x / scale.x;
		offset.y += this->controller->getOffset().y / scale.y;
		controller->setOffset(offset);
	}

	void CalxCoordOtherComponent::OnCalibrateClick(wxCommandEvent &evt) {
		TrailerId tr = this->trailer->GetSelection() == 0 ? TrailerId::Trailer1
		                                                  : TrailerId::Trailer2;
		this->controller->calibrate(tr);
	}

	void CalxCoordOtherComponent::OnMeasureClick(wxCommandEvent &evt) {
		TrailerId tr = this->trailer->GetSelection() == 0 ? TrailerId::Trailer1
		                                                  : TrailerId::Trailer2;
		this->controller->measure(tr);
	}
}  // namespace CalX::UI
