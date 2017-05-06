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


#include <iostream>
#include "CalxFrame.h"
#include <wx/splitter.h>
#include <wx/textctrl.h>
#include <wx/app.h>
#include <wx/aboutdlg.h>
#include "CalxPanel.h"
#include "task/CalxTaskPanel.h"
#include "coord/CalxCoordPanel.h"
#include "dev/CalxDevicePanel.h"
#include "CalxConsoleWidget.h"

namespace CalXUI {
	CalxFrame::CalxFrame(std::string title)
		: wxFrame::wxFrame(nullptr, wxID_ANY, title) {
		
		this->CreateStatusBar(1);
		this->SetStatusText(__("CalX UI"), 0);
		
		wxSplitterWindow *mainPanel = new wxSplitterWindow(this, wxID_ANY);
		
		this->panel = new CalxPanel(mainPanel, wxID_ANY);
		CalxConsoleWidget *console = new CalxConsoleWidget(mainPanel, wxID_ANY);
		panel->Show(true);
		console->Show(true);
		mainPanel->Initialize(panel);
		mainPanel->SplitHorizontally(panel, console);
		mainPanel->SetSashGravity(1.0f);	
		
		this->console = new std::ostream(console);
		
		Bind(wxEVT_CLOSE_WINDOW, &CalxFrame::OnClose, this);
		
		this->menuBar = new wxMenuBar();
		this->aboutMenu = new wxMenu();
		wxMenuItem *aboutItem = new wxMenuItem(this->aboutMenu, wxID_ABOUT, __("About"));
		this->aboutMenu->Append(aboutItem);
		Bind( wxEVT_COMMAND_MENU_SELECTED, &CalxFrame::OnAboutMenuClick, this, 
            wxID_ABOUT);
		this->menuBar->Append(this->aboutMenu, __("About"));
		SetMenuBar(this->menuBar);
		
		Layout();
		Fit();
	}
	
	CalxPanel *CalxFrame::getPanel() {
		return this->panel;
	}
	
	std::ostream *CalxFrame::getConsole() {
		return this->console;
	}

	void CalxFrame::OnClose(wxCloseEvent &evt) {
		if (panel->getDevices()->isBusy() ||
			panel->getCoords()->isBusy() ||
			panel->getTasks()->isBusy()) {
			if (wxMessageBox(__("Cancel current and actions and exit"),
				__("Actions are performed"), wxYES_NO | wxICON_QUESTION) == wxNO) {
				evt.Veto();
				return;
			}
		}
		this->panel->Close(true);
		Destroy();		
	}
	
	void CalxFrame::OnAboutMenuClick(wxCommandEvent &evt) {
		wxAboutDialogInfo about;
		std::string LICENSE = "CalX is free software: you can redistribute it and/or modify\n"
		                      "it under the terms of the GNU Lesser General Public License as published by\n"
		                      "the Free Software Foundation, either version 3 of the License, or\n"
		                      "(at your option) any later version.\n"
		                      "CalX is distributed in the hope that it will be useful,\n"
		                      "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		                      "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		                      "GNU Lesser General Public License for more details.\n"
		                      "You should have received a copy of the GNU Lesser General Public License\n"
		                      "along with CalX.  If not, see <http://www.gnu.org/licenses/>.";
		 
		 about.SetName(__("CalX"));
		 about.AddDeveloper(__("Jevgenijs Protopopovs"));
		 about.SetDescription(__("CalX - stepped motor and instrument control UI and library."));
		 about.SetCopyright(__("(C) 2017 Jevgenijs Protopopovs"));
		 about.SetWebSite("https://github.com/protopopov1122/CalX");
		 about.SetLicense(LICENSE);
		 
		 wxAboutBox(about);
	}
}
