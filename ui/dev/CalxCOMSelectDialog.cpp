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

#include <iostream>
#include "CalxApp.h"
#include "CalxCOMSelectDialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalXUI {
	
	CalxCOMSelectDialog::CalxCOMSelectDialog(wxWindow *win, wxWindowID id)
		: wxDialog::wxDialog(win, id, "Select COM Port") {
		
		this->port = -1;
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 0, wxALL, 10);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
		mainPanel->SetSizer(mainSizer);
		mainSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "COM Port(1-255): "), 0, wxRIGHT, 5);
		this->portSpin = new wxSpinCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 255, wxGetApp().getSystemManager()->getConfiguration()->getEntry("ui")->getInt("default_com", 1));
		mainSizer->Add(this->portSpin);
		
		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALL | wxALIGN_CENTER, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_ANY, "Ok");
		wxButton *cancelButton = new wxButton(buttonPanel, wxID_ANY, "Cancel");
		buttonSizer->Add(okButton);
		buttonSizer->Add(cancelButton);
		okButton->Bind(wxEVT_BUTTON, &CalxCOMSelectDialog::OnOkClick, this);
		cancelButton->Bind(wxEVT_BUTTON, &CalxCOMSelectDialog::OnCancelClick, this);
		
		Layout();
		Fit();
	}
	
	int CalxCOMSelectDialog::getPort() {
		return this->port;
	}
	
	void CalxCOMSelectDialog::OnOkClick(wxCommandEvent &evt) {
		this->port = this->portSpin->GetValue();
		Hide();
	}
	
	void CalxCOMSelectDialog::OnCancelClick(wxCommandEvent &evt) {
		Hide();
	}
}