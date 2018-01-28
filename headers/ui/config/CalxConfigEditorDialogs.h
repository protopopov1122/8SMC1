/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_UI_CONFIG_CALX_CONFIG_EDITOR_DIALOGS_H_
#define CALX_UI_CONFIG_CALX_CONFIG_EDITOR_DIALOGS_H_

#include "ctrl-lib/conf/ConfigManager.h"
#include "ui/CalxApp.h"
#include <wx/spinctrl.h>

namespace CalXUI {

	class CalxConfigEditor;  // Forward referencing

	class CalxNewKeyDialog : public wxDialog {
	 public:
		CalxNewKeyDialog(wxWindow *, wxWindowID, std::shared_ptr<ConfigEntry>);

	 private:
		void updateUI();

		void OnOkClick(wxCommandEvent &);
		void OnCancelClick(wxCommandEvent &);
		void OnTypeChange(wxCommandEvent &);

		std::shared_ptr<ConfigEntry> entry;
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
		CalxNewEntryDialog(wxWindow *, wxWindowID, ConfigManager &);

	 private:
		void OnOkClick(wxCommandEvent &);
		void OnCancelClick(wxCommandEvent &);

		wxTextCtrl *entryName;
		ConfigManager &config;
	};

	class CalxConfigDialog : public wxDialog {
	 public:
		CalxConfigDialog(wxWindow *, wxWindowID, ConfigManager &);
		ConfigManager &getConfiguration();

	 private:
		void OnOkClick(wxCommandEvent &);
		CalxConfigEditor *editor;
	};
}  // namespace CalXUI

#endif
