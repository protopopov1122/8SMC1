#include "CalxConfigEditor.h"
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/stattext.h>

namespace CalXUI {
	
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
	
	void CalxConfigEditor::OnEntryClick(wxCommandEvent &evt) {
		updateEntry();
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
	}
	
	void CalxConfigEditor::updateEntry() {
		if (this->entryList->GetSelection() == wxNOT_FOUND) {
			return;
		}
		
		this->valueList->DeleteAllItems();
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