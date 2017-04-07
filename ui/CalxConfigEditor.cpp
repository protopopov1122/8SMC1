#include "CalxConfigEditor.h"
#include <wx/sizer.h>
#include <wx/splitter.h>

namespace CalXUI {
	
	CalxConfigEditor::CalxConfigEditor(wxWindow *win, wxWindowID id, ConfigManager *conf)
		: wxScrolledWindow::wxScrolledWindow(win, id) {
		
		this->config = conf;
		
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
		
		wxPanel *valuePanel = new wxPanel(mainPanel, wxID_ANY);
		wxBoxSizer *valueSizer = new wxBoxSizer(wxVERTICAL);
		valuePanel->SetSizer(valueSizer);
		this->valueList = new wxDataViewListCtrl (valuePanel, wxID_ANY);
		valueSizer->Add(this->valueList, 1, wxEXPAND | wxALL);
		this->valueList->AppendTextColumn("Name");
		this->valueList->AppendTextColumn("Type");
		this->valueList->AppendTextColumn("Value");
		
		mainPanel->Initialize(valuePanel);
		mainPanel->SplitVertically(entryPanel, valuePanel);
		mainPanel->SetSashGravity(0.1f);
		
		updateEntries();
		Layout();
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