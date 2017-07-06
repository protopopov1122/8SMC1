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

#include "ctrl-lib/task/CoordTask.h"
#include "ctrl-lib/gcode/GCodeInterpreter.h"
#include "ctrl-lib/SystemManager.h"

namespace CalX {

  GCodeCoordTask::GCodeCoordTask(std::istream *input, CoordTranslator *trans)
	  : CoordTask::CoordTask(CoordTaskType::GCodeTask) {
	int ch;
	std::stringstream ss;
	while ((ch = input->get()) != EOF) {
	  ss << (char) ch;
	}
	this->code = ss.str();
	ss.seekg(0);
	this->stream = new GCodeStream(ss);
	this->translator = trans;
	INIT_LOG("GCodeCoordTask");
  }

  GCodeCoordTask::~GCodeCoordTask() {
	delete this->stream;
	delete this->translator;
	DESTROY_LOG("GCodeCoordTask");
  }

  ErrorCode GCodeCoordTask::perform(CoordPlane *plane, TaskParameters &prms,
									SystemManager *sysman, TaskState *state) {
	state->plane = plane;
	state->work = true;
	ErrorCode errcode = GCodeInterpreter::execute(
		this->stream, plane, this->translator, sysman->getConfiguration(),
		prms.speed, state);
	state->work = false;
	return errcode;
  }

  std::string GCodeCoordTask::getGCode() {
	return this->code;
  }

  CoordTranslator *GCodeCoordTask::getTranslator() {
	return this->translator;
  }
}
