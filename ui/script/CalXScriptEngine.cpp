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

#include "ui/script/CalXScriptEngine.h"

namespace CalXUI {

	CalXScriptHookThread::CalXScriptHookThread(std::string path, std::string hook)
	    : wxThread::wxThread(wxTHREAD_DETACHED), path(path), hook(hook) {}

	wxThread::ExitCode CalXScriptHookThread::Entry() {
		std::unique_ptr<CalXScript> scr = wxGetApp().loadScript(this->path);
		scr->call(this->hook);
		return nullptr;
	}

	CalXAppScriptUI::CalXAppScriptUI(CalxApp &app) : app(app) {}

	ErrorCode CalXAppScriptUI::openWatcher(std::size_t id) {
		CalxPlaneHandle *handle =
		    this->app.getMainFrame()->getPlaneList()->getPlaneHandle(id);
		if (handle == nullptr) {
			return ErrorCode::UnknownResource;
		} else {
			handle->newWatcher();
			return ErrorCode::NoError;
		}
	}

	CalXAppScriptEnvironment::CalXAppScriptEnvironment(CalxApp &app)
	    : CalXScriptUIEnvironment::CalXScriptUIEnvironment(
	          app.getSystemManager()->getConfiguration(),
	          app.getSystemManager()->getSettingsRepository()),
	      app(app),
	      devices(app),
	      planes(app),
	      ui(app) {}

	CalXScriptDevices &CalXAppScriptEnvironment::getDevices() {
		return this->devices;
	}

	CalXScriptPlanes &CalXAppScriptEnvironment::getPlanes() {
		return this->planes;
	}

	CalXScriptUI &CalXAppScriptEnvironment::getUI() {
		return this->ui;
	}

}  // namespace CalXUI
