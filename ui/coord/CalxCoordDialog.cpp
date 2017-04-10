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
		: wxDialog::wxDialog(win, id, __("New coordinate plane")) {
		this->sysman = sysman;
		this->ctrl = nullptr;
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxALL | wxEXPAND, 10);
		wxFlexGridSizer *mainSizer = new wxFlexGridSizer(2);
		mainPanel->SetSizer(mainSizer);
		
		wxStaticText *xText = new wxStaticText(mainPanel, wxID_ANY, __("X Axis: "));
		this->xChoice = new wxChoice(mainPanel, wxID_ANY);
		mainSizer->Add(xText, 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mainSizer->Add(xChoice, 0, wxALL | wxEXPAND);
		
		wxStaticText *yText = new wxStaticText(mainPanel, wxID_ANY, __("Y Axis: "));
		this->yChoice = new wxChoice(mainPanel, wxID_ANY);
		mainSizer->Add(yText, 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mainSizer->Add(yChoice, 0, wxALL | wxEXPAND);
		
		wxStaticText *instrText = new wxStaticText(mainPanel, wxID_ANY, __("Instrument: "));
		this->instrChoice = new wxChoice(mainPanel, wxID_ANY);
		mainSizer->Add(instrText, 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mainSizer->Add(instrChoice, 0, wxALL | wxEXPAND);
		
		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALIGN_CENTER | wxALL, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_OK, __("OK"));
		okButton->Bind(wxEVT_BUTTON, &CalxCoordDialog::OnOkButtonClick, this);
		wxButton *cancelButton = new wxButton(buttonPanel, wxID_CANCEL, __("Cancel"));
		cancelButton->Bind(wxEVT_BUTTON, &CalxCoordDialog::OnCancelButtonClick, this);
		buttonSizer->Add(okButton);
		buttonSizer->Add(cancelButton);
		
		
		for (size_t i = 0; i < sysman->getMotorCount(); i++) {
			std::string id = __("Device #") + std::to_string(i);
			xChoice->Append(id);
			yChoice->Append(id);
		}
		
		instrChoice->Append(__("No instrument"));
		
		for (size_t i = 0; i < sysman->getInstrumentCount(); i++) {
			std::string id = __("Instrument #") + std::to_string(i);
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
		
		Layout();
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
				wxMessageBox(__("Devices cannot be same"), __("Warning"), wxOK | wxICON_WARNING);
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