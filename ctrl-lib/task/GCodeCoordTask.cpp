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


#include "CoordTask.h"
#include "ctrl-lib/misc/GCodeParser.h"

namespace CalX {
	
	GCodeCoordTask::GCodeCoordTask(std::istream *input, CoordTranslator *trans)
		: CoordTask::CoordTask(CoordTaskType::GCodeTask) {
		int ch;
		std::stringstream ss;
		while ((ch = input->get()) != EOF) {
			ss << (char) ch;
		}
		this->code = ss.str();
		this->translator = trans;
	}
	
	GCodeCoordTask::~GCodeCoordTask() {
		delete this->translator;
	}
	
	ErrorCode GCodeCoordTask::perform(CoordPlane *plane, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		std::stringstream ss(this->code);
		GCodeLexer lexer(&ss);
		GCodeParser parser(&lexer);
		ProgrammedCoordTask task;
		gcode_translate(&parser, this->translator, &task);
		
		return task.perform(plane, prms, sysman, state);
	}
	
	std::string GCodeCoordTask::getGCode() {
		return this->code;
	}
	
	CoordTranslator *GCodeCoordTask::getTranslator() {
		return this->translator;
	}
}