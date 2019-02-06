/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "calx/ui/script/CalXScriptEngine.h"

namespace CalX::UI {

	CalXScriptHookThread::CalXScriptHookThread(const std::string &path,
	                                           const std::string &hook)
	    : wxThread::wxThread(wxTHREAD_DETACHED), path(path), hook(hook) {}

	wxThread::ExitCode CalXScriptHookThread::Entry() {
		std::unique_ptr<CalXScript> scr = wxGetApp().loadScript(this->path);
		scr->invokeFunction(this->hook);
		return nullptr;
	}

	CalXAppScriptUIMath::CalXAppScriptUIMath(CalxApp &app) : app(app) {}

	std::size_t CalXAppScriptUIMath::getFormulaCount() {
		return this->app.getMainFrame()->getMathEngine()->getFormulaCount();
	}

	void CalXAppScriptUIMath::addFormula(
	    const std::string &title, const std::string &formula,
	    const std::map<std::string, std::string> &variables) {
		this->app.getMainFrame()->getMathEngine()->addFormula(title, formula,
		                                                      variables);
	}

	bool CalXAppScriptUIMath::removeFormula(std::size_t index) {
		return this->app.getMainFrame()->getMathEngine()->removeFormula(index);
	}

	CalXAppScriptUI::CalXAppScriptUI(CalxApp &app) : app(app), math(app) {}

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

	CalXScriptUIMath &CalXAppScriptUI::getMath() {
		return this->math;
	}

	CalXAppScriptEnvironment::CalXAppScriptEnvironment(CalxApp &app)
	    : CalXScriptUIEnvironment::CalXScriptUIEnvironment(
	          app.getSystemManager().getConfiguration(),
	          app.getSystemManager().getSettingsRepository()),
	      app(app),
	      devices(app),
	      planes(app),
	      tasks(app),
	      ui(app) {}

	CalXScriptDevices &CalXAppScriptEnvironment::getDevices() {
		return this->devices;
	}

	CalXScriptPlanes &CalXAppScriptEnvironment::getPlanes() {
		return this->planes;
	}

	CalXScriptTasks &CalXAppScriptEnvironment::getTasks() {
		return this->tasks;
	}

	JournalLogger &CalXAppScriptEnvironment::getLogger() {
		return GlobalLogger::getLogger();
	}

	JournalLogger &CalXAppScriptEnvironment::getJournal() {
		return this->app.getJournal();
	}

	CalXScriptUI &CalXAppScriptEnvironment::getUI() {
		return this->ui;
	}

}  // namespace CalX::UI
