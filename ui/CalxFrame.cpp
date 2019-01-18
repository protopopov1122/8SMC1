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

#include "ui/CalxFrame.h"
#include "ui/CalxPanel.h"
#include "ui/config/CalxConfigEditor.h"
#include "ui/coord/CalxCoordArcComponent.h"
#include "ui/coord/CalxCoordFilterComponent.h"
#include "ui/coord/CalxCoordGraphComponent.h"
#include "ui/coord/CalxCoordLinearComponent.h"
#include "ui/coord/CalxCoordOtherComponent.h"
#include "ui/coord/CalxCoordPanel.h"
#include "ui/coord/CalxCoordPositionComponent.h"
#include "ui/coord/CalxPlaneList.h"
#include "ui/dev/CalxDeviceConstructor.h"
#include "ui/dev/CalxDevicePanel.h"
#include "ui/dev/CalxSerialInstrument.h"
#include "ui/dev/CalxSerialMotor.h"
#include "ui/task/CalxGCodeTask.h"
#include "ui/task/CalxLinearTask.h"
#include "ui/task/CalxProgrammedTask.h"
#include "ui/task/CalxTaskPanel.h"
#include "ui/script/CalxScriptPanel.h"
#include "ui/math/CalxMathPanel.h"
#include <iostream>
#include <wx/aboutdlg.h>
#include <wx/app.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>
namespace CalXUI {

	CalxDevicePanel *newDevicePanel(wxWindow *win) {
		CalxDevicePanel *devPanel = new CalxDevicePanel(win, wxID_ANY);
		std::vector<DeviceConnectionType> devConType;
		std::vector<DeviceConnectionType> instrConType;
		wxGetApp().getSystemManager()->getDeviceManager().getConnectionTypes(
		    devConType, instrConType);
		for (const auto &devCon : devConType) {
			switch (devCon) {
				case DeviceConnectionType::SerialPort: {
					devPanel->appendDeviceFactory(__("COM Motor"),
					                              new CalxSerialMotorFactory());
				} break;
			}
		}
		for (const auto &instrCon : instrConType) {
			switch (instrCon) {
				case DeviceConnectionType::SerialPort: {
					devPanel->appendDeviceFactory(__("COM Instrument"),
					                              new CalxSerialInstrumentFactory());
				} break;
			}
		}

		for (size_t i = 0; i < wxGetApp()
		                           .getSystemManager()
		                           ->getMotorControllerSet()
		                           .getDeviceCount();
		     i++) {
			devPanel->appendDevice(new CalxMotorConstructor(
			    devPanel, wxGetApp()
			                  .getSystemManager()
			                  ->getMotorControllerSet()
			                  .getDeviceController((device_id_t) i)
			                  .lock()));
		}

		for (size_t i = 0; i < wxGetApp()
		                           .getSystemManager()
		                           ->getInstrumentControllerSet()
		                           .getDeviceCount();
		     i++) {
			devPanel->appendDevice(new CalxInstrumentConstructor(
			    devPanel, wxGetApp()
			                  .getSystemManager()
			                  ->getInstrumentControllerSet()
			                  .getDeviceController((device_id_t) i)
			                  .lock()));
		}
		devPanel->updateUI();
		return devPanel;
	}

	CalxCoordPanel *newCoordPanel(wxWindow *win) {
		CalxCoordPanel *coordPanel = new CalxCoordPanel(win, wxID_ANY, 2);
		coordPanel->addComponentFactory(__("Linear movement"),
		                                new CalxCoordLinearComponentFactory(), 0);
		coordPanel->addComponentFactory(__("Arc movement"),
		                                new CalxCoordArcComponentFactory(), 0);
		coordPanel->addComponentFactory(
		    __("Function graph"), new CalxCoordGraphComponentFactory(), 0, false);
		coordPanel->addComponentFactory(
		    __("Other"), new CalxCoordOtherComponentFactory(), 1, false);
		coordPanel->addComponentFactory(
		    __("Position"), new CalxCoordPositionComponentFactory(), 1, false);
		coordPanel->addComponentFactory(__("Filters"),
		                                new CalxCoordFilterComponentFactory(), 1);
		return coordPanel;
	}

	CalxTaskPanel *newTaskPanel(wxWindow *win) {
		CalxTaskPanel *taskPanel = new CalxTaskPanel(win, wxID_ANY);
		taskPanel->attachTaskFactory(__("GCode"), new CalxGCodeTaskFactory());
		taskPanel->attachTaskFactory(__("Programmed"),
		                             new CalxProgrammedTaskFactory());
		taskPanel->attachTaskFactory(__("Linear"), new CalxLinearTaskFactory());
		return taskPanel;
	}

	CalxConfigEditor *newConfigPanel(wxWindow *win) {
		CalxConfigEditor *editor = new CalxConfigEditor(
		    win, wxID_ANY, wxGetApp().getSystemManager()->getConfiguration());
		return editor;
	}

	CalxScriptPanel *newScriptPanel(wxWindow *win) {
		if (wxGetApp().hasScriptEngine()) {
			CalxScriptPanel *scripts =
			    new CalxScriptPanel(win, wxID_ANY, *wxGetApp().getScriptEnvironment(),
			                        *wxGetApp().getScriptFactory());
			return scripts;
		} else {
			return nullptr;
		}
	}

	CalxFrame::CalxFrame(std::string title)
	    : wxFrame::wxFrame(nullptr, wxID_ANY, title) {
		this->CreateStatusBar(1);
		this->SetStatusText(__("CalX UI"), 0);

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);
		this->panel = new CalxPanel(this, wxID_ANY);
		this->quickstartPanel = new CalxPanel(this, wxID_ANY);
		wxPanel *statusPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(this->panel, 1, wxEXPAND | wxALL);
		sizer->Add(this->quickstartPanel, 1, wxEXPAND | wxALL);
		sizer->Add(statusPanel, 0, wxEXPAND | wxALL);

		Bind(wxEVT_CLOSE_WINDOW, &CalxFrame::OnClose, this);

		this->menuBar = new wxMenuBar();
		this->aboutMenu = new wxMenu();
		wxMenuItem *aboutItem =
		    new wxMenuItem(this->aboutMenu, wxID_ABOUT, __("About"));
		this->aboutMenu->Append(aboutItem);
		Bind(wxEVT_COMMAND_MENU_SELECTED, &CalxFrame::OnAboutMenuClick, this,
		     wxID_ABOUT);
		this->menuBar->Append(this->aboutMenu, __("About"));
		SetMenuBar(this->menuBar);

		CalxDevicePanel *devicePanel = newDevicePanel(panel);
		CalxCoordPanel *coordPanel = newCoordPanel(panel);
		CalxTaskPanel *taskPanel = newTaskPanel(panel);
		this->device_pool = devicePanel;
		this->plane_list = coordPanel;
		this->task_list = taskPanel;

		CalxScriptPanel *scriptPanel = newScriptPanel(panel);
		CalxMathPanel *mathPanel = new CalxMathPanel(panel, wxID_ANY);
		this->math_engine = mathPanel;

		panel->addPane(__("Devices"), devicePanel);
		panel->addPane(__("Coordinate planes"), coordPanel);
		panel->addPane(__("Tasks"), taskPanel);
		panel->addPane(__("Configuration"), newConfigPanel(panel));
		if (scriptPanel != nullptr) {
			panel->addPane(__("Scripts"), scriptPanel);
		}
		panel->addPane(__("Math"), mathPanel);

		wxBoxSizer *statusSizer = new wxBoxSizer(wxHORIZONTAL);
		statusPanel->SetSizer(statusSizer);
		/*this->switchButton =
		    new wxButton(statusPanel, wxID_ANY, __("Quickstart/Advanced"));
		statusSizer->Add(this->switchButton);
		switchButton->Bind(wxEVT_BUTTON, &CalxFrame::OnSwitchClick, this);*/
		wxButton *stopButton = new wxButton(statusPanel, wxID_ANY, __("Stop All"));
		statusSizer->Add(stopButton);
		stopButton->Bind(wxEVT_BUTTON, &CalxFrame::OnStopClick, this);

		this->quickstartPanel->Show(false);

		Layout();
		Fit();
	}

	CalxPanel *CalxFrame::getPanel() {
		return this->panel;
	}

	CalxPanel *CalxFrame::getQuickstart() {
		return this->quickstartPanel;
	}

	CalxDevicePool *CalxFrame::getDevicePool() {
		return this->device_pool;
	}

	CalxPlaneList *CalxFrame::getPlaneList() {
		return this->plane_list;
	}

	CalxTaskList *CalxFrame::getTaskList() {
		return this->task_list;
	}

	CalxMathEngine *CalxFrame::getMathEngine() {
		return this->math_engine;
	}

	void CalxFrame::OnStopClick(wxCommandEvent &evt) {
		this->task_list->stop();
		this->plane_list->stop();
		this->device_pool->stop();
	}

	void CalxFrame::OnClose(wxCloseEvent &evt) {
		if (panel->isBusy()) {
			if (wxMessageBox(__("Cancel current and actions and exit"),
			                 __("Actions are performed"),
			                 wxYES_NO | wxICON_QUESTION) == wxNO) {
				evt.Veto();
				return;
			}
		}
		this->panel->Close(true);
		Destroy();
	}

	void CalxFrame::OnAboutMenuClick(wxCommandEvent &evt) {
		wxAboutDialogInfo about;
		std::string LICENSE =
		    "CalX is free software: you can redistribute it and/or modify\n"
		    "it under the terms of the GNU Lesser General Public License as "
		    "published by\n"
		    "the Free Software Foundation, either version 3 of the License, or\n"
		    "(at your option) any later version.\n"
		    "CalX is distributed in the hope that it will be useful,\n"
		    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		    "GNU Lesser General Public License for more details.\n"
		    "You should have received a copy of the GNU Lesser General Public "
		    "License\n"
		    "along with CalX.  If not, see <http://www.gnu.org/licenses/>.";

		about.SetName(__("CalX"));
		about.AddDeveloper(__("Jevgenijs Protopopovs"));
		about.AddDeveloper(__("Mihails Andzans"));
		about.SetDescription(
		    __("CalX - stepped motor and instrument control UI and library."));
		about.SetCopyright(__("(C) 2017 Jevgenijs Protopopovs"));
		about.SetWebSite("https://github.com/protopopov1122/CalX");
		about.SetLicense(LICENSE);

		wxAboutBox(about);
	}

	void CalxFrame::OnSwitchClick(wxCommandEvent &evt) {
		this->switch_modes();
	}

	void CalxFrame::switch_modes() {
		this->quickstartPanel->Show(!this->quickstartPanel->IsShown());
		this->panel->Show(!this->panel->IsShown());
		Layout();
	}
}  // namespace CalXUI
