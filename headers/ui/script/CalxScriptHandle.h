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

#ifndef CALX_UI_SCRIPT_CALX_SCRIPT_HANDLE_H_
#define CALX_UI_SCRIPT_CALX_SCRIPT_HANDLE_H_

#include "ui/CalxApp.h"
#include "ui/CalxActionQueue.h"

namespace CalXUI {

	class CalxActionReceiver {
	 public:
		virtual void sendAction(std::unique_ptr<CalxAction>) = 0;
	};

	class CalxScriptHandle : public wxPanel {
	 public:
		CalxScriptHandle(wxWindow *, wxWindowID);
	};

	class CalxFileScriptHandle : public CalxScriptHandle {
	 public:
		CalxFileScriptHandle(wxWindow *, wxWindowID, CalxActionReceiver *,
		                     std::string, std::unique_ptr<CalXScript>);

	 private:
		void OnCallClick(wxCommandEvent &);
		void OnExecuteClick(wxCommandEvent &);
		void OnExit(wxCloseEvent &);

		wxTextCtrl *hookText;
		wxTextCtrl *shellText;
		CalxActionReceiver *recv;
		std::unique_ptr<CalXScript> script;
	};

	class CalxScriptShellHandle : public CalxScriptHandle {
	 public:
		CalxScriptShellHandle(wxWindow *, wxWindowID, CalxActionReceiver *,
		                      std::unique_ptr<CalXScript>);

	 private:
		void OnExecuteClick(wxCommandEvent &);
		void OnExit(wxCloseEvent &);

		wxTextCtrl *shellText;
		CalxActionReceiver *recv;
		std::unique_ptr<CalXScript> script;
	};
}  // namespace CalXUI

#endif
