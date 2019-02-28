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

#include "calx/ctrl-lib/SystemManager.h"
#include "calx/ctrl-lib/gcode/GCodeInterpreter.h"
#include "calx/ctrl-lib/task/CoordTask.h"
#include "calx/ctrl-lib/logger/Shortcuts.h"
#include "gcodelib/Frontend.h"
#include "gcodelib/runtime/Error.h"

namespace gcl = GCodeLib;

namespace CalX {

	GCodeCoordTask::GCodeCoordTask(std::istream &input,
	                               std::shared_ptr<CoordTranslator> trans,
																 ConfigurationCatalogue &config)
	    : CoordTask::CoordTask(CoordTaskType::GCodeTask) {
		int ch;
		std::stringstream ss;
		while ((ch = input.get()) != EOF) {
			ss << (char) ch;
		}
		this->code = ss.str();
		ss.seekg(0);
		std::unique_ptr<gcl::GCodeCompilerFrontend> compiler;
		if (config.getEntry(CalxConfiguration::Core)->getString(CalxCoreConfiguration::GCodeEngine, "reprap").compare("linuxcnc") == 0) {
			compiler = std::make_unique<gcl::GCodeLinuxCNC>();
		} else {
			compiler = std::make_unique<gcl::GCodeRepRap>();
		}
		try {
			this->module = compiler->compile(ss);
		} catch (gcl::Parser::GCodeParseException &ex) {
			if (ex.getLocation().has_value()) {
				Error(GlobalLogger::getLogger()) << "G-Code compilation error at " << ex.getLocation().value() << " \"" << ex.getMessage() << '\"' << Flush();
			} else {
				Error(GlobalLogger::getLogger()) << "G-Code compilation error \"" << ex.getMessage() << '\"' << Flush();
			}
		} catch (...) {}
		this->translator = trans;
	}

	ErrorCode GCodeCoordTask::perform(std::shared_ptr<CoordPlane> plane,
	                                  TaskParameters &prms, SystemManager &sysman,
	                                  std::shared_ptr<TaskState> state) {
		state->plane = plane;
		state->work = true;
		ErrorCode errcode = ErrorCode::LowLevelError;
		if (this->module) {
			try {
				GCodeInterpreter interpreter(*this->module, *plane, this->translator, sysman.getConfiguration(), prms.speed, *state);
				errcode = interpreter.run();
			} catch (gcl::Runtime::GCodeRuntimeError &ex) {
				if (ex.getLocation().has_value()) {
					Error(GlobalLogger::getLogger()) << "G-Code runtime error at " << ex.getLocation().value() << " \"" << ex.getMessage() << '\"' << Flush();
				} else {
					Error(GlobalLogger::getLogger()) << "G-Code runtime error \"" << ex.getMessage() << '\"' << Flush();
				}
			} catch (...) {}
		}
		state->work = false;
		return errcode;
	}

	const std::string &GCodeCoordTask::getGCode() const {
		return this->code;
	}

	std::shared_ptr<CoordTranslator> GCodeCoordTask::getTranslator() const {
		return this->translator;
	}
}  // namespace CalX
