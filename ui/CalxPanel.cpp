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


#include "CalxPanel.h"
#include "dev/CalxDevicePanel.h"
#include "coord/CalxCoordPanel.h"
#include "task/CalxTaskPanel.h"
#include "CalxConfigEditor.h"

namespace CalXUI {
	
	CalxPanel::CalxPanel(wxWindow *win, wxWindowID id)
		: wxNotebook::wxNotebook(win, id) {
		
		this->dev = new CalxDevicePanel(this, wxID_ANY);
		this->AddPage(this->dev, __("Devices"));
		this->coord = new CalxCoordPanel(this, wxID_ANY);
		this->AddPage(this->coord, __("Coordinate planes"));
		this->task = new CalxTaskPanel(this, wxID_ANY);
		this->AddPage(this->task, __("Tasks"));
		this->conf = new CalxConfigEditor(this, wxID_ANY, wxGetApp().getSystemManager()->getConfiguration());
		this->AddPage(this->conf, __("Configuration"));
		
		Bind(wxEVT_CLOSE_WINDOW, &CalxPanel::OnExit, this);
	}
	
	void CalxPanel::updateUI() {
		this->coord->updateUI();
		this->task->updateUI();
	}
	
	void CalxPanel::OnExit(wxCloseEvent &evt) {
		getTasks()->stop();
		getCoords()->stop();
		getDevices()->stop();
		
		wxMilliSleep(400);
		
		getCoords()->Close(true);
		getDevices()->Close(true);
		getTasks()->Close(true);
	}
}