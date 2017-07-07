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
#include <wx/collpane.h>

namespace CalXUI {

  CalxCoordOtherCtrl::CalxCoordOtherCtrl(wxWindow *win, wxWindowID id,
	CalxCoordController *controller, CalxFilterController *filters)
	: wxPanel::wxPanel(win, id), controller(controller), filters(filters) {
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);

	if (controller->getHandle()->getController()->getInstrument() != nullptr) {
	  this->enableInstrument =
		  new wxCheckBox(this, wxID_ANY, __("Enable instrument"));
	  sizer->Add(enableInstrument, 0, wxALL, 5);
	  enableInstrument->Bind(wxEVT_CHECKBOX,
							 &CalxCoordOtherCtrl::OnInstrumentEnableClick, this);
	  this->enableInstrument->SetValue(
		  controller->getHandle()->getController()->getInstrument()->isRunnable());
	}

	wxButton *curPosZero =
		new wxButton(this, wxID_ANY, __("Current position as 0"));
	sizer->Add(curPosZero, 0, wxALL, 5);
	curPosZero->Bind(wxEVT_BUTTON, &CalxCoordOtherCtrl::OnAdjustPositionClick, this);

	wxPanel *calibratePanel = new wxPanel(this, wxID_ANY);
	sizer->Add(calibratePanel, 0, wxALL, 5);
	wxBoxSizer *calibrateSizer = new wxBoxSizer(wxHORIZONTAL);
	calibratePanel->SetSizer(calibrateSizer);
	wxButton *calibrateButton =
		new wxButton(calibratePanel, wxID_ANY, __("Calibrate"));
	calibrateButton->Bind(wxEVT_BUTTON, &CalxCoordOtherCtrl::OnCalibrateClick, this);
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
	measureButton->Bind(wxEVT_BUTTON, &CalxCoordOtherCtrl::OnMeasureClick, this);
	measureSizer->Add(measureButton);
	measureSizer->Add(new wxStaticText(measurePanel, wxID_ANY, __("to")), 0,
					  wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
	measureTrailer = new wxChoice(measurePanel, wxID_ANY);
	measureTrailer->Append(__("trailer 1"));
	measureTrailer->Append(__("trailer 2"));
	measureTrailer->SetSelection(0);
	measureSizer->Add(measureTrailer, 0, wxALL);

	posCollPane =
		new wxCollapsiblePane(this, wxID_ANY, __("Position"));
	wxWindow *posPane = posCollPane->GetPane();
	sizer->Add(posCollPane, 0, wxALL | wxEXPAND);
	wxBoxSizer *posSizer = new wxBoxSizer(wxVERTICAL);
	posPane->SetSizer(posSizer);
	this->posCtrl = new CalxCoordPositionCtrl(posPane, wxID_ANY, this->controller);
	posSizer->Add(this->posCtrl);
	posCollPane->Collapse(false);

	filtersCollPane =
		new wxCollapsiblePane(this, wxID_ANY, __("Filters"));
	wxWindow *filtersPane = filtersCollPane->GetPane();
	sizer->Add(filtersCollPane, 1, wxALL | wxEXPAND);
	wxBoxSizer *filtersSizer = new wxBoxSizer(wxVERTICAL);
	filtersPane->SetSizer(filtersSizer);

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

	wxPanel *validatePanel = new wxPanel(filtersPane, wxID_ANY);
	filtersSizer->Add(validatePanel, 0, wxALL | wxEXPAND, 5);
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
						   ->getConfiguration()
						   ->getEntry("core")
						   ->getInt("maxspeed", 4000),
					   (int) wxGetApp()
						   .getSystemManager()
						   ->getConfiguration()
						   ->getEntry("core")
						   ->getInt("maxspeed", 4000));
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

	logActions->SetValue(controller->getLogFilter()->isLoggingActions());
	logErrors->SetValue(controller->getLogFilter()->isLoggingErrors());
	xOffset->SetValue((int) controller->getMapFilter()->getOffset().x);
	yOffset->SetValue((int) controller->getMapFilter()->getOffset().y);
	xScale->SetValue(std::to_string((float) controller->getMapFilter()->getScale().x));
	yScale->SetValue(std::to_string((float) controller->getMapFilter()->getScale().y));
	minx->SetValue((int) controller->getValidateFilter()->getMinimum().x);
	miny->SetValue((int) controller->getValidateFilter()->getMinimum().y);
	maxx->SetValue((int) controller->getValidateFilter()->getMaximum().x);
	maxy->SetValue((int) controller->getValidateFilter()->getMaximum().y);
	speed->SetValue(static_cast<int>(controller->getValidateFilter()->getMaxSpeed()));

	logActions->Bind(wxEVT_CHECKBOX, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	logErrors->Bind(wxEVT_CHECKBOX, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	xOffset->Bind(wxEVT_SPINCTRL, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	yOffset->Bind(wxEVT_SPINCTRL, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	xScale->Bind(wxEVT_TEXT, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	yScale->Bind(wxEVT_TEXT, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	minx->Bind(wxEVT_SPINCTRL, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	miny->Bind(wxEVT_SPINCTRL, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	maxx->Bind(wxEVT_SPINCTRL, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	maxy->Bind(wxEVT_SPINCTRL, &CalxCoordOtherCtrl::OnFiltersUpdate, this);
	speed->Bind(wxEVT_SPINCTRL, &CalxCoordOtherCtrl::OnFiltersUpdate, this);

	Layout();
  }
  
  wxCollapsiblePane *CalxCoordOtherCtrl::getPositionPane() {
	return this->posCollPane;
  }
  
  wxCollapsiblePane *CalxCoordOtherCtrl::getFiltersPane() {
	return this->filtersCollPane;
  }
  
  void CalxCoordOtherCtrl::setOffset(motor_point_t offset) {
	this->xOffset->SetValue(offset.x);
	this->yOffset->SetValue(offset.y);
  }
  
  void CalxCoordOtherCtrl::setScale(motor_scale_t scale) {
	this->xScale->SetValue(scale.x);
	this->yScale->SetValue(scale.y);
  }
  
  void CalxCoordOtherCtrl::OnInstrumentEnableClick(wxCommandEvent &evt) {
	this->controller->getHandle()->getController()->getInstrument()->setRunnable(
		this->enableInstrument->GetValue());
  }
  
  void CalxCoordOtherCtrl::OnAdjustPositionClick(wxCommandEvent &evt) {
	motor_point_t offset = this->controller->getHandle()->getPosition();
	motor_scale_t scale = this->controller->getMapFilter()->getScale();
	offset.x += this->controller->getMapFilter()->getOffset().x / scale.x;
	offset.y += this->controller->getMapFilter()->getOffset().y / scale.y;
	filters->setOffset(offset);
  }
  
  void CalxCoordOtherCtrl::OnCalibrateClick(wxCommandEvent &evt) {
	TrailerId tr = this->trailer->GetSelection() == 0 ? TrailerId::Trailer1 : TrailerId::Trailer2;
	this->controller->calibrate(tr);
  }
  
  void CalxCoordOtherCtrl::OnMeasureClick(wxCommandEvent &evt) {
	TrailerId tr = this->trailer->GetSelection() == 0 ? TrailerId::Trailer1 : TrailerId::Trailer2;
	this->controller->measure(tr);
  }
  
  void CalxCoordOtherCtrl::OnFiltersUpdate(wxCommandEvent &evt) {
	controller->getLogFilter()->setLoggingActions(logActions->GetValue());
	controller->getLogFilter()->setLoggingErrors(logErrors->GetValue());
	motor_point_t moffset = { xOffset->GetValue(),
							  yOffset->GetValue() };
	motor_scale_t mscale = { xScale->GetValue(), yScale->GetValue() };
	controller->getMapFilter()->setOffset(moffset);
	controller->getMapFilter()->setScale(mscale);
	motor_point_t min = { minx->GetValue(), miny->GetValue() };
	motor_point_t max = { maxx->GetValue(), maxy->GetValue() };
	controller->getValidateFilter()->setMinimum(min);
	controller->getValidateFilter()->setMaximum(max);
	controller->getValidateFilter()->setMaxSpeed(speed->GetValue());
  }
}
