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

#include "calx/ctrl-lib/conf/INI.h"
#include "calx/ctrl-lib/conf/ConfigManager.h"
#include "calx/ui/CalxFrame.h"
#include "calx/ui/CalxPanel.h"
#include "calx/ui/config/CalxConfigEditor.h"
#include "calx/ui/coord/CalxCoordArcComponent.h"
#include "calx/ui/coord/CalxCoordFilterComponent.h"
#include "calx/ui/coord/CalxCoordGraphComponent.h"
#include "calx/ui/coord/CalxCoordLinearComponent.h"
#include "calx/ui/coord/CalxCoordOtherComponent.h"
#include "calx/ui/coord/CalxCoordPanel.h"
#include "calx/ui/coord/CalxCoordPositionComponent.h"
#include "calx/ui/coord/CalxPlaneList.h"
#include "calx/ui/dev/CalxDeviceConstructor.h"
#include "calx/ui/dev/CalxDevicePanel.h"
#include "calx/ui/dev/CalxSerialInstrument.h"
#include "calx/ui/dev/CalxSerialMotor.h"
#include "calx/ui/task/CalxGCodeTask.h"
#include "calx/ui/task/CalxLinearTask.h"
#include "calx/ui/task/CalxProgrammedTask.h"
#include "calx/ui/task/CalxTaskPanel.h"
#include "calx/ui/script/CalxScriptPanel.h"
#include "calx/ui/math/CalxMathPanel.h"
#include "calx/ui/logs/CalxLogPanel.h"
#include <iostream>
#include <wx/aboutdlg.h>
#include <wx/app.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>

namespace CalX::UI {

	std::unique_ptr<ConfigurationCatalogue> loadUIConfiguration() {
		std::string uiConfigPath = wxGetApp()
		                               .getSystemManager()
		                               .getConfiguration()
		                               .getEntry("ui")
		                               ->getString("ui_configuration", "");
		if (!uiConfigPath.empty()) {
			std::ifstream is(uiConfigPath);
			return INIConfiguration::load(is, std::cout);
		} else {
			return std::make_unique<ConfigManager>();
		}
	}

	CalxDevicePanel *newDevicePanel(wxWindow *win,
	                                ConfigurationCatalogue &uiConfig) {
		auto &devicesConfig = *uiConfig.getEntry("devices");
		CalxDevicePanel *devPanel = new CalxDevicePanel(win, wxID_ANY);
		std::vector<DeviceConnectionType> devConType;
		std::vector<DeviceConnectionType> instrConType;
		wxGetApp().getSystemManager().getDeviceManager().getConnectionTypes(
		    devConType, instrConType);
		for (const auto &devCon : devConType) {
			switch (devCon) {
				case DeviceConnectionType::SerialPort:
					if (devicesConfig.getBool("com_motor", true)) {
						devPanel->appendDeviceFactory(__("COM Motor"),
						                              new CalxSerialMotorFactory());
					}
					break;
			}
		}
		for (const auto &instrCon : instrConType) {
			switch (instrCon) {
				case DeviceConnectionType::SerialPort:
					if (devicesConfig.getBool("com_instrument", true)) {
						devPanel->appendDeviceFactory(__("COM Instrument"),
						                              new CalxSerialInstrumentFactory());
					}
					break;
			}
		}

		for (std::size_t i = 0;
		     i <
		     wxGetApp().getSystemManager().getMotorControllerSet().getDeviceCount();
		     i++) {
			devPanel->appendDevice(new CalxMotorConstructor(
			    devPanel, wxGetApp()
			                  .getSystemManager()
			                  .getMotorControllerSet()
			                  .getDeviceController((device_id_t) i)
			                  .lock()));
		}

		for (std::size_t i = 0; i < wxGetApp()
		                                .getSystemManager()
		                                .getInstrumentControllerSet()
		                                .getDeviceCount();
		     i++) {
			devPanel->appendDevice(new CalxInstrumentConstructor(
			    devPanel, wxGetApp()
			                  .getSystemManager()
			                  .getInstrumentControllerSet()
			                  .getDeviceController((device_id_t) i)
			                  .lock()));
		}
		devPanel->updateUI();
		return devPanel;
	}

	CalxCoordPanel *newCoordPanel(wxWindow *win,
	                              ConfigurationCatalogue &uiConfig) {
		auto &planesConfig = *uiConfig.getEntry("planes");
		CalxCoordPanel *coordPanel =
		    new CalxCoordPanel(win, wxID_ANY, planesConfig.getInt("columns", 2));
		if (planesConfig.getBool("linear_movement", true)) {
			coordPanel->addComponentFactory(
			    __("Linear movement"), new CalxCoordLinearComponentFactory(),
			    planesConfig.getInt("linear_movement_col", 0),
			    planesConfig.getBool("linear_movement_min", true));
		}
		if (planesConfig.getBool("arc_movement", true)) {
			coordPanel->addComponentFactory(
			    __("Arc movement"), new CalxCoordArcComponentFactory(),
			    planesConfig.getInt("arc_movement_col", 0),
			    planesConfig.getBool("arc_movement_min", true));
		}
		if (planesConfig.getBool("function", true)) {
			coordPanel->addComponentFactory(
			    __("Function graph"), new CalxCoordGraphComponentFactory(),
			    planesConfig.getInt("function_col", 0),
			    planesConfig.getBool("function_min", false));
		}
		if (planesConfig.getBool("other", true)) {
			coordPanel->addComponentFactory(__("Other"),
			                                new CalxCoordOtherComponentFactory(),
			                                planesConfig.getInt("other_col", 1),
			                                planesConfig.getBool("other_min", false));
		}
		if (planesConfig.getBool("position", true)) {
			coordPanel->addComponentFactory(
			    __("Position"), new CalxCoordPositionComponentFactory(),
			    planesConfig.getInt("position_col", 1),
			    planesConfig.getBool("position_min", false));
		}
		if (planesConfig.getBool("filters", true)) {
			coordPanel->addComponentFactory(
			    __("Filters"), new CalxCoordFilterComponentFactory(),
			    planesConfig.getInt("filters_col", 1),
			    planesConfig.getBool("filters_min", true));
		}
		return coordPanel;
	}

	CalxTaskPanel *newTaskPanel(wxWindow *win, ConfigurationCatalogue &uiConfig) {
		CalxTaskPanel *taskPanel = new CalxTaskPanel(win, wxID_ANY);
		auto &tasksConfig = *uiConfig.getEntry("tasks");
		if (tasksConfig.getBool("gcode", true)) {
			taskPanel->attachTaskFactory(__("GCode"), new CalxGCodeTaskFactory());
		}
		if (tasksConfig.getBool("programmed", true)) {
			taskPanel->attachTaskFactory(__("Programmed"),
			                             new CalxProgrammedTaskFactory());
		}
		if (tasksConfig.getBool("linear", true)) {
			taskPanel->attachTaskFactory(__("Linear"), new CalxLinearTaskFactory());
		}
		return taskPanel;
	}

	CalxConfigEditor *newConfigPanel(wxWindow *win) {
		CalxConfigEditor *editor = new CalxConfigEditor(
		    win, wxID_ANY, wxGetApp().getSystemManager().getConfiguration());
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

	CalxFrame::CalxFrame(const std::string &title)
	    : wxFrame::wxFrame(nullptr, wxID_ANY, title) {
		this->CreateStatusBar(1);
		this->SetStatusText(__("CalX UI"), 0);

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);
		this->panel = new CalxPanel(this, wxID_ANY);
		wxPanel *statusPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(this->panel, 1, wxEXPAND | wxALL);
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

		std::unique_ptr<ConfigurationCatalogue> uiConfig = loadUIConfiguration();

		CalxDevicePanel *devicePanel = newDevicePanel(panel, *uiConfig);
		CalxCoordPanel *coordPanel = newCoordPanel(panel, *uiConfig);
		CalxTaskPanel *taskPanel = newTaskPanel(panel, *uiConfig);
		this->device_pool = devicePanel;
		this->plane_list = coordPanel;
		this->task_list = taskPanel;

		CalxScriptPanel *scriptPanel = newScriptPanel(panel);
		CalxMathPanel *mathPanel = new CalxMathPanel(panel, wxID_ANY);
		CalxLogPanel *logsPanel = new CalxLogPanel(panel, wxID_ANY);
		this->math_engine = mathPanel;
		this->logSink = logsPanel;

		auto &uiPanes = *uiConfig->getEntry("panes");
		if (uiPanes.getBool("devices", true)) {
			panel->addPane(__("Devices"), devicePanel);
		}
		if (uiPanes.getBool("planes", true)) {
			panel->addPane(__("Coordinate planes"), coordPanel);
		}
		if (uiPanes.getBool("tasks", true)) {
			panel->addPane(__("Tasks"), taskPanel);
		}
		if (uiPanes.getBool("configuration", true)) {
			panel->addPane(__("Configuration"), newConfigPanel(panel));
		}
		if (scriptPanel != nullptr && uiPanes.getBool("scripts", true)) {
			panel->addPane(__("Scripts"), scriptPanel);
		}
		if (uiPanes.getBool("math", true)) {
			panel->addPane(__("Math"), mathPanel);
		}
		if (uiPanes.getBool("logs", true)) {
			panel->addPane(__("Logs"), logsPanel);
		}
		panel->SetSelection(uiPanes.getInt("default", 0));

		wxBoxSizer *statusSizer = new wxBoxSizer(wxHORIZONTAL);
		statusPanel->SetSizer(statusSizer);
		wxButton *stopButton = new wxButton(statusPanel, wxID_ANY, __("Stop All"));
		statusSizer->Add(stopButton);
		stopButton->Bind(wxEVT_BUTTON, &CalxFrame::OnStopClick, this);

		Layout();
		Fit();
	}

	CalxPanel *CalxFrame::getPanel() {
		return this->panel;
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

	CalxLogSink *CalxFrame::getLogSink() {
		return this->logSink;
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
		    "the Free Software Foundation\n"
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
		about.SetCopyright(__("(C) 2017-2019 Jevgenijs Protopopovs"));
		about.SetWebSite("https://github.com/protopopov1122/CalX");
		about.SetLicense(LICENSE);

		wxAboutBox(about);
	}
}  // namespace CalX::UI
