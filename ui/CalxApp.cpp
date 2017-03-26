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


#include "CalxApp.h"
#ifdef OS_WIN
	#include <windows.h>
#endif
#include <wx/filedlg.h>
#include "CalxDebugConsole.h"
#include "device/DeviceManager.h"

#include <fstream>
#include "ctrl-lib/ConfigManager.h"

namespace CalXUI {
	
	bool CalxApp::OnInit() {
		this->debug_mode = true;
		
		this->dynlib = new wxDynamicLibrary(wxDynamicLibrary::CanonicalizeName(STRINGIZE(DEVICES_LIB)), wxDL_DEFAULT | wxDL_QUIET);
		if (!dynlib->IsLoaded()) {
			wxMessageBox("Device API plugin not found\nSpecify library location", "Warning", wxOK | wxICON_WARNING);
			loadDevicesPlugin();
		}
		if (!dynlib->IsLoaded()) {
			wxMessageBox("Dynamic library not found", "Error", wxOK | wxICON_ERROR);
			return false;
		}
		
		bool suc;
		void *raw_getter = dynlib->GetSymbol("getDeviceManager", &suc);
		DeviceManager_getter getter = *((DeviceManager_getter*) &raw_getter);
		if (!suc) {
			wxMessageBox("Dynamic library is corrupt", "Error", wxOK | wxICON_ERROR);
			return false;
		}
		
		if (this->debug_mode) {
			#ifdef OS_WIN
			AllocConsole();
			AttachConsole(GetCurrentProcessId());
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);
			#endif
		}
		
		std::ifstream cnf("config.ini");
		ConfigManager *conf = ConfigManager::load(&cnf);
		cnf.close();
		this->devman = getter();
		this->sysman = new SystemManager(this->devman, conf);
		
		if (this->debug_mode) {
			this->debug_console = new CalxDebugConsole(this->sysman);
			this->debug_console->Run();
		}
		
		this->frame = new CalxFrame("CalX UI");
		this->frame->Show(true);
		this->frame->Maximize(true);
		
		return true;
	}
	
	int CalxApp::OnExit() {
		if (this->debug_mode) {
			this->debug_console->Kill();
		}
		
		delete this->sysman;
		delete this->devman;
		delete this->dynlib;
		
		#ifdef OS_WIN
		if (this->debug_mode) {
			FreeConsole();
		}
		#endif
		return 0;
	}
	
	SystemManager *CalxApp::getSystemManager() {
		return this->sysman;
	}
	
	CalxFrame *CalxApp::getMainFrame() {
		return this->frame;
	}
	
	void CalxApp::loadDevicesPlugin() {
		wxFileDialog openDialog(nullptr, "Load devices plugin");
		if (openDialog.ShowModal() == wxID_CANCEL) {
			return;
		}
		dynlib->Load(openDialog.GetPath(), wxDL_DEFAULT | wxDL_QUIET);
	}
}

wxIMPLEMENT_APP(CalXUI::CalxApp);