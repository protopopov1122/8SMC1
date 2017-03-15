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
#include "device/DeviceManager.h"

namespace CalXUI {
	
	bool CalxApp::OnInit() {
		this->dynlib = new wxDynamicLibrary(wxDynamicLibrary::CanonicalizeName("dev_8smc1"));
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
		this->devman = getter();
		this->sysman = new SystemManager(this->devman);
		this->frame = new CalxFrame("CalX UI");
		this->frame->Show(true);
		this->frame->Maximize(true);
		
		return true;
	}
	
	int CalxApp::OnExit() {
		delete this->sysman;
		delete this->devman;
		delete this->dynlib;
		return 0;
	}
	
	SystemManager *CalxApp::getSystemManager() {
		return this->sysman;
	}
	
	CalxFrame *CalxApp::getMainFrame() {
		return this->frame;
	}
}

wxIMPLEMENT_APP(CalXUI::CalxApp);