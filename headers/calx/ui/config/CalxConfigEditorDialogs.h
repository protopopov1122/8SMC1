/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_UI_CONFIG_CALX_CONFIG_EDITOR_DIALOGS_H_
#define CALX_UI_CONFIG_CALX_CONFIG_EDITOR_DIALOGS_H_

#include "calx/ctrl-lib/conf/Dictionary.h"
#include "calx/ui/CalxApp.h"
#include <wx/spinctrl.h>

namespace CalX::UI {

	class CalxConfigEditor;  // Forward referencing

	class CalxNewKeyDialog : public wxDialog {
	 public:
		CalxNewKeyDialog(wxWindow *, wxWindowID, ConfiguationFlatDictionary *);

	 private:
		void updateUI();

		void OnOkClick(wxCommandEvent &);
		void OnCancelClick(wxCommandEvent &);
		void OnTypeChange(wxCommandEvent &);

		ConfiguationFlatDictionary *entry;
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
		CalxNewEntryDialog(wxWindow *, wxWindowID, ConfigurationCatalogue &);

	 private:
		void OnOkClick(wxCommandEvent &);
		void OnCancelClick(wxCommandEvent &);

		wxTextCtrl *entryName;
		ConfigurationCatalogue &config;
	};

	class CalxConfigDialog : public wxDialog {
	 public:
		CalxConfigDialog(wxWindow *, wxWindowID, ConfigurationCatalogue &);
		ConfigurationCatalogue &getConfiguration();

	 private:
		void OnOkClick(wxCommandEvent &);
		CalxConfigEditor *editor;
	};
}  // namespace CalX::UI

#endif
