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

#ifndef CALX_UI_CALX_APP_H_
#define CALX_UI_CALX_APP_H_

#include "ui/CalxFrame.h"
#include "ui/UIExtEngine.h"
#include "ctrl-lib/script/ScriptEngine.h"
#include "ui/CalxUnitProcessor.h"
#include "ui/calx.h"
#include <fstream>
#include <iostream>
#include <wx/dynlib.h>
#include <wx/intl.h>

#ifndef DEVICES_LIB
#define DEVICES_LIB dev_standart
#endif

// For internatization purposes
#define __(String) (wxGetTranslation(String).ToStdString().c_str())

#define FORMAT(str, ...) (wxString::Format(str, __VA_ARGS__).ToStdString())

namespace CalXUI {

	class CalxDebugConsole;  // Forward referencing
	class CalxErrorHandler;  // Forward referencing

	wxDECLARE_EVENT(wxEVT_APP_ERROR, wxThreadEvent);

	class CalxApp : public wxApp {
	 public:
		virtual bool OnInit();
		virtual int OnExit();
		virtual void OnUnhandledException();
		virtual void OnFatalException();
		void OnErrorEvent(wxThreadEvent &);
		SystemManager *getSystemManager();
		CalxErrorHandler *getErrorHandler();
		CalxFrame *getMainFrame();

		bool hasScriptEngine();
		std::shared_ptr<CalXScriptEnvironment> getScriptEnvironment();
		std::shared_ptr<CalXScriptFactory> getScriptFactory();
		std::unique_ptr<CalXScript> loadScript(std::string);

		CalxUnitProcessor &getUnitProcessor();

	 private:
		void loadDevicesPlugin();

		std::unique_ptr<ConfigManager> loadConfiguration();
		DeviceManager_getter loadDeviceDrivers(ConfigManager &);
		void initDebugConsole(ConfigManager &);
		void startDebugConsole(ConfigManager &);
		void initLogging(ConfigManager &);
		std::unique_ptr<ExtEngine> loadExtensionEngine(ConfigManager &);
		void loadScriptEngine(ConfigManager &);
		void startInitScript(ConfigManager &);

		CalxFrame *frame;
		wxDynamicLibrary *dynlib;
		wxDynamicLibrary *extLib;
		wxDynamicLibrary *scriptLib;
		std::unique_ptr<SystemManager> sysman;
		std::unique_ptr<CalxUnitProcessor> unit_processor;

		bool debug_mode;
		CalxDebugConsole *debug_console;
		CalxErrorHandler *error_handler;

		std::shared_ptr<CalXScriptEnvironment> script_env;
		std::shared_ptr<CalXScriptFactory> scriptFactory;

		std::ofstream *errors_log;
		std::ofstream *warnings_log;
		std::ofstream *debug_log;
		std::ofstream *info_log;
		std::ofstream *resources_log;
		std::ofstream *instruments_log;
	};
}  // namespace CalXUI
wxDECLARE_APP(CalXUI::CalxApp);

#endif
