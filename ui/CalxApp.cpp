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
#include "CalxErrorHandler.h"
#include "device/DeviceManager.h"
#include "ui/coord/CalxCoordPanel.h"
#include "ui/CalxAutoconfDialog.h"

#include <fstream>
#include "ctrl-lib/ConfigManager.h"
#include "ctrl-lib/ConfigValidator.h"

namespace CalXUI {
	
	class AutoconfThread : public wxThread {
		public:
			AutoconfThread(std::string autoconf, CalxAutoconfDialog *waitDialog)
				: wxThread::wxThread(wxTHREAD_DETACHED) {
				this->autoconf = autoconf;
				this->waitDialog = waitDialog;
			}
		protected:
			virtual ExitCode Entry() {
				std::ifstream is(autoconf);
				wxGetApp().getSystemManager()->getRequestResolver()->execute(&is, waitDialog);
				is.close();
				waitDialog->Close(true);
				return nullptr;
			}
		private:
			std::string autoconf;
			CalxAutoconfDialog *waitDialog;
	};
	
	bool CalxApp::OnInit() {	
	
		std::ifstream cnf("config.ini");
		ConfigManager *conf = ConfigManager::load(&cnf, &std::cout);
		cnf.close();		
		
		this->debug_mode = conf->getEntry("ui")->getBool("debug", false);
		std::string lib_addr = conf->getEntry("ui")->getString("devicelib", STRINGIZE(DEVICES_LIB));
		
		this->dynlib = new wxDynamicLibrary(wxDynamicLibrary::CanonicalizeName(lib_addr), wxDL_DEFAULT | wxDL_QUIET);
		if (!dynlib->IsLoaded()) {
			wxMessageBox(__("Device API plugin not found\nSpecify library location"), __("Warning"), wxOK | wxICON_WARNING);
			loadDevicesPlugin();
		}
		if (!dynlib->IsLoaded()) {
			wxMessageBox(__("Dynamic library not found"), __("Error"), wxOK | wxICON_ERROR);
			return false;
		}
		
		bool suc;
		void *raw_getter = dynlib->GetSymbol("getDeviceManager", &suc);
		DeviceManager_getter getter = *((DeviceManager_getter*) &raw_getter);
		if (!suc) {
			wxMessageBox(__("Dynamic library is corrupt"), __("Error"), wxOK | wxICON_ERROR);
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
		#define SETUP_LOG(name, id, dest) {\
		this->name = nullptr;\
		std::string logger = conf->getEntry("log")->getString(id, "");\
		if (logger.compare("stdout") == 0) {\
			SET_LOGGER(dest, &std::cout);\
		} else if (logger.length() > 0) {\
			this->name = new std::ofstream(logger);\
			SET_LOGGER(dest, this->name);\
		}}
	
		SETUP_LOG(errors_log, "errors", ERRORS)
		SETUP_LOG(warnings_log, "warnings", WARNINGS)
		SETUP_LOG(debug_log, "debug", DEBUG)
		SETUP_LOG(info_log, "info", INFO)
		SETUP_LOG(resources_log, "resources", RESOURCES)
		SETUP_LOG(instruments_log, "instruments", INSTRUMENTS)

		#undef SETUP_LOG
	
		this->devman = getter();
		this->sysman = new SystemManager(this->devman, conf);
		this->error_handler = new CalxErrorHandler(this->sysman);
		
		if (this->debug_mode && conf->getEntry("ui")->getBool("console", false)) {
			this->debug_console = new CalxDebugConsole(this->sysman);
			this->debug_console->Run();
		} else {
			this->debug_console = nullptr;
		}
		
		this->frame = new CalxFrame(__("CalX UI"));
		this->frame->Show(true);
		this->frame->Maximize(true);
		
		std::string autoconf = conf->getEntry("core")->getString("autoconf", "");
		if (!autoconf.empty()) {
			this->frame->Enable(false);
			CalxAutoconfDialog *waitDialog = new CalxAutoconfDialog(this->frame, wxID_ANY);
			waitDialog->Show(true);
			AutoconfThread *th = new AutoconfThread(autoconf, waitDialog);
			if (th->Run() != wxTHREAD_NO_ERROR) {
				delete th;
				waitDialog->Close(true);
			}
		}
		
		return true;
	}
	
	int CalxApp::OnExit() {
		if (this->debug_console != nullptr) {
			this->debug_console->Kill();
		}
		
		delete this->error_handler;
		delete this->sysman;
		delete this->devman;
		delete this->dynlib;
		
		
		#ifdef OS_WIN
		if (this->debug_mode) {
			FreeConsole();
		}
		#endif

		if (this->resources_log != nullptr) {
			this->resources_log->close();
			delete this->resources_log;
		}
		return 0;
	}
	
	SystemManager *CalxApp::getSystemManager() {
		return this->sysman;
	}
	
	CalxErrorHandler *CalxApp::getErrorHandler() {
		return this->error_handler;
	}
	
	CalxFrame *CalxApp::getMainFrame() {
		return this->frame;
	}
	
	void CalxApp::loadDevicesPlugin() {
		wxFileDialog openDialog(nullptr, __("Load devices plugin"));
		if (openDialog.ShowModal() == wxID_CANCEL) {
			return;
		}
		dynlib->Load(openDialog.GetPath(), wxDL_DEFAULT | wxDL_QUIET);
	}
}

wxIMPLEMENT_APP(CalXUI::CalxApp);