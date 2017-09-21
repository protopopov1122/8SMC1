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

#ifndef CALX_UI_CALX_FRAME_H_
#define CALX_UI_CALX_FRAME_H_

#include "ui/CalxPanel.h"
#include "ui/calx.h"
#include <iostream>
#include <string>
#include <wx/menu.h>

namespace CalXUI {

	class CalxFrame : public wxFrame {
	 public:
		CalxFrame(std::string);
		CalxPanel *getPanel();

	 private:
		void OnClose(wxCloseEvent &);
		void OnAboutMenuClick(wxCommandEvent &);

		wxMenuBar *menuBar;
		wxMenu *aboutMenu;

		CalxPanel *panel;
	};
}  // namespace CalXUI

#endif
