/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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
#include <iosfwd>
#include <wx/dynlib.h>
#include <wx/intl.h>

#ifndef DEVICES_LIB
#define DEVICES_LIB dev_standard
#endif

// For internatization purposes
#define __(String) (wxGetTranslation(String).ToStdString().c_str())

#define FORMAT(str, ...) (wxString::Format(str, __VA_ARGS__).ToStdString())

namespace CalXUI {

	class CalxDebugConsole;  // Forward referencing
	class CalxErrorHandler;  // Forward referencing

	wxDECLARE_EVENT(wxEVT_APP_ERROR, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_APP_ALERT, wxThreadEvent);

	class CalxApp : public wxApp {
	 public:
		virtual bool OnInit();
		virtual int OnExit();
		virtual void OnUnhandledException();
		virtual void OnFatalException();
		void OnErrorEvent(wxThreadEvent &);
		void OnAlertEvent(wxThreadEvent &);
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

		std::unique_ptr<ConfigurationCatalogue> loadConfiguration();
		DeviceManager_getter loadDeviceDrivers(ConfigurationCatalogue &);
		void initDebugConsole(ConfigurationCatalogue &);
		void startDebugConsole(ConfigurationCatalogue &);
		void initLogging(ConfigurationCatalogue &);
		std::unique_ptr<ExtEngine> loadExtensionEngine(ConfigurationCatalogue &);
		void loadScriptEngine(ConfigurationCatalogue &);
		void startInitScript(ConfigurationCatalogue &);

		CalxFrame *frame;
		wxDynamicLibrary *dynlib;
		wxDynamicLibrary *extLib;
		wxDynamicLibrary *scriptLib;
		std::unique_ptr<SystemManager> sysman;
		std::unique_ptr<CalxUnitProcessor> unit_processor;

		bool debug_mode;
		std::unique_ptr<CalxDebugConsole> debug_console;
		CalxErrorHandler *error_handler;

		std::shared_ptr<CalXScriptEnvironment> script_env;
		std::shared_ptr<CalXScriptFactory> scriptFactory;
	};
}  // namespace CalXUI
wxDECLARE_APP(CalXUI::CalxApp);

#endif
