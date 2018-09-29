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

#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/gcode/GCodeInterpreter.h"
#include "ctrl-lib/task/CoordTask.h"

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
		this->stream = std::make_shared<GCodeStream>(ss);
		this->translator = trans;
	}

	ErrorCode GCodeCoordTask::perform(std::shared_ptr<CoordPlane> plane,
	                                  TaskParameters &prms, SystemManager *sysman,
	                                  std::shared_ptr<TaskState> state) {
		state->plane = plane;
		state->work = true;
		ErrorCode errcode = GCodeInterpreter::execute(
		    *this->stream.get(), *plane.get(), this->translator,
		    sysman->getConfiguration(), prms.speed, *state.get());
		state->work = false;
		return errcode;
	}

	std::string GCodeCoordTask::getGCode() {
		return this->code;
	}

	std::shared_ptr<CoordTranslator> GCodeCoordTask::getTranslator() {
		return this->translator;
	}
}  // namespace CalX
