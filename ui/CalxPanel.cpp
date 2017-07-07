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

#include "ui/CalxPanel.h"
#include "ui/dev/CalxDevicePanel.h"
#include "ui/coord/CalxCoordPanel.h"
#include "ui/task/CalxTaskPanel.h"
#include "ui/CalxConfigEditor.h"

namespace CalXUI {

  CalxPanel::CalxPanel(wxWindow *win, wxWindowID id)
	  : wxNotebook::wxNotebook(win, id) {
	this->dev = new CalxDevicePanel(this, wxID_ANY);
	this->AddPage(this->dev, __("Devices"));
	this->coord = new CalxCoordPanel(this, wxID_ANY);
	this->AddPage(this->coord, __("Coordinate planes"));
	this->task = new CalxTaskPanel(this, wxID_ANY);
	this->AddPage(this->task, __("Tasks"));
	this->conf = new CalxConfigEditor(
		this, wxID_ANY, wxGetApp().getSystemManager()->getConfiguration());
	this->AddPage(this->conf, __("Configuration"));

	Bind(wxEVT_CLOSE_WINDOW, &CalxPanel::OnExit, this);
  }
  
  CalxDevicePanel *CalxPanel::getDevices() {
	return this->dev;
  }
  
  CalxCoordPanel *CalxPanel::getCoords()  {
	return this->coord;
  }
  
  CalxTaskPanel *CalxPanel::getTasks() {
	return this->task;
  }


  void CalxPanel::updateUI() {
	this->dev->updateUI();
	this->coord->updateUI();
	this->task->updateUI();
  }

  void CalxPanel::OnExit(wxCloseEvent &evt) {
	getTasks()->shutdown();
	getCoords()->shutdown();
	getDevices()->shutdown();

	wxMilliSleep(400);

	while (GetPageCount()) {
	  RemovePage(0);
	}
	getCoords()->Close(true);
	getDevices()->Close(true);
	getTasks()->Close(true);

	Destroy();
  }
}
