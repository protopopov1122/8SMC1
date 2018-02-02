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

#include "ui/CalxApp.h"
#ifdef OS_WIN
#include <windows.h>
#endif
#include "ctrl-lib/device/DeviceManager.h"
#include "ui/CalxConfigLoader.h"
#include "ui/CalxDebugConsole.h"
#include "ui/CalxErrorHandler.h"
#include "ui/coord/CalxCoordPanel.h"
#include "ui/script/CalXScriptEngine.h"
#include <wx/filedlg.h>

#include "ctrl-lib/SignalHandler.h"
#include "ctrl-lib/conf/ConfigManager.h"
#include "ctrl-lib/conf/ConfigValidator.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

namespace CalXUI {

	wxDEFINE_EVENT(wxEVT_APP_ERROR, wxThreadEvent);

	// Utility startup methods
	// System startup is quite complex action
	// so it is split into several methods
	std::unique_ptr<ConfigManager> CalxApp::loadConfiguration() {
		// Determine which configuration to use
		CalxConfigLoader *confLoader = new CalxConfigLoader(nullptr, wxID_ANY);
		confLoader->load();
		if (confLoader->isExiting()) {
			confLoader->Destroy();
			return nullptr;
		}
		std::string path = confLoader->getFileName();
		confLoader->Destroy();

		// Load selected configuration
		std::unique_ptr<ConfigManager> conf_ptr = nullptr;
		std::ifstream cnf(path);
		if (!cnf.good()) {
			wxMessageBox(__("Can't open configuration. Using default values."),
			             __("Warning"), wxICON_WARNING);
			conf_ptr = std::make_unique<ConfigManager>();
		} else {
			conf_ptr = ConfigManager::load(cnf, std::cout);
		}
		cnf.close();
		return conf_ptr;
	}

	DeviceManager_getter CalxApp::loadDeviceDrivers(ConfigManager &conf) {
		// Load specified dynamic library
		std::string lib_addr =
			conf.getEntry("ui")->getString("devicelib", STRINGIZE(DEVICES_LIB));

		this->dynlib =
		    new wxDynamicLibrary(wxDynamicLibrary::CanonicalizeName(lib_addr),
		                         wxDL_DEFAULT | wxDL_QUIET);
		if (!dynlib->IsLoaded()) {
			wxMessageBox(__("Device API plugin not found\nSpecify library location"),
			             __("Warning"), wxOK | wxICON_WARNING);
			loadDevicesPlugin();
		}
		if (!dynlib->IsLoaded()) {
			wxMessageBox(__("Dynamic library not found"), __("Error"),
			             wxOK | wxICON_ERROR);
			return nullptr;
		}

		// 
		bool suc;
		void *raw_getter = dynlib->GetSymbol("getDeviceManager", &suc);
		DeviceManager_getter getter = *((DeviceManager_getter *) &raw_getter);
		if (!suc) {
			wxMessageBox(__("Dynamic library is corrupt"), __("Error"),
			             wxOK | wxICON_ERROR);
			return nullptr;
		}
		return getter;
	}

	void CalxApp::initDebugConsole(ConfigManager &conf) {
		this->debug_mode = conf.getEntry("ui")->getBool("debug", false);
		if (this->debug_mode) {
#ifdef OS_WIN
			AllocConsole();
			AttachConsole(GetCurrentProcessId());
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);
#endif
		}
	}

	void CalxApp::startDebugConsole(ConfigManager &conf) {
		if (this->debug_mode &&
		    sysman->getConfiguration().getEntry("ui")->getBool("console", false)) {
			this->debug_console = new CalxDebugConsole(this->sysman.get());
			this->debug_console->Run();
		} else {
			this->debug_console = nullptr;
		}
		this->Bind(wxEVT_APP_ERROR, &CalxApp::OnErrorEvent, this);
	}

	void CalxApp::initLogging(ConfigManager &conf) {
#define SETUP_LOG(name, id, dest)                                              \
	{                                                                            \
		this->name = nullptr;                                                      \
		std::string logger = conf.getEntry("log")->getString(id, "");              \
		if (logger.compare("stdout") == 0) {                                       \
			SET_LOGGER(dest, &std::cout);                                            \
		} else if (logger.length() > 0) {                                          \
			this->name = new std::ofstream(logger);                                  \
			SET_LOGGER(dest, this->name);                                            \
		}                                                                          \
	}

		SETUP_LOG(errors_log, "errors", ERRORS)
		SETUP_LOG(warnings_log, "warnings", WARNINGS)
		SETUP_LOG(debug_log, "debug", DEBUG)
		SETUP_LOG(info_log, "info", INFO)
		SETUP_LOG(resources_log, "resources", RESOURCES)
		SETUP_LOG(instruments_log, "instruments", INSTRUMENTS)

#undef SETUP_LOG
	}

	std::unique_ptr<ExtEngine> CalxApp::loadExtensionEngine(ConfigManager &conf) {
		std::string ext_addr = conf.getEntry("ext")->getString("engine", "");
		std::unique_ptr<ExtEngine> ext = nullptr;
		if (!ext_addr.empty()) {
			this->extLib =
			    new wxDynamicLibrary(wxDynamicLibrary::CanonicalizeName(ext_addr),
			                         wxDL_DEFAULT | wxDL_QUIET);
			if (!this->extLib->IsLoaded()) {
				wxMessageBox(__("Extension engine can't be loaded"), __("Warning"),
				             wxOK | wxICON_WARNING);
			} else {
				bool ext_suc;
				if (extLib->HasSymbol("getUIExtEngine")) {
					void *ext_raw_getter = extLib->GetSymbol("getUIExtEngine", &ext_suc);
					if (!ext_suc) {
						wxMessageBox(__("Extension engine can't be loaded"), __("Warning"),
						             wxOK | wxICON_WARNING);
					} else {
						UIExtEngine_getter ext_getter =
						    *((UIExtEngine_getter *) &ext_raw_getter);
						UIExtEngine *uiext = ext_getter();
						uiext->uiInit(wxGetApp());
						ext = std::unique_ptr<ExtEngine>(uiext);
					}
				} else {
					void *ext_raw_getter = extLib->GetSymbol("getExtEngine", &ext_suc);
					if (!ext_suc) {
						wxMessageBox(__("Extension engine can't be loaded"), __("Warning"),
						             wxOK | wxICON_WARNING);
					} else {
						ExtEngine_getter ext_getter =
						    *((ExtEngine_getter *) &ext_raw_getter);
						ext = std::unique_ptr<ExtEngine>(ext_getter());
					}
				}
			}
		} else {
			this->extLib = nullptr;
		}
		return ext;
	}

	void CalxApp::loadScriptEngine(ConfigManager &conf) {
		std::string script_eng_addr =
		    conf.getEntry("script")->getString("engine", "");
		this->scriptFactory = nullptr;
		this->script_env = nullptr;
		this->scriptLib = nullptr;
		if (!script_eng_addr.empty()) {
			this->scriptLib =
			    new wxDynamicLibrary(script_eng_addr, wxDL_DEFAULT | wxDL_QUIET);
			if (!this->scriptLib->IsLoaded()) {
				wxMessageBox(__("Scripting engine can't be loaded"), __("Warning"),
				             wxOK | wxICON_WARNING);
			} else {
				bool scr_success;
				if (this->scriptLib->HasSymbol("getScriptEngine")) {
					void *scr_raw_getter =
					    this->scriptLib->GetSymbol("getScriptEngine", &scr_success);
					ScriptEngine_getter scr_getter =
					    *((ScriptEngine_getter *) &scr_raw_getter);
					this->script_env =
					    std::make_unique<CalXAppScriptEnvironment>(wxGetApp());
					this->scriptFactory =
					    std::unique_ptr<CalXScriptFactory>(scr_getter());
				} else {
					wxMessageBox(__("Scripting engine can't be loaded"), __("Warning"),
					             wxOK | wxICON_WARNING);
				}
			}
		}
	}

	void CalxApp::startInitScript(ConfigManager &conf) {
		if (this->scriptFactory != nullptr &&
		    this->sysman->getConfiguration().getEntry("script")->getBool("autoinit",
		                                                                 false)) {
			CalXScriptHookThread *th = new CalXScriptHookThread(
			    this->sysman->getConfiguration().getEntry("script")->getString(
			        "main", "scripts/main.lua"),
			    this->sysman->getConfiguration().getEntry("script")->getString(
			        "init_entry", "init"));
			th->Run();
		}
	}

	// Application entry-point. It performs system startup and
	// calls methods above 
	bool CalxApp::OnInit() {
		// Load configuration
		std::unique_ptr<ConfigManager> conf_ptr = this->loadConfiguration();
		if (conf_ptr == nullptr) {
			return false;
		}
		ConfigManager &conf = *conf_ptr;

		// Init device drivers
		DeviceManager_getter getter = this->loadDeviceDrivers(conf);
		if (getter == nullptr) {
			return false;
		}

		// Open debug console if necessary
		this->initDebugConsole(conf);

		// Init logging subsystem
		this->initLogging(conf);

		// Load extension engine
		std::unique_ptr<ExtEngine> ext = this->loadExtensionEngine(conf);
		
		// Initialize main system structures
		std::unique_ptr<DeviceManager> devman =
		    std::unique_ptr<DeviceManager>(getter());
		this->sysman = std::make_unique<SystemManager>(std::move(devman), std::move(conf_ptr),
		                                 std::move(ext));
		this->error_handler = new CalxErrorHandler(this->sysman.get());

		// Start debug console if necessary
		this->startDebugConsole(conf);

		// Load script engine
		this->loadScriptEngine(conf);

		// Show main program window
		this->frame = new CalxFrame(__("CalX UI"));
		this->frame->Show(true);
		this->frame->Maximize(true);

		// Init signal and SEH handlers which perform emergency shutdown if needed
		setup_signals(this->sysman.get());

		// Start initialization script if it is defined
		this->startInitScript(conf);

		return true;
	}

	bool CalxApp::hasScriptEngine() {
		return this->scriptFactory != nullptr;
	}

	std::shared_ptr<CalXScriptEnvironment> CalxApp::getScriptEnvironment() {
		return this->script_env;
	}

	std::shared_ptr<CalXScriptFactory> CalxApp::getScriptFactory() {
		return this->scriptFactory;
	}

	std::unique_ptr<CalXScript> CalxApp::loadScript(std::string path) {
		if (this->scriptFactory == nullptr) {
			wxMessageBox(FORMAT(__("Scripting engine is not loaded! Script '%s' "
			                       "can\'t be loaded"),
			                    path),
			             __("Warning"), wxOK | wxICON_WARNING);
			return nullptr;
		} else {
			return this->scriptFactory->openFile(*this->script_env, path);
		}
	}

	// This method is called before system shuts down
	// So it frees resources and stops all actions
	int CalxApp::OnExit() {
		if (this->debug_console != nullptr) {
			this->debug_console->Kill();
		}

		delete this->error_handler;
		this->sysman.reset(nullptr);
		this->dynlib->Detach();
		this->dynlib->Unload();
		if (this->extLib != nullptr) {
			this->extLib->Detach();
			this->extLib->Unload();
		}
		if (this->scriptLib != nullptr) {
			this->scriptLib->Detach();
			this->scriptLib->Unload();
		}

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

	void CalxApp::OnUnhandledException() {
		calx_terminate();
		exit(-1);
	}

	void CalxApp::OnFatalException() {
		calx_terminate();
		exit(-1);
	}

	SystemManager *CalxApp::getSystemManager() {
		return this->sysman.get();
	}

	CalxErrorHandler *CalxApp::getErrorHandler() {
		return this->error_handler;
	}

	std::string CalxApp::formatDouble(double d) {
		std::ostringstream os;
		os << d;
		return os.str();
	}

	std::string CalxApp::getUnits() {
		return wxGetApp()
		    .getSystemManager()
		    ->getConfiguration()
		    .getEntry("units")
		    ->getString("unit_suffix", "");
	}

	std::string CalxApp::getSpeedUnits() {
		std::string units = this->getUnits();
		std::string timing = wxGetApp()
		                         .getSystemManager()
		                         ->getConfiguration()
		                         .getEntry("units")
		                         ->getString("timing", "");
		return units.empty() ? "" : units + timing;
	}

	double CalxApp::getUnitPrecision() {
		return 1.0 / this->getUnitScale();
	}

	double CalxApp::getSpeedPrecision() {
		return 1.0 / this->getSpeedScale();
	}

	double CalxApp::getUnitScale() {
		return wxGetApp()
		    .getSystemManager()
		    ->getConfiguration()
		    .getEntry("units")
		    ->getReal("unit_scale", 1.0f);
	}

	double CalxApp::getSpeedScale() {
		return wxGetApp()
		    .getSystemManager()
		    ->getConfiguration()
		    .getEntry("units")
		    ->getReal("speed_scale", 1.0f);
	}

	coord_point_t CalxApp::getUnitOffset() {
		coord_point_t offset = { wxGetApp()
			                           .getSystemManager()
			                           ->getConfiguration()
			                           .getEntry("units")
			                           ->getReal("unit_offset_x", 0.0f),
			                       wxGetApp()
			                           .getSystemManager()
			                           ->getConfiguration()
			                           .getEntry("units")
			                           ->getReal("unit_offset_y", 0.0f) };
		return offset;
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

	void CalxApp::OnErrorEvent(wxThreadEvent &evt) {
		this->error_handler->display(evt.GetPayload<ErrorCode>());
	}
}  // namespace CalXUI

wxIMPLEMENT_APP(CalXUI::CalxApp);
