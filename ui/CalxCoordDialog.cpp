#include "CalxCoordDialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalX {
	
	CalxCoordDialog::CalxCoordDialog(wxWindow* win, wxWindowID id, SystemManager *sysman)
		: wxDialog::wxDialog(win, id, "New coordinate plane") {
		this->sysman = sysman;
		this->ctrl = nullptr;
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		
		wxPanel *xPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *xSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *xText = new wxStaticText(xPanel, wxID_ANY, "X Axis: ");
		this->xChoice = new wxChoice(xPanel, wxID_ANY);
		xSizer->Add(xText, 0, wxALL | wxALIGN_CENTER, 5);
		xSizer->Add(xChoice, 0, wxALL | wxEXPAND);
		xPanel->SetSizer(xSizer);
		sizer->Add(xPanel, 0, wxALL | wxEXPAND);
		
		wxPanel *yPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *ySizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *yText = new wxStaticText(yPanel, wxID_ANY, "Y Axis: ");
		this->yChoice = new wxChoice(yPanel, wxID_ANY);
		ySizer->Add(yText, 0, wxALL | wxALIGN_CENTER, 5);
		ySizer->Add(yChoice, 0, wxALL | wxEXPAND);
		yPanel->SetSizer(ySizer);
		sizer->Add(yPanel, 0, wxALL | wxEXPAND);
		
		wxPanel *ctrlPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *ctrlSizer = new wxBoxSizer(wxHORIZONTAL);
		wxButton *okButton = new wxButton(ctrlPanel, wxID_ANY, "OK");
		ctrlSizer->Add(okButton, 0, wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxCoordDialog::OnOkButtonClick, this);
		wxButton *cancelButton = new wxButton(ctrlPanel, wxID_ANY, "Cancel");
		ctrlSizer->Add(cancelButton, 0, wxALIGN_CENTER);
		cancelButton->Bind(wxEVT_BUTTON, &CalxCoordDialog::OnCancelButtonClick, this);
		ctrlPanel->SetSizer(ctrlSizer);
		sizer->Add(ctrlPanel, 0, wxALL | wxEXPAND, 10);
		
		
		for (size_t i = 0; i < sysman->getDeviceCount(); i++) {
			std::string id = "Device #" + std::to_string(i);
			xChoice->Append(id);
			yChoice->Append(id);
		}
		
		if (sysman->getDeviceCount() >= 2) {
			xChoice->SetSelection(0);
			yChoice->SetSelection(1);
		}
		
		SetSizer(sizer);
		Fit();
	}
	
	CoordHandle *CalxCoordDialog::getCoordPlane() {
		return this->ctrl;
	}
	
	void CalxCoordDialog::OnOkButtonClick(wxCommandEvent &evt) {
		int x = xChoice->GetSelection();
		int y = yChoice->GetSelection();
		if (x != wxNOT_FOUND &&
			y != wxNOT_FOUND) {
			this->ctrl = sysman->createCoord(x, y);
		}
		Hide();
	}
	
	void CalxCoordDialog::OnCancelButtonClick(wxCommandEvent &evt) {
		Hide();
	}
}