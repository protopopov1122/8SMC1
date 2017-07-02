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


#ifndef CALX_UI_CALX_PANEL_H_
#define CALX_UI_CALX_PANEL_H_

#include "ui/calx.h"
#include <wx/notebook.h>

namespace CalXUI {
	
	class CalxDevicePanel; // Forward referencing
	class CalxCoordPanel; // Forward referencing
	class CalxTaskPanel; // Forward referencing
	class CalxConfigEditor; //Forward referencing
	
	class CalxPanel : public wxNotebook {
		public:
			CalxPanel(wxWindow*, wxWindowID);
			CalxDevicePanel *getDevices() {return this->dev;}
			CalxCoordPanel *getCoords() {return this->coord;}
			CalxTaskPanel *getTasks() {return this->task;}
			
			void updateUI();
		private:
			void OnExit(wxCloseEvent&);
		
			CalxDevicePanel *dev;
			CalxCoordPanel *coord;
			CalxTaskPanel *task;
			CalxConfigEditor *conf;
	};
}

#endif