#include "CalxConfigEditor.h"
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/stattext.h>

namespace CalXUI {
	
	CalxNewKeyDialog::CalxNewKeyDialog(wxWindow *win, wxWindowID id, ConfigEntry *entry)
		: wxDialog::wxDialog(win, id, "Add new key") {
			
		this->entry = entry;

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *mainSizer = new wxFlexGridSizer(2);
		mainPanel->SetSizer(mainSizer);
		
		mainSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Name:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		this->key = new wxTextCtrl(mainPanel, wxID_ANY, "");
		mainSizer->Add(key);
		
		mainSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Type:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		this->type = new wxComboBox(mainPanel, wxID_ANY);
		this->type->Append("Integer");
		this->type->Append("Real");
		this->type->Append("Boolean");
		this->type->Append("String");
		this->type->SetSelection(0);
		mainSizer->Add(this->type);
		this->type->Bind(wxEVT_COMBOBOX, &CalxNewKeyDialog::OnTypeChange, this);
		
		mainSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Value:"), 0, wxALIGN_RIGHT | wxRIGHT, 5);
		this->editorPanel = new wxPanel(mainPanel, wxID_ANY);
		mainSizer->Add(this->editorPanel);
		wxBoxSizer *editorSizer = new wxBoxSizer(wxVERTICAL);
		editorPanel->SetSizer(editorSizer);
		
		this->integer = new wxSpinCtrl(this->editorPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
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
		wxButton *okButton = new wxButton(buttonPanel, wxID_OK, "OK");
		wxButton *cancelButton = new wxButton(buttonPanel, wxID_CANCEL, "Cancel");
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
			wxMessageBox("Key is empty", "Warning", wxICON_WARNING);
			return;
		}
		if (entry->has(key)) {
			wxMessageBox("Key '" + key + "' already exists", "Warning", wxICON_WARNING);
			return;
		}
		switch (this->type->GetSelection()) {
			case 0: {
				int_conf_t value = this->integer->GetValue();
				this->entry->put(key, new IntegerConfigValue(value));
			} break;
			case 1: {
				wxString raw = this->real->GetValue();
				double val;
				if (!raw.ToDouble(&val)) {
					wxMessageBox("Enter valid real value", "Error", wxICON_ERROR);
					return;
				}
				this->entry->put(key, new RealConfigValue(val));
			} break;
			case 2: {
				bool value = this->boolean->GetValue();
				this->entry->put(key, new BoolConfigValue(value));
			} break;
			case 3: {
				std::string value = this->string->GetValue().ToStdString();
				this->entry->put(key, new StringConfigValue(value));
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
	
	CalxNewEntryDialog::CalxNewEntryDialog(wxWindow *win, wxWindowID id, ConfigManager *config)
		: wxDialog::wxDialog(win, id, "Add new entry") {
		
		this->config = config;
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxEXPAND | wxALL, 10);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
		mainPanel->SetSizer(mainSizer);
		mainSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Entry name:"), 0, wxALL, 5);
		this->entryName = new wxTextCtrl(mainPanel, wxID_ANY, "");
		mainSizer->Add(this->entryName);
		
		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALL, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_OK, "OK");
		wxButton *cancelButton = new wxButton(buttonPanel, wxID_CANCEL, "Cancel");
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
			wxMessageBox("Entry name is empty", "Warning", wxICON_WARNING);
			return;
		}
		if (this->config->hasEntry(name)) {
			wxMessageBox("Entry '" + name + "' already exists", "Error", wxICON_ERROR);
			return;
		}
		this->config->getEntry(name);
		Hide();
	}
	
	void CalxNewEntryDialog::OnCancelClick(wxCommandEvent &evt) {
		Hide();
	}
	
	CalxConfigEditor::CalxConfigEditor(wxWindow *win, wxWindowID id, ConfigManager *conf)
		: wxScrolledWindow::wxScrolledWindow(win, id) {
		
		this->config = conf;
		this->currentValue = nullptr;
		
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
		wxButton *newEntryButton = new wxButton(entryPanel, wxID_ANY, "New");
		entrySizer->Add(newEntryButton, 0, wxEXPAND | wxALL);
		wxButton *removeEntryButton = new wxButton(entryPanel, wxID_ANY, "Remove");
		entrySizer->Add(removeEntryButton, 0, wxEXPAND | wxALL);
		newEntryButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnNewEntryClick, this);
		removeEntryButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnRemoveEntryClick, this);
		
		this->valuePanel = new wxPanel(mainPanel, wxID_ANY);
		wxBoxSizer *valueSizer = new wxBoxSizer(wxVERTICAL);
		valuePanel->SetSizer(valueSizer);
		this->valueList = new wxDataViewListCtrl (valuePanel, wxID_ANY);
		valueSizer->Add(this->valueList, 1, wxEXPAND | wxALL);
		this->valueList->AppendTextColumn("Name");
		this->valueList->AppendTextColumn("Type");
		this->valueList->AppendTextColumn("Value");
		this->valueList->Bind(wxEVT_DATAVIEW_SELECTION_CHANGED, &CalxConfigEditor::OnKeyClick, this);
		
		mainPanel->Initialize(valuePanel);
		mainPanel->SplitVertically(entryPanel, valuePanel);
		mainPanel->SetSashGravity(0.1f);
		
		this->editorPanel = new wxPanel(valuePanel, wxID_ANY);
		valueSizer->Add(editorPanel, 0, wxEXPAND | wxALL);
		wxBoxSizer *editorSizer = new wxBoxSizer(wxVERTICAL);
		editorPanel->SetSizer(editorSizer);
		
		wxPanel *entryCtrlPanel = new wxPanel(this->editorPanel, wxID_ANY);
		editorSizer->Add(entryCtrlPanel);
		wxBoxSizer *entryCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
		entryCtrlPanel->SetSizer(entryCtrlSizer);
		wxButton *newKeyButton = new wxButton(entryCtrlPanel, wxID_ANY, "New");
		entryCtrlSizer->Add(newKeyButton);
		newKeyButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnNewKeyClick, this);
		wxButton *removeKeyButton = new wxButton(entryCtrlPanel, wxID_ANY, "Remove");
		entryCtrlSizer->Add(removeKeyButton);
		removeKeyButton->Bind(wxEVT_BUTTON, &CalxConfigEditor::OnRemoveKeyClick, this);
		
		this->integerEditor = new wxPanel(editorPanel, wxID_ANY);
		editorSizer->Add(this->integerEditor, 0, wxEXPAND | wxALL);
		wxBoxSizer *integerSizer = new wxBoxSizer(wxHORIZONTAL);
		integerEditor->SetSizer(integerSizer);
		integerSizer->Add(new wxStaticText(integerEditor, wxID_ANY, "Value:"), 0, wxALL | wxALIGN_CENTER, 5);
		this->integerSpin = new wxSpinCtrl(this->integerEditor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		integerSizer->Add(this->integerSpin, 0, wxALIGN_CENTER);
		this->integerSpin->Bind(wxEVT_SPINCTRL, &CalxConfigEditor::OnIntegerEdit, this);
		
		this->realEditor = new wxPanel(editorPanel, wxID_ANY);
		editorSizer->Add(this->realEditor, 0, wxEXPAND | wxALL);
		wxBoxSizer *realSizer = new wxBoxSizer(wxHORIZONTAL);
		realEditor->SetSizer(realSizer);
		realSizer->Add(new wxStaticText(realEditor, wxID_ANY, "Value: "), 0, wxALL | wxALIGN_CENTER, 5);
		this->realCtrl = new wxTextCtrl(realEditor, wxID_ANY, "0");
		realSizer->Add(this->realCtrl, 0, wxALIGN_CENTER);
		this->realCtrl->Bind(wxEVT_TEXT, &CalxConfigEditor::OnRealEdit, this);
			
		this->booleanEditor = new wxPanel(editorPanel, wxID_ANY);
		editorSizer->Add(this->booleanEditor, 0, wxEXPAND | wxALL);
		wxBoxSizer *booleanSizer = new wxBoxSizer(wxHORIZONTAL);
		booleanEditor->SetSizer(booleanSizer);
		booleanSizer->Add(new wxStaticText(booleanEditor, wxID_ANY, "Value: "), 0, wxALL | wxALIGN_CENTER, 5);
		this->booleanCheckbox = new wxCheckBox(booleanEditor, wxID_ANY, "");
		booleanSizer->Add(this->booleanCheckbox, 0, wxALIGN_CENTER);
		this->booleanCheckbox->Bind(wxEVT_CHECKBOX, &CalxConfigEditor::OnBooleanEdit, this);
		
		this->stringEditor = new wxPanel(editorPanel, wxID_ANY);
		editorSizer->Add(this->stringEditor, 0, wxEXPAND | wxALL);
		wxBoxSizer *stringSizer = new wxBoxSizer(wxHORIZONTAL);
		stringEditor->SetSizer(stringSizer);
		stringSizer->Add(new wxStaticText(stringEditor, wxID_ANY, "Value: "), 0, wxALL | wxALIGN_CENTER, 5);
		this->stringCtrl = new wxTextCtrl(stringEditor, wxID_ANY, "");
		stringSizer->Add(this->stringCtrl, 0, wxALIGN_CENTER);
		this->stringCtrl->Bind(wxEVT_TEXT, &CalxConfigEditor::OnStringEdit, this);
		
		updateEditor();
		updateEntries();
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxConfigEditor::OnExit, this);
	}
	
	wxButton *CalxConfigEditor::getOkButton() {
		return this->okButton;
	}
	
	ConfigManager *CalxConfigEditor::getConfiguration() {
		return this->config;
	}
	
	void CalxConfigEditor::OnNewEntryClick(wxCommandEvent &evt) {
		CalxNewEntryDialog *dialog = new CalxNewEntryDialog(this, wxID_ANY, this->config);
		dialog->ShowModal();
		delete dialog;
		updateEntries();
		updateEntry();
	}
	
	void CalxConfigEditor::OnRemoveEntryClick(wxCommandEvent &evt) {
		if (this->entryList->GetSelection() != wxNOT_FOUND) {
			std::string name = this->entryList->GetStringSelection().ToStdString();
			this->config->removeEntry(name);
			updateEntries();
			updateEntry();
		} else {
			wxMessageBox("Select entry to remove", "Warning", wxICON_WARNING);
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
		this->currentValue = nullptr;
	}
	
	void CalxConfigEditor::OnKeyClick(wxDataViewEvent  &evt) {
		if (this->entryList->GetSelection() == wxNOT_FOUND ||
			this->valueList->GetSelectedRow() == wxNOT_FOUND) {
			this->currentValue = nullptr;
			updateEditor();
			return;	
		}
		std::string entry = this->entryList->GetStringSelection().ToStdString();
		wxVariant vrt;
		this->valueList->GetValue(vrt, this->valueList->GetSelectedRow(), 0);
		std::string key = vrt.GetString().ToStdString();
		this->currentValue = this->config->getEntry(entry)->get(key);
		updateEditor();
	}
	
	void CalxConfigEditor::OnIntegerEdit(wxCommandEvent &evt) {
		int_conf_t value = this->integerSpin->GetValue();
		if (this->currentValue != nullptr &&
			this->currentValue->getType() == ConfigValueType::Integer) {
			this->valueList->SetValue(wxVariant(std::to_string(value)), this->valueList->GetSelectedRow(), 2);
			((IntegerConfigValue*) this->currentValue)->setValue(value);
		}
	}
	
	void CalxConfigEditor::OnRealEdit(wxCommandEvent &evt) {
		wxString raw = this->realCtrl->GetValue();
		double value;
		if (!raw.ToDouble(&value)) {
			wxMessageBox("Enter valid real value", "Error", wxICON_ERROR);
			wxVariant vrt;
			this->valueList->GetValue(vrt, this->valueList->GetSelectedRow(), 2);
			this->realCtrl->SetValue(vrt.GetString());
			return;
		}
		if (this->currentValue != nullptr &&
			this->currentValue->getType() == ConfigValueType::Real) {
			this->valueList->SetValue(wxVariant(std::to_string(value)), this->valueList->GetSelectedRow(), 2);
			((RealConfigValue*) this->currentValue)->setValue(value);
		}
	}
	
	void CalxConfigEditor::OnBooleanEdit(wxCommandEvent &evt) {
		bool value = this->booleanCheckbox->GetValue();
		if (this->currentValue != nullptr &&
			this->currentValue->getType() == ConfigValueType::Boolean) {
			this->valueList->SetValue(wxVariant(std::string(value ? "true" : "false")), this->valueList->GetSelectedRow(), 2);
			((BoolConfigValue*) this->currentValue)->setValue(value);
		}
	}
	
	void CalxConfigEditor::OnStringEdit(wxCommandEvent &evt) {
		std::string value = this->stringCtrl->GetValue().ToStdString();
		if (this->currentValue != nullptr &&
			this->currentValue->getType() == ConfigValueType::String) {
			this->valueList->SetValue(wxVariant(value), this->valueList->GetSelectedRow(), 2);
			((StringConfigValue*) this->currentValue)->setValue(value);
		}
	}
	
	void CalxConfigEditor::OnNewKeyClick(wxCommandEvent &evt) {
		if (this->entryList->GetSelection() != wxNOT_FOUND) {
			ConfigEntry *entry = this->config->getEntry(this->entryList->GetStringSelection().ToStdString());
			CalxNewKeyDialog *dialog = new CalxNewKeyDialog(this, wxID_ANY, entry);
			dialog->ShowModal();
			delete dialog;
			updateEntry();
		} else {
			wxMessageBox("Select one entry to add key", "Attention", wxICON_INFORMATION);
		}
	}
	
	void CalxConfigEditor::OnRemoveKeyClick(wxCommandEvent &evt) {
		if (this->entryList->GetSelection() != wxNOT_FOUND &&
			this->valueList->GetSelectedRow() != wxNOT_FOUND) {
			ConfigEntry *entry = this->config->getEntry(this->entryList->GetStringSelection().ToStdString());
			wxVariant vrt;
			this->valueList->GetValue(vrt, this->valueList->GetSelectedRow(), 0);
			std::string key = vrt.GetString().ToStdString();
			entry->remove(key);
			updateEntry();
		} else {
			wxMessageBox("Select key to remove", "Warning", wxICON_WARNING);
		}
	}
	
	void CalxConfigEditor::OnExit(wxCloseEvent &evt) {
		Destroy();
	}
	
	void CalxConfigEditor::updateEntries() {
		this->entryList->Clear();
		std::vector<ConfigEntry*> entries;
		config->getEntries(entries);
		for (const auto& entry : entries) {
			this->entryList->Append(entry->getEntryName());
		}
		Layout();
		this->entryList->SetSelection(wxNOT_FOUND);
	}
	
	void CalxConfigEditor::updateEntry() {
		this->valueList->DeleteAllItems();
		if (this->entryList->GetSelection() == wxNOT_FOUND) {
			return;
		}
		
		std::string entryName = this->entryList->GetStringSelection().ToStdString();
		ConfigEntry *entry = this->config->getEntry(entryName);
		std::vector<std::pair<std::string, ConfigValue*>> content;
		entry->getContent(content);
		for (const auto& kv : content) {
			wxVector<wxVariant> data;
			data.push_back(wxVariant(kv.first));
			ConfigValue *value = kv.second;
			switch (value->getType()) {
				case ConfigValueType::Integer:
					data.push_back(wxVariant("integer"));
					data.push_back(wxVariant(std::to_string(((IntegerConfigValue*) value)->getValue())));
				break;
				case ConfigValueType::Real:
					data.push_back(wxVariant("real"));
					data.push_back(wxVariant(std::to_string(((RealConfigValue*) value)->getValue())));
				break;
				case ConfigValueType::Boolean:
					data.push_back(wxVariant("boolean"));
					data.push_back(wxVariant(((BoolConfigValue*) value)->getValue() ? "true" : "false"));
				break;
				case ConfigValueType::String:
					data.push_back(wxVariant("string"));
					data.push_back(wxVariant(((StringConfigValue*) value)->getValue()));
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
		if (this->currentValue == nullptr) {
			this->editorPanel->Layout();
			this->valuePanel->Layout();
			return;
		}
		switch (this->currentValue->getType()) {
			case ConfigValueType::Integer:
				this->integerEditor->Show(true);
				this->integerSpin->SetValue(((IntegerConfigValue*) this->currentValue)->getValue());
			break;
			case ConfigValueType::Real:
				this->realEditor->Show(true);
				this->realCtrl->SetValue(std::to_string(((RealConfigValue*) this->currentValue)->getValue()));
			break;
			case ConfigValueType::Boolean:
				this->booleanEditor->Show(true);
				this->booleanCheckbox->SetValue(((BoolConfigValue*) this->currentValue)->getValue());
			break;
			case ConfigValueType::String:
				this->stringEditor->Show(true);
				this->stringCtrl->SetValue(((StringConfigValue*) this->currentValue)->getValue());
			break;
		}
		this->editorPanel->Layout();
		this->valuePanel->Layout();
	}
	
	CalxConfigDialog::CalxConfigDialog(wxWindow *win, wxWindowID id, ConfigManager *conf)
		: wxDialog::wxDialog(win, id, "Configuration Editor", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		this->editor = new CalxConfigEditor(this, wxID_ANY, conf);
		sizer->Add(this->editor, 1, wxEXPAND | wxALL, 5);
		
		wxButton *okButton = new wxButton(this, wxID_OK, "OK");
		sizer->Add(okButton, 0, wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxConfigDialog::OnOkClick, this);
		
		Layout();
		Fit();
	}
	
	ConfigManager *CalxConfigDialog::getConfiguration() {
		return this->editor->getConfiguration();
	}
	
	void CalxConfigDialog::OnOkClick(wxCommandEvent &evt) {
		Hide();
	}
}