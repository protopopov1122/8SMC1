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

#include "calx/ui/task/CalxProgrammedTask.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalX::UI {

	CalxTaskLinearStepHandle::CalxTaskLinearStepHandle(wxWindow *win,
	                                                   wxWindowID id)
	    : CalxTaskStepHandle::CalxTaskStepHandle(win, id) {
		std::string units = wxGetApp().getUnitProcessor().getUnits();

		motor_point_t pos = { 0, 0 };
		this->step = std::make_shared<MoveTaskStep>(pos, 1, false);

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
		this->speed =
		    new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                         wxDefaultSize, wxSP_ARROW_KEYS, 0, 1, 1, 0.001);
		this->relative = new wxCheckBox(this, wxID_ANY, __("Relative"));
		this->relative->SetValue(step->isRelative());

		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Destination") + std::string(":")),
		    0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT,
		           5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("x") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(xCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(yCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Speed") + std::string(":")),
		           0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(speed);
		sizer->Add(new wxStaticText(this, wxID_ANY,
		                            wxGetApp().getUnitProcessor().getSpeedUnits()));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT,
		           5);
		sizer->Add(relative);

		xCoord->Bind(wxEVT_SPINCTRL, &CalxTaskLinearStepHandle::OnFieldChange,
		             this);
		yCoord->Bind(wxEVT_SPINCTRL, &CalxTaskLinearStepHandle::OnFieldChange,
		             this);
		speed->Bind(wxEVT_SPINCTRLDOUBLE, &CalxTaskLinearStepHandle::OnFieldChange,
		            this);
		relative->Bind(wxEVT_CHECKBOX, &CalxTaskLinearStepHandle::OnFieldChange,
		               this);

		Layout();
	}

	std::shared_ptr<MoveTaskStep> CalxTaskLinearStepHandle::getTaskStep() {
		return this->step;
	}

	void CalxTaskLinearStepHandle::update() {
		double scale = wxGetApp().getUnitProcessor().getUnitScale();
		motor_point_t pos = {
			static_cast<motor_coord_t>(xCoord->GetValue() * scale),
			static_cast<motor_coord_t>(yCoord->GetValue() * scale)
		};
		step->setPosition(pos);
		double speed = this->speed->GetValue();
		step->setSpeed((float) speed);
		step->setRelative(relative->GetValue());
	}

	void CalxTaskLinearStepHandle::OnFieldChange(wxCommandEvent &evt) {
		update();
	}

	CalxTaskLinearJumpStepHandle::CalxTaskLinearJumpStepHandle(wxWindow *win,
	                                                           wxWindowID id)
	    : CalxTaskStepHandle::CalxTaskStepHandle(win, id) {
		std::string units = wxGetApp().getUnitProcessor().getUnits();

		motor_point_t pos = { 0, 0 };
		this->step = std::make_shared<JumpTaskStep>(pos, 1, false);

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
		this->speed =
		    new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                         wxDefaultSize, wxSP_ARROW_KEYS, 0, 1, 1, 0.001);
		this->relative = new wxCheckBox(this, wxID_ANY, __("Relative"));
		this->relative->SetValue(step->isRelative());

		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Destination") + std::string(":")),
		    0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT,
		           5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("x") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(xCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(yCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Speed") + std::string(":")),
		           0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(speed);
		sizer->Add(new wxStaticText(this, wxID_ANY,
		                            units.empty() ? "" : units + __("/sec")));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT,
		           5);
		sizer->Add(relative);

		xCoord->Bind(wxEVT_SPINCTRL, &CalxTaskLinearJumpStepHandle::OnFieldChange,
		             this);
		yCoord->Bind(wxEVT_SPINCTRL, &CalxTaskLinearJumpStepHandle::OnFieldChange,
		             this);
		speed->Bind(wxEVT_SPINCTRLDOUBLE,
		            &CalxTaskLinearJumpStepHandle::OnFieldChange, this);
		relative->Bind(wxEVT_CHECKBOX, &CalxTaskLinearJumpStepHandle::OnFieldChange,
		               this);

		Layout();
	}

	std::shared_ptr<JumpTaskStep> CalxTaskLinearJumpStepHandle::getTaskStep() {
		return this->step;
	}

	void CalxTaskLinearJumpStepHandle::update() {
		double scale = wxGetApp().getUnitProcessor().getUnitScale();
		motor_point_t pos = {
			static_cast<motor_coord_t>(xCoord->GetValue() * scale),
			static_cast<motor_coord_t>(yCoord->GetValue() * scale)
		};
		step->setPosition(pos);
		double speed = this->speed->GetValue();
		step->setSpeed((float) speed);
		step->setRelative(relative->GetValue());
	}

	void CalxTaskLinearJumpStepHandle::OnFieldChange(wxCommandEvent &evt) {
		update();
	}

	CalxTaskArcStepHandle::CalxTaskArcStepHandle(wxWindow *win, wxWindowID id)
	    : CalxTaskStepHandle::CalxTaskStepHandle(win, id) {
		std::string units = wxGetApp().getUnitProcessor().getUnits();

		motor_point_t pnt = { 0, 0 };
		ConfiguationFlatDictionary *coreEntry =
		    wxGetApp().getSystemManager().getConfiguration().getEntry(
		        CalxConfiguration::Core);
		this->step = std::make_shared<ArcTaskStep>(pnt, pnt, coreEntry->getInt(CalxCoreConfiguration::ChordCount, 200), 1.0f, false);

		wxFlexGridSizer *sizer = new wxFlexGridSizer(3);
		SetSizer(sizer);

		this->destXCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->destYCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, (int) pnt.y,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->cenXCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, (int) pnt.x,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->cenYCoord = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, (int) pnt.y,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->splitter = new wxSpinCtrl(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, 0, 1000, step->getSplitter());
		this->speed = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, 0, 1, step->getSpeed(), 0.01);
		this->clockwise = new wxCheckBox(this, wxID_ANY, __("Clockwise"));
		clockwise->SetValue(step->isClockwise());
		this->relative = new wxCheckBox(this, wxID_ANY, __("Relative"));
		relative->SetValue(step->isRelative());

		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Destination") + std::string(":")),
		    0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT,
		           5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("x") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(destXCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(destYCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Center") + std::string(":")), 0,
		    wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""), 0, wxALIGN_RIGHT | wxRIGHT,
		           5);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("x") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(cenXCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("y") + std::string(":")), 0,
		           wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(cenYCoord);
		sizer->Add(new wxStaticText(this, wxID_ANY, units));
		sizer->Add(new wxStaticText(this, wxID_ANY, __("Speed") + std::string(":")),
		           0, wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(speed);
		sizer->Add(new wxStaticText(this, wxID_ANY,
		                            units.empty() ? "" : units + __("/sec")));
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Splitter") + std::string(":")), 0,
		    wxALIGN_RIGHT | wxRIGHT, 5);
		sizer->Add(splitter);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(clockwise);
		sizer->Add(relative);

		destXCoord->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange,
		                 this);
		destYCoord->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange,
		                 this);
		cenXCoord->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange,
		                this);
		cenYCoord->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange,
		                this);
		splitter->Bind(wxEVT_SPINCTRL, &CalxTaskArcStepHandle::OnFieldChange, this);
		speed->Bind(wxEVT_SPINCTRLDOUBLE, &CalxTaskArcStepHandle::OnFieldChange,
		            this);
		clockwise->Bind(wxEVT_CHECKBOX, &CalxTaskArcStepHandle::OnFieldChange,
		                this);
		relative->Bind(wxEVT_CHECKBOX, &CalxTaskArcStepHandle::OnFieldChange, this);

		Layout();
	}

	std::shared_ptr<ArcTaskStep> CalxTaskArcStepHandle::getTaskStep() {
		return this->step;
	}

	void CalxTaskArcStepHandle::update() {
		double scale = wxGetApp().getUnitProcessor().getUnitScale();
		motor_point_t dest = {
			static_cast<motor_coord_t>(destXCoord->GetValue() * scale),
			static_cast<motor_coord_t>(destYCoord->GetValue() * scale)
		};
		motor_point_t cen = {
			static_cast<motor_coord_t>(cenXCoord->GetValue() * scale),
			static_cast<motor_coord_t>(cenYCoord->GetValue() * scale)
		};
		float speed = static_cast<float>(this->speed->GetValue());
		int splitter = this->splitter->GetValue();
		bool cw = this->clockwise->GetValue();
		bool rel = this->clockwise->GetValue();

		step->setDestination(dest);
		step->setCenter(cen);
		step->setSpeed(speed);
		step->setSplitter(splitter);
		step->setClockwise(cw);
		step->setRelative(rel);
	}

	void CalxTaskArcStepHandle::OnFieldChange(wxCommandEvent &evt) {
		update();
	}
}  // namespace CalX::UI
