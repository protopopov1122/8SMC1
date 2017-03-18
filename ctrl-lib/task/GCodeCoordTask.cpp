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
		while ((ch = input->get()) != EOF) {
			code << (char) ch;
		}
		this->translator = trans;
	}
	
	GCodeCoordTask::~GCodeCoordTask() {
		delete this->translator;
	}
	
	ErrorCode GCodeCoordTask::perform(CoordPlane *plane, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		this->code.seekg(0, this->code.beg);
		GCodeLexer lexer(&this->code);
		GCodeParser parser(&lexer);
		ProgrammedCoordTask task;
		coord_point_t last = {0, 0};
		GCodeCommand *cmd = nullptr;
		const int CHORD_COUNT = 100;
		while ((cmd = parser.nextCommand()) != nullptr) {
			
			if (cmd->getLetter() == 'G') {
				long double x = cmd->hasArg('X') ? cmd->getArg('X').fractValue() : last.x;
				long double y = cmd->hasArg('Y') ? cmd->getArg('Y').fractValue() : last.y;
				motor_point_t real = translator->get(x, y);
				switch (cmd->getCommand()) {
					case GCodeOpcode::GCode_Jump:
						task.addStep(new JumpTaskStep(real, 1.0f));
					break;
					case GCodeOpcode::GCode_Move:
						task.addStep(new MoveTaskStep(real, 1.0f));
					break;
					case GCodeOpcode::GCode_Clockwise_Arc: {
						long double i = cmd->getArg('I').fractValue();
						long double j = cmd->getArg('J').fractValue();
						motor_point_t center = translator->get(i, j);
						RelArcTaskStep *step = new RelArcTaskStep(real, center, CHORD_COUNT, 1.0f);
						step->setClockwise(true);
						task.addStep(step);
					} break;
					case GCodeOpcode::GCode_CounterClockwise_Arc: {
						long double i = cmd->getArg('I').fractValue();
						long double j = cmd->getArg('J').fractValue();
						motor_point_t center = translator->get(i, j);
						RelArcTaskStep *step = new RelArcTaskStep(real, center, CHORD_COUNT, 1.0f);
						step->setClockwise(false);
						task.addStep(step);
					} break;
				}
				last = {x, y};
			}
			
			delete cmd;
		}
		
		return task.perform(plane, prms, sysman, state);
	}
	
	std::string GCodeCoordTask::getGCode() {
		return this->code.str();
	}
	
	CoordTranslator *GCodeCoordTask::getTranslator() {
		return this->translator;
	}
}