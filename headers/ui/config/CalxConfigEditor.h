/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#ifndef CALX_UI_CALX_CONFIG_EDITOR_H_
#define CALX_UI_CALX_CONFIG_EDITOR_H_

#include "ctrl-lib/conf/Dictionary.h"
#include "ui/CalxApp.h"
#include "ui/CalxPanelPane.h"
#include "ui/config/CalxConfigEditorDialogs.h"
#include <wx/checkbox.h>
#include <wx/dataview.h>
#include <wx/listbox.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>

namespace CalX::UI {

	class CalxConfigEditor;  // Forward referencing

	class CalxConfigEventListener : public CatalogueListener {
	 public:
		CalxConfigEventListener(CalxConfigEditor *);
		void entryAdd(ConfigurationCatalogue *, const std::string &) override;
		void entryRemove(ConfigurationCatalogue *, const std::string &) override;
		void keyAdd(ConfigurationCatalogue *, const std::string &,
		            const std::string &) override;
		void keyRemove(ConfigurationCatalogue *, const std::string &,
		               const std::string &) override;
		void keyChange(ConfigurationCatalogue *, const std::string &,
		               const std::string &) override;

	 private:
		CalxConfigEditor *editor;
	};

	class CalxConfigEditor : public CalxPanelPane {
	 public:
		CalxConfigEditor(wxWindow *, wxWindowID, ConfigurationCatalogue &);
		ConfigurationCatalogue &getConfiguration();
		wxButton *getOkButton();

		bool isBusy() override;
		void shutdown() override;
		void updateUI() override;

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

		ConfigurationValue currentValue;
		wxPanel *integerEditor;
		wxSpinCtrl *integerSpin;
		wxPanel *realEditor;
		wxTextCtrl *realCtrl;
		wxPanel *booleanEditor;
		wxCheckBox *booleanCheckbox;
		wxPanel *stringEditor;
		wxTextCtrl *stringCtrl;

		ConfigurationCatalogue &config;
		std::shared_ptr<CalxConfigEventListener> listener;
	};
}  // namespace CalX::UI

#endif