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

#ifndef CALX_UI_SCRIPT_CALX_SCRIPT_PANEL_H_
#define CALX_UI_SCRIPT_CALX_SCRIPT_PANEL_H_

#include "calx/ui/CalxApp.h"
#include "calx/ui/CalxPanelPane.h"
#include "calx/ui/script/CalxScriptHandle.h"
#include "calx/ui/CalxActionQueue.h"
#include <wx/listbox.h>
#include <vector>

namespace CalX::UI {

	wxDECLARE_EVENT(wxEVT_ENABLE_EVENT, wxThreadEvent);

	class CalxScriptPanel : public CalxPanelPane, CalxActionReceiver {
	 public:
		CalxScriptPanel(wxWindow *, wxWindowID, CalXScriptUIEnvironment &,
		                CalXScriptUIFactory &);

		bool isBusy() override;
		void shutdown() override;
		void updateUI() override;
		void setEnabled(bool);

		void sendAction(std::unique_ptr<CalxAction>) override;

	 private:
		void OnListClick(wxCommandEvent &);
		void OnLoadScriptClick(wxCommandEvent &);
		void OnOpenShellClick(wxCommandEvent &);
		void OnRemoveScriptClick(wxCommandEvent &);
		void OnExit(wxCloseEvent &);
		void OnEnableEvent(wxThreadEvent &);
		void addHandle(const std::string &, CalxScriptHandle *);

		CalXScriptUIEnvironment &env;
		CalXScriptUIFactory &scriptFactory;
		CalxActionQueue *queue;

		wxListBox *scriptList;
		wxPanel *scriptPanel;
		std::vector<CalxScriptHandle *> scripts;
	};
}  // namespace CalX::UI

#endif