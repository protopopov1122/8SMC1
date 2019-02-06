/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "ui/coord/CalxCoordFilterComponent.h"

namespace CalX::UI {

	wxDEFINE_EVENT(wxEVT_FILTER_UPDATE_OFFSET, wxThreadEvent);
	wxDEFINE_EVENT(wxEVT_FILTER_UPDATE_SCALE, wxThreadEvent);

	CalxCoordComponent *CalxCoordFilterComponentFactory::newComponent(
	    wxWindow *win, CalxCoordController *ctrl) {
		return new CalxCoordFilterComponent(win, wxID_ANY, ctrl);
	}

	CalxCoordFilterComponent::CalxCoordFilterComponent(
	    wxWindow *win, wxWindowID id, CalxCoordController *controller)
	    : CalxCoordComponent::CalxCoordComponent(win, id),
	      controller(controller) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		wxPanel *logPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(logPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *logSizer = new wxBoxSizer(wxHORIZONTAL);
		logPanel->SetSizer(logSizer);
		this->logActions = new wxCheckBox(logPanel, wxID_ANY, __("Log actions"));
		this->logErrors = new wxCheckBox(logPanel, wxID_ANY, __("Log errors"));
		logSizer->Add(logActions);
		logSizer->Add(logErrors);

		wxPanel *mapPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mapPanel, 0, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *mapSizer = new wxFlexGridSizer(2);
		mapPanel->SetSizer(mapSizer);
		double scale_prec = 0.00001;
		this->xOffset =
		    new wxSpinCtrl(mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yOffset =
		    new wxSpinCtrl(mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->xScale = new wxSpinCtrlDouble(
		    mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, scale_prec, INT_MAX, 1, scale_prec);
		this->yScale = new wxSpinCtrlDouble(
		    mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, scale_prec, INT_MAX, 1, scale_prec);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY,
		                               __("Coordinate offset") + std::string(":")));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, ""));
		mapSizer->Add(
		    new wxStaticText(mapPanel, wxID_ANY, __("x") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(xOffset, 0, wxEXPAND);
		mapSizer->Add(
		    new wxStaticText(mapPanel, wxID_ANY, __("y") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(yOffset, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY,
		                               __("Coordinate scale") + std::string(":")));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, ""));
		mapSizer->Add(
		    new wxStaticText(mapPanel, wxID_ANY, __("x") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(xScale, 0, wxEXPAND);
		mapSizer->Add(
		    new wxStaticText(mapPanel, wxID_ANY, __("y") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(yScale, 0, wxEXPAND);

		wxPanel *validatePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(validatePanel, 0, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *validateSizer = new wxFlexGridSizer(2);
		validatePanel->SetSizer(validateSizer);
		this->minx = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString,
		                            wxDefaultPosition, wxDefaultSize,
		                            wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->miny = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString,
		                            wxDefaultPosition, wxDefaultSize,
		                            wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->maxx = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString,
		                            wxDefaultPosition, wxDefaultSize,
		                            wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->maxy = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString,
		                            wxDefaultPosition, wxDefaultSize,
		                            wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->speed =
		    new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString,
		                   wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0,
		                   (int) wxGetApp()
		                       .getSystemManager()
		                       .getConfiguration()
		                       .getEntry(CalxConfiguration::Core)
		                       ->getInt(CalxCoordConfiguration::MaxSpeed, 4000),
		                   (int) wxGetApp()
		                       .getSystemManager()
		                       .getConfiguration()
		                       .getEntry(CalxConfiguration::Core)
		                       ->getInt(CalxCoordConfiguration::MaxSpeed, 4000));
		validateSizer->Add(
		    new wxStaticText(validatePanel, wxID_ANY,
		                     __("Coordinate validation") + std::string(":")));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, ""));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY,
		                                    __("min x") + std::string(":")),
		                   0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(minx, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY,
		                                    __("min y") + std::string(":")),
		                   0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(miny, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY,
		                                    __("max x") + std::string(":")),
		                   0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(maxx, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY,
		                                    __("max y") + std::string(":")),
		                   0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(maxy, 0, wxEXPAND);
		validateSizer->Add(
		    new wxStaticText(validatePanel, wxID_ANY, __("Maximum speed")));
		validateSizer->Add(speed, 0, wxEXPAND);

		logActions->SetValue(controller->isLoggingActions());
		logErrors->SetValue(controller->isLoggingErrors());
		xOffset->SetValue((int) controller->getOffset().x);
		yOffset->SetValue((int) controller->getOffset().y);
		xScale->SetValue((float) controller->getScale().x);
		yScale->SetValue((float) controller->getScale().y);
		minx->SetValue((int) controller->getValidateMinimum().x);
		miny->SetValue((int) controller->getValidateMinimum().y);
		maxx->SetValue((int) controller->getValidateMaximum().x);
		maxy->SetValue((int) controller->getValidateMaximum().y);
		speed->SetValue(static_cast<int>(controller->getValidateMaxSpeed()));

		logActions->Bind(wxEVT_CHECKBOX, &CalxCoordFilterComponent::OnFiltersUpdate,
		                 this);
		logErrors->Bind(wxEVT_CHECKBOX, &CalxCoordFilterComponent::OnFiltersUpdate,
		                this);
		xOffset->Bind(wxEVT_SPINCTRL, &CalxCoordFilterComponent::OnFiltersUpdate,
		              this);
		yOffset->Bind(wxEVT_SPINCTRL, &CalxCoordFilterComponent::OnFiltersUpdate,
		              this);
		xScale->Bind(wxEVT_TEXT, &CalxCoordFilterComponent::OnFiltersUpdate, this);
		yScale->Bind(wxEVT_TEXT, &CalxCoordFilterComponent::OnFiltersUpdate, this);
		minx->Bind(wxEVT_SPINCTRL, &CalxCoordFilterComponent::OnFiltersUpdate,
		           this);
		miny->Bind(wxEVT_SPINCTRL, &CalxCoordFilterComponent::OnFiltersUpdate,
		           this);
		maxx->Bind(wxEVT_SPINCTRL, &CalxCoordFilterComponent::OnFiltersUpdate,
		           this);
		maxy->Bind(wxEVT_SPINCTRL, &CalxCoordFilterComponent::OnFiltersUpdate,
		           this);
		speed->Bind(wxEVT_SPINCTRL, &CalxCoordFilterComponent::OnFiltersUpdate,
		            this);

		this->Bind(wxEVT_FILTER_UPDATE_OFFSET,
		           &CalxCoordFilterComponent::OnUpdateOffset, this);
		this->Bind(wxEVT_FILTER_UPDATE_SCALE,
		           &CalxCoordFilterComponent::OnUpdateScale, this);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxCoordFilterComponent::OnExit, this);
		this->controller->addFilterListener(this);
		Layout();
	}

	void CalxCoordFilterComponent::updateOffset(motor_point_t offset) {
		wxThreadEvent evt(wxEVT_FILTER_UPDATE_OFFSET);
		evt.SetPayload(offset);
		wxPostEvent(this, evt);
	}

	void CalxCoordFilterComponent::updateScale(motor_scale_t scale) {
		wxThreadEvent evt(wxEVT_FILTER_UPDATE_SCALE);
		evt.SetPayload(scale);
		wxPostEvent(this, evt);
	}

	void CalxCoordFilterComponent::OnUpdateOffset(wxThreadEvent &evt) {
		motor_point_t offset = evt.GetPayload<motor_point_t>();
		this->xOffset->SetValue(offset.x);
		this->yOffset->SetValue(offset.y);
	}

	void CalxCoordFilterComponent::OnUpdateScale(wxThreadEvent &evt) {
		motor_scale_t scale = evt.GetPayload<motor_scale_t>();
		this->xScale->SetValue(scale.x);
		this->yScale->SetValue(scale.y);
	}

	void CalxCoordFilterComponent::OnFiltersUpdate(wxCommandEvent &evt) {
		controller->setLoggingActions(logActions->GetValue());
		controller->setLoggingErrors(logErrors->GetValue());
		motor_point_t moffset = { xOffset->GetValue(), yOffset->GetValue() };
		motor_scale_t mscale = { xScale->GetValue(), yScale->GetValue() };
		controller->setOffset(moffset);
		controller->setScale(mscale);
		motor_point_t min = { minx->GetValue(), miny->GetValue() };
		motor_point_t max = { maxx->GetValue(), maxy->GetValue() };
		controller->setValidateMinimum(min);
		controller->setValidateMaximum(max);
		controller->setValidateMaxSpeed(speed->GetValue());
	}

	void CalxCoordFilterComponent::OnExit(wxCloseEvent &evt) {
		this->controller->removeFilterListener(this);
	}
}  // namespace CalX::UI
