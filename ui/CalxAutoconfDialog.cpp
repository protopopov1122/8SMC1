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


#include "CalxAutoconfDialog.h"

namespace CalXUI {
	
	CalxAutoconfDialog::CalxAutoconfDialog(wxWindow *win, wxWindowID id)
		: wxDialog::wxDialog(win, id, __("Please wait"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE) {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		this->text = new wxStaticText(this, wxID_ANY, __("Please wait until initialization finishes"));
		sizer->Add(text, 0, wxALL | wxALIGN_CENTER, 100);
		
		Layout();
		Fit();
		Bind(wxEVT_CLOSE_WINDOW, &CalxAutoconfDialog::OnExit, this);
		
		wxSize parentSize = GetParent()->GetSize();
		wxSize size = GetSize();
		SetPosition(wxPoint((parentSize.x - size.x) / 2, (parentSize.y - size.y) / 2));
		GetParent()->Enable(false);
	}
	
	void CalxAutoconfDialog::resolving(std::string code, size_t step, size_t full) {
		std::string txt = FORMAT(__("Please wait until initialization finishes\nPerforming step %s of %s:\n%s"),
			std::to_string(step), std::to_string(full), code);
		this->text->SetLabel(txt);
	}
	
	void CalxAutoconfDialog::failed(std::string code, size_t step, size_t full) {
		std::string txt = FORMAT(__("Initialization stopped\nStep %s of %s failed:\n%s"),
			std::to_string(step), std::to_string(full), code);
		this->text->SetLabel(txt);
		wxMessageBox(txt, "Initialization error", wxICON_ERROR);
	}
	
	void CalxAutoconfDialog::OnExit(wxCloseEvent &evt) {
		GetParent()->Enable(true);
		Destroy();
	}
}