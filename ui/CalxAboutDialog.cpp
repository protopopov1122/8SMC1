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
#include <fstream>
#include "CalxApp.h"
#include "CalxAboutDialog.h"
#include <wx/sizer.h>

namespace CalXUI {
	
	CalxAboutDialog::CalxAboutDialog(wxWindow *win, wxWindowID id)
		: wxDialog::wxDialog(win, id, "CalX About") {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		std::string about;
		std::ifstream is(wxGetApp().getSystemManager()->getConfiguration()->getEntry("path")->getString("about", "ABOUT"));
		int chr;
		while ((chr = is.get()) != EOF) {
			about.push_back((char) chr);
		}
		
		wxTextCtrl *aboutText = new wxTextCtrl(this, wxID_ANY, about, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
		sizer->Add(aboutText, 1, wxALL | wxEXPAND, 5);
		
		wxButton *okButton = new wxButton(this, wxID_ANY, "Ok");
		sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 5);
		okButton->Bind(wxEVT_BUTTON, &CalxAboutDialog::OnOkClick, this);
	}
	
	void CalxAboutDialog::OnOkClick(wxCommandEvent &evt) {
		Hide();
	}
}