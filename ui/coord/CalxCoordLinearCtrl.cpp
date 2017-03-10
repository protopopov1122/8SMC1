#include "CalxCoordCtrl.h"

namespace CalXUI {
	
	void CalxCoordLinearCtrl::init() {
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->speed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 4000, 4000);
		this->divisor = new wxChoice(this, wxID_ANY);
		this->divisor->Append("1");
		this->divisor->Append("2");
		this->divisor->Append("4");
		this->divisor->Append("8");
		this->divisor->SetSelection(3);
		this->relative = new wxCheckBox(this, wxID_ANY, "Relative");
		wxButton *moveButton = new wxButton(this, wxID_ANY, "Move");
		wxButton *jumpButton = new wxButton(this, wxID_ANY, "Jump");
		moveButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnLinearMoveClick, ctrl);
		jumpButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnLinearJumpClick, ctrl);
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Destination:"));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(xCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(yCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Speed:"));
		sizer->Add(speed, 0, wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Divisor:"));
		sizer->Add(divisor);
		sizer->Add(relative, 0, wxALIGN_CENTER);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(moveButton);
		sizer->Add(jumpButton);
	}
}