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

#include "CalxApp.h"
#include <wx/listbox.h>
#include <wx/dataview.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include "ctrl-lib/ConfigManager.h"

namespace CalXUI {
	
	class CalxConfigEditor : public wxScrolledWindow {
		public:
			CalxConfigEditor(wxWindow*, wxWindowID, ConfigManager*);
			ConfigManager *getConfiguration();
			wxButton *getOkButton();
		private:
			void updateEntries();
			void updateEntry();
			void updateEditor();
		
			void OnEntryClick(wxCommandEvent&);
			void OnKeyClick(wxDataViewEvent&);
			void OnIntegerEdit(wxCommandEvent&);
			void OnRealEdit(wxCommandEvent&);
			void OnBooleanEdit(wxCommandEvent&);
			void OnStringEdit(wxCommandEvent&);
			void OnExit(wxCloseEvent&);
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
	};
	
	class CalxConfigDialog : public wxDialog {
		public:
			CalxConfigDialog(wxWindow*, wxWindowID, ConfigManager*);
			ConfigManager *getConfiguration();
		private:
			void OnOkClick(wxCommandEvent&);
			CalxConfigEditor *editor;
	};
}


#endif