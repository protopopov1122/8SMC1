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
#include "gcodelib/parser/Parser.h"
#include "gcodelib/runtime/Translator.h"

namespace CalX {

	GCodeCoordTask::GCodeCoordTask(std::istream &input,
	                               std::shared_ptr<CoordTranslator> trans)
	    : CoordTask::CoordTask(CoordTaskType::GCodeTask) {
		int ch;
		std::stringstream ss;
		while ((ch = input.get()) != EOF) {
			ss << (char) ch;
		}
		this->code = ss.str();
		ss.seekg(0);
		gcl::GCodeDefaultScanner scanner(ss);
		gcl::GCodeParser parser(scanner);
		auto root = parser.parse();
		gcl::GCodeIRTranslator translator;
		this->module = translator.translate(*root);
		this->translator = trans;
	}

	ErrorCode GCodeCoordTask::perform(std::shared_ptr<CoordPlane> plane,
	                                  TaskParameters &prms, SystemManager &sysman,
	                                  std::shared_ptr<TaskState> state) {
		state->plane = plane;
		state->work = true;
		GCodeInterpreter interpreter(*this->module, *plane, this->translator, sysman.getConfiguration(), prms.speed, *state);
		ErrorCode errcode = interpreter.run();
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
