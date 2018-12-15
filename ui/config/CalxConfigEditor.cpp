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
#include "ctrl-lib/conf/INI.h"
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/stattext.h>

namespace CalXUI {

	CalxConfigEventListener::CalxConfigEventListener(CalxConfigEditor *editor) {
		this->editor = editor;
	}

	void CalxConfigEventListener::entryAdd(ConfigurationCatalogue *manager,
	                                       const std::string &id) {
		editor->updateEntries();
		editor->updateEntry();
	}

	void CalxConfigEventListener::entryRemove(ConfigurationCatalogue *manager,
	                                           const std::string &id) {
		editor->updateEntries();
		editor->updateEntry();
	}

	void CalxConfigEventListener::keyAdd(ConfigurationCatalogue *manager,
	                                       const std::string &entry,
										   const std::string &key) {
		editor->updateEntry();
	}

	void CalxConfigEventListener::keyRemove(ConfigurationCatalogue *manager,
	                                         const std::string &entry,
	                                         const std::string &key) {
		editor->updateEntry();
	}

	void CalxConfigEventListener::keyChange(ConfigurationCatalogue *manager,
	                                         const std::string &entry,
	                                         const std::string &key) {
		editor->updateKey();
	}

	CalxConfigEditor::CalxConfigEditor(wxWindow *win, wxWindowID id,
	                                   ConfigurationCatalogue &conf)
	    : CalxPanelPane::CalxPanelPane(win, id), config(conf) {
		this->currentValue = ConfigurationValue();

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		wxSplitterWindow *mainPanel = new wxSplitterWindow(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxEXPAND | wxALL);

		wxPanel *entryPanel = new wxPanel(mainPanel, wxID_ANY);
		wxBoxSizer *entrySizer = new wxBoxSizer(wxVERTICAL);
		entryPanel->SetSizer(entrySizer);
		this->entryList = new wxListBox(entryPanel, wxID_ANY);
		entrySizer->Add(this->entryList, 1, wxBOTTOM | wxALL | wxEXPAND);
		entryList->Bind(wxEVT_LISTBOX, &CalxConfigEditor::OnEntryClick, this);
		wxButton *newEntryButton = new wxButton(entryPanel, wxID_ANY, __("New"));
		entrySizer->Add(newEntryButton, 0, wxEXPAND | wxALL);
		wxButton *removeEntryButton =
		    new wxButton(entryPanel, wxID_ANY, __("Remove"));
		entrySizer->Add(removeEntryButton, 0, wxEXPAND | wxALL);
		wxButton *exportButton =
		    new wxButton(entryPanel, wxID_ANY, __("Export configuration"));
		entrySizer->Add(exportButton, 0, wxEXPAND | wxALL);
		newEntryButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnNewEntryClick,
		                     this);
		removeEntryButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnRemoveEntryClick,
		                        this);
		exportButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnExportClick, this);

		this->valuePanel = new wxPanel(mainPanel, wxID_ANY);
		wxBoxSizer *valueSizer = new wxBoxSizer(wxVERTICAL);
		valuePanel->SetSizer(valueSizer);
		this->valueList = new wxDataViewListCtrl(valuePanel, wxID_ANY);
		valueSizer->Add(this->valueList, 1, wxEXPAND | wxALL);
		this->valueList->AppendTextColumn(__("Name"));
		this->valueList->AppendTextColumn(__("Type"));
		this->valueList->AppendTextColumn(__("Value"));
		this->valueList->Bind(wxEVT_DATAVIEW_SELECTION_CHANGED,
		                      &CalxConfigEditor::OnKeyClick, this);

		mainPanel->Initialize(valuePanel);
		mainPanel->SplitVertically(entryPanel, valuePanel);
		mainPanel->SetSashGravity(0.2f);

		this->editorPanel = new wxPanel(valuePanel, wxID_ANY);
		valueSizer->Add(editorPanel, 0, wxEXPAND | wxALL);
		wxBoxSizer *editorSizer = new wxBoxSizer(wxVERTICAL);
		editorPanel->SetSizer(editorSizer);

		wxPanel *entryCtrlPanel = new wxPanel(this->editorPanel, wxID_ANY);
		editorSizer->Add(entryCtrlPanel);
		wxBoxSizer *entryCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
		entryCtrlPanel->SetSizer(entryCtrlSizer);
		wxButton *newKeyButton = new wxButton(entryCtrlPanel, wxID_ANY, __("New"));
		entryCtrlSizer->Add(newKeyButton);
		newKeyButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnNewKeyClick, this);
		wxButton *removeKeyButton =
		    new wxButton(entryCtrlPanel, wxID_ANY, __("Remove"));
		entryCtrlSizer->Add(removeKeyButton);
		removeKeyButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnRemoveKeyClick,
		                      this);

		this->integerEditor = new wxPanel(editorPanel, wxID_ANY);
		editorSizer->Add(this->integerEditor, 0, wxEXPAND | wxALL);
		wxBoxSizer *integerSizer = new wxBoxSizer(wxHORIZONTAL);
		integerEditor->SetSizer(integerSizer);
		integerSizer->Add(new wxStaticText(integerEditor, wxID_ANY,
		                                   __("Value") + std::string(":")),
		                  0, wxALL | wxALIGN_CENTER, 5);
		this->integerSpin = new wxSpinCtrl(
		    this->integerEditor, wxID_ANY, wxEmptyString, wxDefaultPosition,
		    wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		integerSizer->Add(this->integerSpin, 0, wxALIGN_CENTER);
		this->integerSpin->Bind(wxEVT_SPINCTRL, &CalxConfigEditor::OnIntegerEdit,
		                        this);
		this->integerSpin->Bind(wxEVT_TEXT, &CalxConfigEditor::OnIntegerEdit, this);

		this->realEditor = new wxPanel(editorPanel, wxID_ANY);
		editorSizer->Add(this->realEditor, 0, wxEXPAND | wxALL);
		wxBoxSizer *realSizer = new wxBoxSizer(wxHORIZONTAL);
		realEditor->SetSizer(realSizer);
		realSizer->Add(
		    new wxStaticText(realEditor, wxID_ANY, __("Value") + std::string(": ")),
		    0, wxALL | wxALIGN_CENTER, 5);
		this->realCtrl = new wxTextCtrl(realEditor, wxID_ANY, "0");
		realSizer->Add(this->realCtrl, 0, wxALIGN_CENTER);
		this->realCtrl->Bind(wxEVT_TEXT, &CalxConfigEditor::OnRealEdit, this);

		this->booleanEditor = new wxPanel(editorPanel, wxID_ANY);
		editorSizer->Add(this->booleanEditor, 0, wxEXPAND | wxALL);
		wxBoxSizer *booleanSizer = new wxBoxSizer(wxHORIZONTAL);
		booleanEditor->SetSizer(booleanSizer);
		booleanSizer->Add(new wxStaticText(booleanEditor, wxID_ANY,
		                                   __("Value") + std::string(": ")),
		                  0, wxALL | wxALIGN_CENTER, 5);
		this->booleanCheckbox = new wxCheckBox(booleanEditor, wxID_ANY, "");
		booleanSizer->Add(this->booleanCheckbox, 0, wxALIGN_CENTER);
		this->booleanCheckbox->Bind(wxEVT_CHECKBOX,
		                            &CalxConfigEditor::OnBooleanEdit, this);

		this->stringEditor = new wxPanel(editorPanel, wxID_ANY);
		editorSizer->Add(this->stringEditor, 0, wxEXPAND | wxALL);
		wxBoxSizer *stringSizer = new wxBoxSizer(wxHORIZONTAL);
		stringEditor->SetSizer(stringSizer);
		stringSizer->Add(new wxStaticText(stringEditor, wxID_ANY,
		                                  __("Value") + std::string(": ")),
		                 0, wxALL | wxALIGN_CENTER, 5);
		this->stringCtrl = new wxTextCtrl(stringEditor, wxID_ANY, "");
		stringSizer->Add(this->stringCtrl, 0, wxALIGN_CENTER);
		this->stringCtrl->Bind(wxEVT_TEXT, &CalxConfigEditor::OnStringEdit, this);

		this->listener = std::make_shared<CalxConfigEventListener>(this);
		this->config.addEventListener(this->listener);

		updateEditor();
		updateEntries();
		updateEntry();
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxConfigEditor::OnExit, this);
	}

	bool CalxConfigEditor::isBusy() {
		return false;
	}

	void CalxConfigEditor::updateUI() {}

	void CalxConfigEditor::shutdown() {}

	wxButton *CalxConfigEditor::getOkButton() {
		return this->okButton;
	}

	ConfigurationCatalogue &CalxConfigEditor::getConfiguration() {
		return this->config;
	}

	void CalxConfigEditor::OnNewEntryClick(wxCommandEvent &evt) {
		CalxNewEntryDialog *dialog =
		    new CalxNewEntryDialog(this, wxID_ANY, this->config);
		dialog->ShowModal();
		delete dialog;
	}

	void CalxConfigEditor::OnRemoveEntryClick(wxCommandEvent &evt) {
		if (this->entryList->GetSelection() != wxNOT_FOUND) {
			this->integerEditor->Hide();
			this->realEditor->Hide();
			this->booleanEditor->Hide();
			this->stringEditor->Hide();
			this->editorPanel->Layout();
			this->valuePanel->Layout();
			this->currentValue = ConfigurationValue();
			std::string name = this->entryList->GetStringSelection().ToStdString();
			this->config.removeEntry(name);
		} else {
			wxMessageBox(__("Select entry to remove"), __("Warning"), wxICON_WARNING);
		}
	}

	void CalxConfigEditor::OnEntryClick(wxCommandEvent &evt) {
		updateEntry();
		this->integerEditor->Show(false);
		this->realEditor->Show(false);
		this->booleanEditor->Show(false);
		this->stringEditor->Show(false);
		this->editorPanel->Layout();
		this->valuePanel->Layout();
		this->currentValue = ConfigurationValue();
	}

	void CalxConfigEditor::OnKeyClick(wxDataViewEvent &evt) {
		if (this->entryList->GetSelection() == wxNOT_FOUND ||
		    this->valueList->GetSelectedRow() == wxNOT_FOUND) {
			this->currentValue = ConfigurationValue();
			updateEditor();
			return;
		}
		std::string entry = this->entryList->GetStringSelection().ToStdString();
		wxVariant vrt;
		this->valueList->GetValue(
		    vrt, (unsigned int) this->valueList->GetSelectedRow(), 0);
		std::string key = vrt.GetString().ToStdString();
		this->currentValue = this->config.getEntry(entry)->get(key);
		updateEditor();
	}

	void CalxConfigEditor::OnIntegerEdit(wxCommandEvent &evt) {
		int_conf_t value = this->integerSpin->GetValue();
		if (!this->currentValue.is(ConfigurationValueType::None) &&
		    this->currentValue.getType() == ConfigurationValueType::Integer) {
			std::string entry = this->entryList->GetStringSelection().ToStdString();
			wxVariant vrt;
			this->valueList->GetValue(
			    vrt, (unsigned int) this->valueList->GetSelectedRow(), 0);
			std::string key = vrt.GetString().ToStdString();
			this->currentValue = ConfigurationValue(value);
			this->config.getEntry(entry)->put(key, currentValue);
		}
	}

	void CalxConfigEditor::OnRealEdit(wxCommandEvent &evt) {
		wxString raw = this->realCtrl->GetValue();
		double value;
		if (!raw.ToDouble(&value)) {
			wxMessageBox(__("Enter valid real value"), __("Error"), wxICON_ERROR);
			wxVariant vrt;
			this->valueList->GetValue(
			    vrt, (unsigned int) this->valueList->GetSelectedRow(), 2);
			this->realCtrl->SetValue(vrt.GetString());
			return;
		}
		if (!this->currentValue.is(ConfigurationValueType::None) &&
		    this->currentValue.getType() == ConfigurationValueType::Real) {
			std::string entry = this->entryList->GetStringSelection().ToStdString();
			wxVariant vrt;
			this->valueList->GetValue(
			    vrt, (unsigned int) this->valueList->GetSelectedRow(), 0);
			std::string key = vrt.GetString().ToStdString();
			this->currentValue = ConfigurationValue(value);
			this->config.getEntry(entry)->put(key, currentValue);
		}
	}

	void CalxConfigEditor::OnBooleanEdit(wxCommandEvent &evt) {
		bool value = this->booleanCheckbox->GetValue();
		if (!this->currentValue.is(ConfigurationValueType::None) &&
		    this->currentValue.getType() == ConfigurationValueType::Boolean) {
			std::string entry = this->entryList->GetStringSelection().ToStdString();
			wxVariant vrt;
			this->valueList->GetValue(
			    vrt, (unsigned int) this->valueList->GetSelectedRow(), 0);
			std::string key = vrt.GetString().ToStdString();
			this->currentValue = ConfigurationValue(value);
			this->config.getEntry(entry)->put(key, currentValue);
		}
	}

	void CalxConfigEditor::OnStringEdit(wxCommandEvent &evt) {
		std::string value = this->stringCtrl->GetValue().ToStdString();
		if (!this->currentValue.is(ConfigurationValueType::None) &&
		    this->currentValue.getType() == ConfigurationValueType::String) {
			std::string entry = this->entryList->GetStringSelection().ToStdString();
			wxVariant vrt;
			this->valueList->GetValue(
			    vrt, (unsigned int) this->valueList->GetSelectedRow(), 0);
			std::string key = vrt.GetString().ToStdString();
			this->currentValue = ConfigurationValue(value);
			this->config.getEntry(entry)->put(key, currentValue);
		}
	}

	void CalxConfigEditor::OnNewKeyClick(wxCommandEvent &evt) {
		if (this->entryList->GetSelection() != wxNOT_FOUND) {
			ConfiguationFlatDictionary *entry = this->config.getEntry(
			    this->entryList->GetStringSelection().ToStdString());
			CalxNewKeyDialog *dialog = new CalxNewKeyDialog(this, wxID_ANY, entry);
			dialog->ShowModal();
			delete dialog;
		} else {
			wxMessageBox(__("Select one entry to add key"), __("Attention"),
			             wxICON_INFORMATION);
		}
	}

	void CalxConfigEditor::OnRemoveKeyClick(wxCommandEvent &evt) {
		if (this->entryList->GetSelection() != wxNOT_FOUND &&
		    this->valueList->GetSelectedRow() != wxNOT_FOUND) {
			this->integerEditor->Hide();
			this->realEditor->Hide();
			this->booleanEditor->Hide();
			this->stringEditor->Hide();
			this->editorPanel->Layout();
			this->valuePanel->Layout();
			this->currentValue = ConfigurationValue();
			ConfiguationFlatDictionary *entry = this->config.getEntry(
			    this->entryList->GetStringSelection().ToStdString());
			wxVariant vrt;
			this->valueList->GetValue(
			    vrt, (unsigned int) this->valueList->GetSelectedRow(), 0);
			std::string key = vrt.GetString().ToStdString();
			entry->remove(key);
		} else {
			wxMessageBox(__("Select key to remove"), __("Warning"), wxICON_WARNING);
		}
	}

	void CalxConfigEditor::OnExit(wxCloseEvent &evt) {
		this->config.removeEventListener(this->listener);
		Destroy();
	}

	void CalxConfigEditor::updateEntries() {
		this->entryList->Clear();
		bool hasEntries = false;
		config.visit([&](const std::string &entryName, ConfiguationFlatDictionary &entry) {
			this->entryList->Append(entryName);
			hasEntries = true;
		});
		Layout();
		this->entryList->SetSelection(hasEntries ? 0 : wxNOT_FOUND);
	}

	void CalxConfigEditor::updateEntry() {
		this->valueList->DeleteAllItems();
		if (this->entryList->GetSelection() == wxNOT_FOUND) {
			return;
		}

		std::string entryName = this->entryList->GetStringSelection().ToStdString();
		ConfiguationFlatDictionary *entry = this->config.getEntry(entryName);
		std::vector<std::pair<std::string, ConfigurationValue>> content;
		entry->visit([&](const std::string &key, const ConfigurationValue &value) {
			content.push_back(std::make_pair(key, value));
		});
		for (const auto &kv : content) {
			wxVector<wxVariant> data;
			data.push_back(wxVariant(kv.first));
			const ConfigurationValue &value = kv.second;
			switch (value.getType()) {
				case ConfigurationValueType::Integer:
					data.push_back(wxVariant(__("integer")));
					data.push_back(wxVariant(value.toString()));
					break;
				case ConfigurationValueType::Real:
					data.push_back(wxVariant(__("real")));
					data.push_back(wxVariant(value.toString()));
					break;
				case ConfigurationValueType::Boolean:
					data.push_back(wxVariant(__("boolean")));
					data.push_back(wxVariant(value.toString()));
					break;
				case ConfigurationValueType::String:
					data.push_back(wxVariant(__("string")));
					data.push_back(
					    wxVariant(value.toString()));
					break;
				case ConfigurationValueType::None:
					break;
			}
			this->valueList->AppendItem(data);
		}
	}

	void CalxConfigEditor::updateEditor() {
		this->integerEditor->Hide();
		this->realEditor->Hide();
		this->booleanEditor->Hide();
		this->stringEditor->Hide();
		if (this->currentValue.is(ConfigurationValueType::None)) {
			this->editorPanel->Layout();
			this->valuePanel->Layout();
			return;
		}
		switch (this->currentValue.getType()) {
			case ConfigurationValueType::Integer:
				this->integerEditor->Show(true);
				this->integerSpin->SetValue(
				    (int) this->currentValue.getInt());
				break;
			case ConfigurationValueType::Real:
				this->realEditor->Show(true);
				this->realCtrl->SetValue(std::to_string(this->currentValue.getReal()));
				break;
			case ConfigurationValueType::Boolean:
				this->booleanEditor->Show(true);
				this->booleanCheckbox->SetValue(this->currentValue.getBool());
				break;
			case ConfigurationValueType::String:
				this->stringEditor->Show(true);
				this->stringCtrl->SetValue(this->currentValue.getString());
				break;
			case ConfigurationValueType::None:
				break;
		}
		this->editorPanel->Layout();
		this->valuePanel->Layout();
	}

	void CalxConfigEditor::updateKey() {
		if (this->entryList->GetSelection() != wxNOT_FOUND &&
		    this->valueList->GetSelectedRow() != wxNOT_FOUND) {
			ConfiguationFlatDictionary *entry = this->config.getEntry(
			    this->entryList->GetStringSelection().ToStdString());
			wxVariant vrt;
			this->valueList->GetValue(
			    vrt, (unsigned int) this->valueList->GetSelectedRow(), 0);
			std::string key = vrt.GetString().ToStdString();
			const ConfigurationValue &value = entry->get(key);
			if (value.is(ConfigurationValueType::None)) {
				return;
			}
			switch (value.getType()) {
				case ConfigurationValueType::Integer:
					this->valueList->SetValue(
					    wxVariant(__("integer")),
					    (unsigned int) this->valueList->GetSelectedRow(), 1);
					this->valueList->SetValue(
					    wxVariant(value.toString()),
					    (unsigned int) this->valueList->GetSelectedRow(), 2);
					break;
				case ConfigurationValueType::Real:
					this->valueList->SetValue(
					    wxVariant(__("real")),
					    (unsigned int) this->valueList->GetSelectedRow(), 1);
					this->valueList->SetValue(
					    wxVariant(value.toString()),
					    (unsigned int) this->valueList->GetSelectedRow(), 2);
					break;
				case ConfigurationValueType::Boolean:
					this->valueList->SetValue(
					    wxVariant(__("boolean")),
					    (unsigned int) this->valueList->GetSelectedRow(), 1);
					this->valueList->SetValue(
					    wxVariant(value.toString()),
					    (unsigned int) this->valueList->GetSelectedRow(), 2);
					break;
				case ConfigurationValueType::String:
					this->valueList->SetValue(
					    wxVariant(__("string")),
					    (unsigned int) this->valueList->GetSelectedRow(), 1);
					this->valueList->SetValue(
					    wxVariant(value.toString()),
					    (unsigned int) this->valueList->GetSelectedRow(), 2);
					break;
				case ConfigurationValueType::None:
					break;
			}
		}
	}

	void CalxConfigEditor::OnExportClick(wxCommandEvent &evt) {
		wxFileDialog *dialog =
		    new wxFileDialog(this, __("Export linearized GCode"), "", "", "",
		                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (dialog->ShowModal() == wxID_OK) {
			std::string path = dialog->GetPath().ToStdString();
			std::ofstream out(path);
			INIConfiguration::store(this->config, out);
			out.close();
		}
		dialog->Destroy();
	}
}  // namespace CalXUI
