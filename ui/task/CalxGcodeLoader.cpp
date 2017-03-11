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


#include "CalxGcodeLoader.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>

namespace CalXUI {
	
	CalxGcodeLoader::CalxGcodeLoader(wxWindow *win, wxWindowID id)
		: wxDialog::wxDialog(win, id, "Load GCode") {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		this->state = false;
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Choose GCode file:"));
		wxPanel *gcodePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(gcodePanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *gcodeSizer = new wxBoxSizer(wxHORIZONTAL);
		gcodePanel->SetSizer(gcodeSizer);
		this->gcodePath = new wxTextCtrl(gcodePanel, wxID_ANY, "");
		gcodeSizer->Add(gcodePath, 0, wxEXPAND);
		this->gcodePath->SetMinSize(wxSize(200, -1));
		wxButton *chooseButton = new wxButton(gcodePanel, wxID_ANY, "Choose");
		gcodeSizer->Add(chooseButton, 0, wxALL);
		chooseButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnChooseClick, this);
		
		this->translator = new CalxCoordFilterCtrl(this, wxID_ANY);
		sizer->Add(translator, 0, wxALL | wxEXPAND, 10);
				
		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 1, wxALL | wxALIGN_CENTER, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_ANY, "OK");
		wxButton *cancelButton = new wxButton(buttonPanel, wxID_ANY, "Cancel");
		buttonSizer->Add(okButton, 0, wxALIGN_CENTER);
		buttonSizer->Add(cancelButton, 0, wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnOkClick, this);
		cancelButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnCancelClick, this);
		
		Layout();
		Fit();
		
		wxCommandEvent evt;
		OnChooseClick(evt);
	}
	
	bool CalxGcodeLoader::isLoaded() {
		return this->state;
	}
	
	std::string CalxGcodeLoader::getPath() {
		return this->gcodePath->GetValue().ToStdString();
	}
	
	CoordTranslator *CalxGcodeLoader::getTranslator() {
		return this->translator->getTranslator();
	}
	
	void CalxGcodeLoader::OnOkClick(wxCommandEvent &evt) {
		this->state = true;
		Hide();
	}
	
	void CalxGcodeLoader::OnCancelClick(wxCommandEvent &evt) {
		delete this->translator->getTranslator();
		Hide();
	}
	
	void CalxGcodeLoader::OnChooseClick(wxCommandEvent &evt) {
		wxFileDialog *dialog = new wxFileDialog(this);
		if (dialog->ShowModal() == wxID_OK) {
			this->gcodePath->SetValue(dialog->GetPath());
		}
		dialog->Destroy();
	}
	
}