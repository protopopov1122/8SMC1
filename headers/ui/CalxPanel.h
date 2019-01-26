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

#ifndef CALX_UI_CALX_PANEL_H_
#define CALX_UI_CALX_PANEL_H_

#include "ui/CalxPanelPane.h"
#include "ui/calx.h"
#include <wx/notebook.h>

namespace CalX::UI {

	class CalxPanel : public wxNotebook {
	 public:
		CalxPanel(wxWindow *, wxWindowID);
		void addPane(std::string, CalxPanelPane *);
		void updateUI();
		bool isBusy();

	 private:
		void OnExit(wxCloseEvent &);

		std::vector<CalxPanelPane *> panes;
	};
}  // namespace CalX::UI

#endif