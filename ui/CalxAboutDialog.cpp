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

#include "CalxAboutDialog.h"
#include <wx/sizer.h>

namespace CalXUI {

	std::string ABOUT_TEXT = "CalX Project\n"
								"CalX is stepper motor controller library and user interface. "
								"CalX supports plugins to control different types of devices and instruments "
								"using unitified library and interface. System offers different API "
								"to operate with motors, allows graph building and limited g-code execution.\n"
								"Current software state is unstable, user interface and library is under development.\n"
								"Author: Jevgenijs Protopopovs.\nProject repo: <https://github.com/protopopov1122/CalX>\n\nLicense:\n"
								"CalX is free software: you can redistribute it and/or modify "
								"it under the terms of the GNU Lesser General Public License as published by "
								"the Free Software Foundation, either version 3 of the License, or "
								"(at your option) any later version.\n\n"
								"CalX is distributed in the hope that it will be useful, "
								"but WITHOUT ANY WARRANTY; without even the implied warranty of "
								"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
								"GNU Lesser General Public License for more details.\n\n"
								"You should have received a copy of the GNU Lesser General Public License "
								"along with CalX.  If not, see <http://www.gnu.org/licenses/>.";
	
	CalxAboutDialog::CalxAboutDialog(wxWindow *win, wxWindowID id)
		: wxDialog::wxDialog(win, id, "CalX About") {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		wxTextCtrl *aboutText = new wxTextCtrl(this, wxID_ANY, ABOUT_TEXT, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
		sizer->Add(aboutText, 1, wxALL | wxEXPAND, 5);
		
		wxButton *okButton = new wxButton(this, wxID_ANY, "Ok");
		sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 5);
		okButton->Bind(wxEVT_BUTTON, &CalxAboutDialog::OnOkClick, this);
	}
	
	void CalxAboutDialog::OnOkClick(wxCommandEvent &evt) {
		Hide();
	}
}