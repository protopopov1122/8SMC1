/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ui/config/CalxConfigEditor.h"

namespace CalXUI {

	CalxNewKeyDialog::CalxNewKeyDialog(wxWindow *win, wxWindowID id,
	                                   std::shared_ptr<ConfigEntry> entry)
	    : wxDialog::wxDialog(win, id, __("Add new key")) {
		this->entry = entry;

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *mainSizer = new wxFlexGridSizer(2);
		mainPanel->SetSizer(mainSizer);

		mainSizer->Add(
		    new wxStaticText(mainPanel, wxID_ANY, __("Name") + std::string(":")), 0,
		    wxALIGN_RIGHT | wxRIGHT, 5);
		this->key = new wxTextCtrl(mainPanel, wxID_ANY, "");
		mainSizer->Add(key);

		mainSizer->Add(
		    new wxStaticText(mainPanel, wxID_ANY, __("Type") + std::string(":")), 0,
		    wxALIGN_RIGHT | wxRIGHT, 5);
		this->type = new wxComboBox(mainPanel, wxID_ANY);
		this->type->Append(__("Integer"));
		this->type->Append(__("Real"));
		this->type->Append(__("Boolean"));
		this->type->Append(__("String"));
		this->type->SetSelection(0);
		mainSizer->Add(this->type);
		this->type->Bind(wxEVT_COMBOBOX, &CalxNewKeyDialog::OnTypeChange, this);

		mainSizer->Add(
		    new wxStaticText(mainPanel, wxID_ANY, __("Value") + std::string(":")),
		    0, wxALIGN_RIGHT | wxRIGHT, 5);
		this->editorPanel = new wxPanel(mainPanel, wxID_ANY);
		mainSizer->Add(this->editorPanel);
		wxBoxSizer *editorSizer = new wxBoxSizer(wxVERTICAL);
		editorPanel->SetSizer(editorSizer);

		this->integer = new wxSpinCtrl(this->editorPanel, wxID_ANY, wxEmptyString,
		                               wxDefaultPosition, wxDefaultSize,
		                               wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		editorSizer->Add(this->integer);
		this->real = new wxTextCtrl(this->editorPanel, wxID_ANY, "0");
		editorSizer->Add(this->real);
		this->boolean = new wxCheckBox(this->editorPanel, wxID_ANY, "");
		editorSizer->Add(this->boolean);
		this->string = new wxTextCtrl(this->editorPanel, wxID_ANY, "");
		editorSizer->Add(this->string);

		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALIGN_CENTER | wxALL, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_OK, __("OK"));
		wxButton *cancelButton =
		    new wxButton(buttonPanel, wxID_CANCEL, __("Cancel"));
		buttonSizer->Add(okButton);
		buttonSizer->Add(cancelButton);
		okButton->Bind(wxEVT_BUTTON, &CalxNewKeyDialog::OnOkClick, this);
		cancelButton->Bind(wxEVT_BUTTON, &CalxNewKeyDialog::OnCancelClick, this);

		updateUI();
		Layout();
		Fit();
	}

	void CalxNewKeyDialog::updateUI() {
		this->integer->Show(false);
		this->real->Show(false);
		this->boolean->Show(false);
		this->string->Show(false);
		switch (this->type->GetSelection()) {
			case 0:
				this->integer->Show(true);
				break;
			case 1:
				this->real->Show(true);
				break;
			case 2:
				this->boolean->Show(true);
				break;
			case 3:
				this->string->Show(true);
				break;
		}
		this->editorPanel->Layout();
		Layout();
	}

	void CalxNewKeyDialog::OnOkClick(wxCommandEvent &evt) {
		std::string key = this->key->GetValue().ToStdString();
		if (key.empty()) {
			wxMessageBox(__("Key is empty"), __("Warning"), wxICON_WARNING);
			return;
		}
		if (entry->has(key)) {
			wxMessageBox(FORMAT(__("Key '%s' already exists"), key.c_str()),
			             __("Warning"), wxICON_WARNING);
			return;
		}
		switch (this->type->GetSelection()) {
			case 0: {
				int_conf_t value = this->integer->GetValue();
				this->entry->put(key, ConfigurationValue(value));
			} break;
			case 1: {
				wxString raw = this->real->GetValue();
				double val;
				if (!raw.ToDouble(&val)) {
					wxMessageBox(__("Enter valid real value"), __("Error"), wxICON_ERROR);
					return;
				}
				this->entry->put(key, ConfigurationValue(val));
			} break;
			case 2: {
				bool value = this->boolean->GetValue();
				this->entry->put(key, ConfigurationValue(value));
			} break;
			case 3: {
				std::string value = this->string->GetValue().ToStdString();
				this->entry->put(key, ConfigurationValue(value));
			} break;
		}
		Hide();
	}

	void CalxNewKeyDialog::OnCancelClick(wxCommandEvent &evt) {
		Hide();
	}

	void CalxNewKeyDialog::OnTypeChange(wxCommandEvent &evt) {
		updateUI();
	}

	CalxNewEntryDialog::CalxNewEntryDialog(wxWindow *win, wxWindowID id,
	                                       ConfigManager &config)
	    : wxDialog::wxDialog(win, id, __("Add new entry")), config(config) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxEXPAND | wxALL, 10);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
		mainPanel->SetSizer(mainSizer);
		mainSizer->Add(new wxStaticText(mainPanel, wxID_ANY,
		                                __("Entry name") + std::string(":")),
		               0, wxALL, 5);
		this->entryName = new wxTextCtrl(mainPanel, wxID_ANY, "");
		mainSizer->Add(this->entryName);

		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALL, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_OK, __("OK"));
		wxButton *cancelButton =
		    new wxButton(buttonPanel, wxID_CANCEL, __("Cancel"));
		buttonSizer->Add(okButton);
		buttonSizer->Add(cancelButton);
		okButton->Bind(wxEVT_BUTTON, &CalxNewEntryDialog::OnOkClick, this);
		cancelButton->Bind(wxEVT_BUTTON, &CalxNewEntryDialog::OnCancelClick, this);

		Layout();
		Fit();
	}

	void CalxNewEntryDialog::OnOkClick(wxCommandEvent &evt) {
		std::string name = this->entryName->GetValue().ToStdString();
		if (name.empty()) {
			wxMessageBox(__("Entry name is empty"), __("Warning"), wxICON_WARNING);
			return;
		}
		if (this->config.hasEntry(name)) {
			wxMessageBox(FORMAT(__("Entry '%s' already exists"), name.c_str()),
			             __("Error"), wxICON_ERROR);
			return;
		}
		this->config.getEntry(name);
		Hide();
	}

	void CalxNewEntryDialog::OnCancelClick(wxCommandEvent &evt) {
		Hide();
	}

	CalxConfigDialog::CalxConfigDialog(wxWindow *win, wxWindowID id,
	                                   ConfigManager &conf)
	    : wxDialog::wxDialog(win, id, __("Configuration Editor"),
	                         wxDefaultPosition, wxDefaultSize,
	                         wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		this->editor = new CalxConfigEditor(this, wxID_ANY, conf);
		sizer->Add(this->editor, 1, wxEXPAND | wxALL, 5);

		wxButton *okButton = new wxButton(this, wxID_OK, __("OK"));
		sizer->Add(okButton, 0, wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxConfigDialog::OnOkClick, this);

		Layout();
		Fit();
	}

	ConfigManager &CalxConfigDialog::getConfiguration() {
		return this->editor->getConfiguration();
	}

	void CalxConfigDialog::OnOkClick(wxCommandEvent &evt) {
		this->editor->Close(true);
		Hide();
	}
}  // namespace CalXUI