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
#include <wx/collpane.h>

namespace CalXUI {
	
	
	void CalxCoordOtherCtrl::init() {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		if (ctrl->getHandle()->getController()->getInstrument() != nullptr) {
			this->enableInstrument = new wxCheckBox(this, wxID_ANY, __("Enable instrument"));
			sizer->Add(enableInstrument, 0, wxALL, 5);
			enableInstrument->Bind(wxEVT_CHECKBOX, &CalxCoordCtrl::OnInstrumentEnableClick, ctrl);
			this->enableInstrument->SetValue(ctrl->getHandle()->getController()->getInstrument()->isRunnable());
		}
		
		wxPanel *calibratePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(calibratePanel, 0, wxALL, 5);
		wxBoxSizer *calibrateSizer = new wxBoxSizer(wxHORIZONTAL);
		calibratePanel->SetSizer(calibrateSizer);
		wxButton *calibrateButton = new wxButton(calibratePanel, wxID_ANY, __("Calibrate"));
		calibrateButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnCalibrateClick, ctrl);
		calibrateSizer->Add(calibrateButton);
		calibrateSizer->Add(new wxStaticText(calibratePanel, wxID_ANY, __("to")), 0, wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		trailer = new wxChoice(calibratePanel, wxID_ANY);
		trailer->Append(__("trailer 1"));
		trailer->Append(__("trailer 2"));
		trailer->SetSelection(0);
		calibrateSizer->Add(trailer, 0, wxALL);
		
		wxPanel *measurePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(measurePanel, 0, wxBOTTOM | wxLEFT, 5);
		wxBoxSizer *measureSizer = new wxBoxSizer(wxHORIZONTAL);
		measurePanel->SetSizer(measureSizer);
		wxButton *measureButton = new wxButton(measurePanel, wxID_ANY, __("Measure"));
		measureButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnMeasureClick, ctrl);
		measureSizer->Add(measureButton);
		measureSizer->Add(new wxStaticText(measurePanel, wxID_ANY, __("to")), 0, wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		measureTrailer = new wxChoice(measurePanel, wxID_ANY);
		measureTrailer->Append(__("trailer 1"));
		measureTrailer->Append(__("trailer 2"));
		measureTrailer->SetSelection(0);
		measureSizer->Add(measureTrailer, 0, wxALL);
		
		wxCollapsiblePane  *posCollPane = new wxCollapsiblePane(this, wxID_ANY, __("Position"));
		wxWindow *posPane = posCollPane->GetPane();
		sizer->Add(posCollPane, 0, wxALL | wxEXPAND);
		wxBoxSizer *posSizer = new wxBoxSizer(wxVERTICAL);
		posPane->SetSizer(posSizer);
		posCollPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &CalxCoordCtrl::OnInterfaceUpdate, ctrl);
		this->posCtrl = new CalxCoordPositionCtrl(posPane, wxID_ANY);
		posSizer->Add(this->posCtrl);
		this->posCtrl->getMoveButton()->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnPositionChangeClick, ctrl);
		this->posCtrl->getConfigureButton()->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnConfigureClick, ctrl);
		
		wxCollapsiblePane  *filtersCollPane = new wxCollapsiblePane(this, wxID_ANY, __("Filters"));
		wxWindow *filtersPane = filtersCollPane->GetPane();
		sizer->Add(filtersCollPane, 1, wxALL | wxEXPAND);
		wxBoxSizer *filtersSizer = new wxBoxSizer(wxVERTICAL);
		filtersPane->SetSizer(filtersSizer);
		filtersCollPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &CalxCoordCtrl::OnInterfaceUpdate, ctrl);
		
		
		wxPanel *logPanel = new wxPanel(filtersPane, wxID_ANY);
		filtersSizer->Add(logPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *logSizer = new wxBoxSizer(wxHORIZONTAL);
		logPanel->SetSizer(logSizer);
		this->logActions = new wxCheckBox(logPanel, wxID_ANY, __("Log actions"));		
		this->logErrors = new wxCheckBox(logPanel, wxID_ANY, __("Log errors"));
		logSizer->Add(logActions);
		logSizer->Add(logErrors);
		
		wxPanel *mapPanel = new wxPanel(filtersPane, wxID_ANY);
		filtersSizer->Add(mapPanel, 0, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *mapSizer = new wxFlexGridSizer(2);
		mapPanel->SetSizer(mapSizer);
        this->xOffset = new wxSpinCtrl(mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                       wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
        this->yOffset = new wxSpinCtrl(mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                       wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->xScale = new wxTextCtrl(mapPanel, wxID_ANY, "1");
		this->yScale = new wxTextCtrl(mapPanel, wxID_ANY, "1");
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, __("Coordinate offset") + std::string(":")));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, ""));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, __("x") + std::string(":")), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(xOffset, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, __("y") + std::string(":")), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(yOffset, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, __("Coordinate scale") + std::string(":")));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, ""));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, __("x") + std::string(":")), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(xScale, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, __("y") + std::string(":")), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(yScale, 0, wxEXPAND);
		
		wxPanel *validatePanel = new wxPanel(filtersPane, wxID_ANY);
		filtersSizer->Add(validatePanel, 0, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *validateSizer = new wxFlexGridSizer(2);
		validatePanel->SetSizer(validateSizer);
        this->minx = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
        this->miny = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
        this->maxx = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
        this->maxy = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
        this->speed = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                     wxDefaultSize, wxSP_ARROW_KEYS, 0,
                                     (int) wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("core")->getInt("maxspeed", 4000),
                                     (int) wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("core")->getInt("maxspeed", 4000));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, __("Coordinate validation") + std::string(":")));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, ""));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, __("min x") + std::string(":")), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(minx, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, __("min y") + std::string(":")), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(miny, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, __("max x") + std::string(":")), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(maxx, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, __("max y") + std::string(":")), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(maxy, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, __("Maximum speed")));
		validateSizer->Add(speed, 0, wxEXPAND);
		
		logActions->SetValue(ctrl->getPlaneLog()->isLoggingActions());
		logErrors->SetValue(ctrl->getPlaneLog()->isLoggingErrors());
        xOffset->SetValue((int) ctrl->getPlaneMap()->getOffset().x);
        yOffset->SetValue((int) ctrl->getPlaneMap()->getOffset().y);
		xScale->SetValue(std::to_string((float) ctrl->getPlaneMap()->getScale().x));
		yScale->SetValue(std::to_string((float) ctrl->getPlaneMap()->getScale().y));
        minx->SetValue((int) ctrl->getPlaneValidator()->getMinimum().x);
        miny->SetValue((int) ctrl->getPlaneValidator()->getMinimum().y);
        maxx->SetValue((int) ctrl->getPlaneValidator()->getMaximum().x);
        maxy->SetValue((int) ctrl->getPlaneValidator()->getMaximum().y);
        speed->SetValue(static_cast<int>(ctrl->getPlaneValidator()->getMaxSpeed()));
		
		logActions->Bind(wxEVT_CHECKBOX, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		logErrors->Bind(wxEVT_CHECKBOX, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		xOffset->Bind(wxEVT_SPINCTRL, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		yOffset->Bind(wxEVT_SPINCTRL, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		xScale->Bind(wxEVT_TEXT, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		yScale->Bind(wxEVT_TEXT, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		minx->Bind(wxEVT_SPINCTRL, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		miny->Bind(wxEVT_SPINCTRL, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		maxx->Bind(wxEVT_SPINCTRL, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		maxy->Bind(wxEVT_SPINCTRL, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		speed->Bind(wxEVT_SPINCTRL, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
		
		Layout();
	}
}
