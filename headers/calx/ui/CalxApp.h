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

#ifndef CALX_UI_CALX_APP_H_
#define CALX_UI_CALX_APP_H_

#include "calx/ui/CalxFrame.h"
#include "calx/ui/UIExtEngine.h"
#include "calx/ui/script/ScriptEngine.h"
#include "calx/ui/CalxUnitProcessor.h"
#include "calx/ui/calx.h"
#include "calx/ui/CalxJournalManager.h"
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

namespace CalX::UI {

	class CalXScriptUIEnvironment;  // Forward referencing
	class CalxDebugConsole;         // Forward referencing
	class CalxErrorHandler;         // Forward referencing

	wxDECLARE_EVENT(wxEVT_APP_ERROR, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_APP_ALERT, wxThreadEvent);

	class CalxApp : public wxApp {
	 public:
		bool OnInit() override;
		int OnExit() override;
		void OnUnhandledException() override;
		void OnFatalException() override;
		void OnErrorEvent(wxThreadEvent &);
		void OnAlertEvent(wxThreadEvent &);
		SystemManager &getSystemManager();
		CalxErrorHandler *getErrorHandler();
		CalxFrame *getMainFrame();

		bool hasScriptEngine();
		std::shared_ptr<CalXScriptUIEnvironment> getScriptEnvironment();
		std::shared_ptr<CalXScriptUIFactory> getScriptFactory();
		std::unique_ptr<CalXScript> loadScript(const std::string &);
		JournalLogger &getJournal();

		CalxUnitProcessor &getUnitProcessor();

	 private:
		void loadDevicesPlugin();

		std::unique_ptr<ConfigurationCatalogue> loadConfiguration();
		DeviceManager_getter loadDeviceDrivers(ConfigurationCatalogue &);
		void initDebugConsole(ConfigurationCatalogue &);
		void startDebugConsole(ConfigurationCatalogue &);
		void initLogging(ConfigurationCatalogue &);
		void updateLogging(ConfigurationCatalogue &);
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

		std::shared_ptr<CalXScriptUIEnvironment> script_env;
		std::shared_ptr<CalXScriptUIFactory> scriptFactory;
		std::unique_ptr<CalxJournalManager> journalManager;
	};
}  // namespace CalX::UI
wxDECLARE_APP(CalX::UI::CalxApp);

#endif
