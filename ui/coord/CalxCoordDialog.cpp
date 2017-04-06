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


#include "CalxCoordDialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalXUI {
	
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
		
		wxPanel *instrPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *instrSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *instrText = new wxStaticText(instrPanel, wxID_ANY, "Instrument: ");
		this->instrChoice = new wxChoice(instrPanel, wxID_ANY);
		instrSizer->Add(instrText, 0, wxALL | wxALIGN_CENTER, 5);
		instrSizer->Add(instrChoice, 0, wxALL | wxEXPAND);
		instrPanel->SetSizer(instrSizer);
		sizer->Add(instrPanel, 0, wxALL | wxEXPAND);
		
		wxPanel *ctrlPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *ctrlSizer = new wxBoxSizer(wxHORIZONTAL);
		wxButton *okButton = new wxButton(ctrlPanel, wxID_OK, "OK");
		ctrlSizer->Add(okButton, 0, wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxCoordDialog::OnOkButtonClick, this);
		wxButton *cancelButton = new wxButton(ctrlPanel, wxID_CANCEL, "Cancel");
		ctrlSizer->Add(cancelButton, 0, wxALIGN_CENTER);
		cancelButton->Bind(wxEVT_BUTTON, &CalxCoordDialog::OnCancelButtonClick, this);
		ctrlPanel->SetSizer(ctrlSizer);
		sizer->Add(ctrlPanel, 0, wxALL | wxEXPAND, 10);
		
		
		for (size_t i = 0; i < sysman->getMotorCount(); i++) {
			std::string id = "Device #" + std::to_string(i);
			xChoice->Append(id);
			yChoice->Append(id);
		}
		
		instrChoice->Append("No instrument");
		
		for (size_t i = 0; i < sysman->getInstrumentCount(); i++) {
			std::string id = "Instrument #" + std::to_string(i);
			instrChoice->Append(id);
		}
		
		if (sysman->getMotorCount() >= 2) {
			xChoice->SetSelection(0);
			yChoice->SetSelection(1);
		}
		
		if (sysman->getInstrumentCount() > 0) {
			instrChoice->SetSelection(1);
		} else {
			instrChoice->SetSelection(0);
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
		int i = instrChoice->GetSelection();
		if (x != wxNOT_FOUND &&
			y != wxNOT_FOUND) {
			if (x == y) {
				wxMessageBox("Devices cannot be same", "Warning", wxOK | wxICON_WARNING);
				return;
			}
			this->ctrl = sysman->createCoord(x, y, i == wxNOT_FOUND || i == 0 ? -1 : (i - 1));
		}
		Hide();
	}
	
	void CalxCoordDialog::OnCancelButtonClick(wxCommandEvent &evt) {
		Hide();
	}
}