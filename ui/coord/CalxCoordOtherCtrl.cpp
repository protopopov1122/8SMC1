#include "CalxCoordCtrl.h"

namespace CalXUI {
	
	
	void CalxCoordOtherCtrl::init() {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		wxPanel *calibratePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(calibratePanel, 0, wxALL, 5);
		wxBoxSizer *calibrateSizer = new wxBoxSizer(wxHORIZONTAL);
		calibratePanel->SetSizer(calibrateSizer);
		wxButton *calibrateButton = new wxButton(calibratePanel, wxID_ANY, "Calibrate");
		calibrateButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnCalibrateClick, ctrl);
		calibrateSizer->Add(calibrateButton);
		calibrateSizer->Add(new wxStaticText(calibratePanel, wxID_ANY, " to "), wxALL | wxALIGN_CENTER, 5);
		trailer = new wxChoice(calibratePanel, wxID_ANY);
		trailer->Append("trailer 1");
		trailer->Append("trailer 2");
		trailer->SetSelection(0);
		calibrateSizer->Add(trailer, 0, wxALL);
		
		wxPanel *logPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(logPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *logSizer = new wxBoxSizer(wxHORIZONTAL);
		logPanel->SetSizer(logSizer);
		this->logActions = new wxCheckBox(logPanel, wxID_ANY, "Log actions");		
		this->logErrors = new wxCheckBox(logPanel, wxID_ANY, "Log errors");
		logSizer->Add(logActions);
		logSizer->Add(logErrors);
		
		wxPanel *mapPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mapPanel, 0, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *mapSizer = new wxFlexGridSizer(2);
		mapPanel->SetSizer(mapSizer);
		this->xOffset = new wxSpinCtrl(mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yOffset = new wxSpinCtrl(mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->xScale = new wxTextCtrl(mapPanel, wxID_ANY, "1");
		this->yScale = new wxTextCtrl(mapPanel, wxID_ANY, "1");
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "Coordinate offset:"));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, ""));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "x:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(xOffset, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "y:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(yOffset, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "Coordinate scale:"));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, ""));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "x:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(xScale, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "y:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(yScale, 0, wxEXPAND);
		
		wxPanel *validatePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(validatePanel, 0, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *validateSizer = new wxFlexGridSizer(2);
		validatePanel->SetSizer(validateSizer);
		this->minx = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->miny = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->maxx = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->maxy = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->speed = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 4000, 4000);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "Coordinate validation:"));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, ""));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "min x:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(minx, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "min y:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(miny, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "max x:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(maxx, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "max y:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(maxy, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "Maximum speed"));
		validateSizer->Add(speed, 0, wxEXPAND);
		
		logActions->SetValue(ctrl->getPlaneLog()->isLoggingActions());
		logErrors->SetValue(ctrl->getPlaneLog()->isLoggingErrors());
		xOffset->SetValue(ctrl->getPlaneMap()->getOffset().x);
		yOffset->SetValue(ctrl->getPlaneMap()->getOffset().y);
		xScale->SetValue(std::to_string((float) ctrl->getPlaneMap()->getScale().x));
		yScale->SetValue(std::to_string((float) ctrl->getPlaneMap()->getScale().y));
		minx->SetValue(ctrl->getPlaneValidator()->getMinimum().x);
		miny->SetValue(ctrl->getPlaneValidator()->getMinimum().y);
		maxx->SetValue(ctrl->getPlaneValidator()->getMaximum().x);
		maxy->SetValue(ctrl->getPlaneValidator()->getMaximum().y);
		speed->SetValue(ctrl->getPlaneValidator()->getMaxSpeed());
		
		wxButton *updateButton = new wxButton(this, wxID_ANY, "Update filters");
		sizer->Add(updateButton);
		updateButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
	}
}