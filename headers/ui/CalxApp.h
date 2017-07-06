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

#ifndef CALX_UI_CALX_APP_H_
#define CALX_UI_CALX_APP_H_

#include <iostream>
#include <fstream>
#include "ui/calx.h"
#include <wx/dynlib.h>
#include <wx/intl.h>
#include "ui/CalxFrame.h"
#include "ui/UIExtEngine.h"

using namespace CalX;

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
  wxDECLARE_EVENT(wxEVT_APP_AUTOCONF, wxThreadEvent);

  class CalxApp : public wxApp {
   public:
	virtual bool OnInit();
	virtual int OnExit();
	virtual void OnUnhandledException();
	virtual void OnFatalException();
	void OnErrorEvent(wxThreadEvent &);
	void OnAutoconfEvent(wxThreadEvent &);
	SystemManager *getSystemManager();
	CalxErrorHandler *getErrorHandler();
	CalxFrame *getMainFrame();

	std::string formatDouble(double);
	std::string getUnits();
	std::string getSpeedUnits();
	double getUnitPrecision();
	double getSpeedPrecision();
	double getUnitScale();
	double getSpeedScale();
	coord_point_t getUnitOffset();

   private:
	void loadDevicesPlugin();

	CalxFrame *frame;
	wxDynamicLibrary *dynlib;
	wxDynamicLibrary *extLib;
	DeviceManager *devman;
	SystemManager *sysman;

	bool debug_mode;
	CalxDebugConsole *debug_console;
	CalxErrorHandler *error_handler;

	std::ofstream *errors_log;
	std::ofstream *warnings_log;
	std::ofstream *debug_log;
	std::ofstream *info_log;
	std::ofstream *resources_log;
	std::ofstream *instruments_log;
  };
}
wxDECLARE_APP(CalXUI::CalxApp);

#endif
