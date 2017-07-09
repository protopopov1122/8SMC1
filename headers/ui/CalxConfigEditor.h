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

#ifndef CALX_UI_CALX_CONFIG_EDITOR_H_
#define CALX_UI_CALX_CONFIG_EDITOR_H_

#include "ui/CalxApp.h"
#include <wx/listbox.h>
#include <wx/dataview.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include "ctrl-lib/conf/ConfigManager.h"
#include "ui/CalxPanelPane.h"

namespace CalXUI {

  class CalxConfigEditor;  // Forward referencing

  class CalxNewKeyDialog : public wxDialog {
   public:
	CalxNewKeyDialog(wxWindow *, wxWindowID, ConfigEntry *);

   private:
	void updateUI();

	void OnOkClick(wxCommandEvent &);
	void OnCancelClick(wxCommandEvent &);
	void OnTypeChange(wxCommandEvent &);

	ConfigEntry *entry;
	wxTextCtrl *key;
	wxComboBox *type;

	wxPanel *editorPanel;
	wxSpinCtrl *integer;
	wxTextCtrl *real;
	wxCheckBox *boolean;
	wxTextCtrl *string;
  };

  class CalxNewEntryDialog : public wxDialog {
   public:
	CalxNewEntryDialog(wxWindow *, wxWindowID, ConfigManager *);

   private:
	void OnOkClick(wxCommandEvent &);
	void OnCancelClick(wxCommandEvent &);

	wxTextCtrl *entryName;
	ConfigManager *config;
  };

  class CalxConfigEventListener : public ConfigEventListener {
   public:
	CalxConfigEventListener(CalxConfigEditor *);
	virtual ~CalxConfigEventListener();
	virtual void entryAdded(ConfigManager *, std::string);
	virtual void entryRemoved(ConfigManager *, std::string);
	virtual void keyAdded(ConfigManager *, ConfigEntry *, std::string);
	virtual void keyRemoved(ConfigManager *, ConfigEntry *, std::string);
	virtual void keyChanged(ConfigManager *, ConfigEntry *, std::string);

   private:
	CalxConfigEditor *editor;
  };

  class CalxConfigEditor : public CalxPanelPane {
   public:
	CalxConfigEditor(wxWindow *, wxWindowID, ConfigManager *);
	ConfigManager *getConfiguration();
	wxButton *getOkButton();

	virtual bool isBusy();
	virtual void shutdown();
	virtual void updateUI();

	void updateEntries();
	void updateEntry();
	void updateEditor();
	void updateKey();

   private:
	void OnEntryClick(wxCommandEvent &);
	void OnKeyClick(wxDataViewEvent &);
	void OnIntegerEdit(wxCommandEvent &);
	void OnRealEdit(wxCommandEvent &);
	void OnBooleanEdit(wxCommandEvent &);
	void OnStringEdit(wxCommandEvent &);
	void OnNewKeyClick(wxCommandEvent &);
	void OnRemoveKeyClick(wxCommandEvent &);
	void OnNewEntryClick(wxCommandEvent &);
	void OnRemoveEntryClick(wxCommandEvent &);
	void OnExportClick(wxCommandEvent &);
	void OnExit(wxCloseEvent &);
	wxButton *okButton;
	wxListBox *entryList;
	wxDataViewListCtrl *valueList;
	wxPanel *valuePanel;
	wxPanel *editorPanel;

	ConfigValue *currentValue;
	wxPanel *integerEditor;
	wxSpinCtrl *integerSpin;
	wxPanel *realEditor;
	wxTextCtrl *realCtrl;
	wxPanel *booleanEditor;
	wxCheckBox *booleanCheckbox;
	wxPanel *stringEditor;
	wxTextCtrl *stringCtrl;

	ConfigManager *config;
	CalxConfigEventListener *listener;
  };

  class CalxConfigDialog : public wxDialog {
   public:
	CalxConfigDialog(wxWindow *, wxWindowID, ConfigManager *);
	ConfigManager *getConfiguration();

   private:
	void OnOkClick(wxCommandEvent &);
	CalxConfigEditor *editor;
  };
}

#endif