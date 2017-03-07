#include "CalxCoordCtrl.h"

namespace CalX {
	
	
	void CalxCoordGraphCtrl::init() {
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);
		
		
		wxPanel *graphPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(graphPanel, 0, wxRIGHT, 10);
		wxFlexGridSizer *graphSizer = new wxFlexGridSizer(2);
		graphPanel->SetSizer(graphSizer);
		
		this->expr = new wxTextCtrl(graphPanel, wxID_ANY, "x");
		this->xmin = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, -10);
		this->xmax = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 10);
		this->ymin = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, -10);
		this->ymax = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 10);
		this->step = new wxTextCtrl(graphPanel, wxID_ANY, "1");
		wxButton *buildButton = new wxButton(graphPanel, wxID_ANY, "Build");
		buildButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnGraphBuildClick, ctrl);
		
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "Function "), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(expr, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "X axis range:"));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "min"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "max"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "Y axis range:"));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "min"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "max"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "X axis step"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(step, 0, wxEXPAND);
		graphSizer->Add(buildButton);
		
		wxPanel *coordPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(coordPanel, 0, wxLEFT, 10);
		wxFlexGridSizer *coordSizer = new wxFlexGridSizer(2);
		coordPanel->SetSizer(coordSizer);
		
		this->xoffset = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yoffset = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->xscale = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 1);
		this->yscale = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 1);
		this->speed = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 4000, 4000, 4000);
		
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "X axis offset"), 0, wxRIGHT, 10);
		coordSizer->Add(xoffset, 0, wxEXPAND);
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "Y axis offset"), 0, wxRIGHT, 10);
		coordSizer->Add(yoffset, 0, wxEXPAND);
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "X axis scale"), 0, wxRIGHT, 10);
		coordSizer->Add(xscale, 0, wxEXPAND);
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "Y axis scale"), 0, wxRIGHT, 10);
		coordSizer->Add(yscale, 0, wxEXPAND);
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "Build speed"), 0, wxRIGHT, 10);
		coordSizer->Add(speed, 0, wxEXPAND);
	}
}