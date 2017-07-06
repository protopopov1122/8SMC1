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

#ifndef CALX_CTRL_LIB_TASK_ABSTRACT_COORD_TASK_H_
#define CALX_CTRL_LIB_TASK_ABSTRACT_COORD_TASK_H_

#include <vector>
#include <iostream>
#include <sstream>
#include "ctrl-lib/plane/CoordPlane.h"
#include "ctrl-lib/translator/CoordTranslator.h"

namespace CalX {

  class SystemManager;  // Forward referencing

  struct TaskParameters {
	float speed;
  };

  class TaskStep {
   public:
	virtual ~TaskStep(){};
	virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*,
							  TaskState*) = 0;
  };

  enum class CoordTaskType {
	ProgrammedTask,
	GraphTask,
	WrapperTask,
	GCodeTask,
	LinearTask
  };

  class CoordTask : public TaskStep {
   public:
	CoordTask(CoordTaskType tp) {
	  this->type = tp;
	}
	virtual ~CoordTask() {}
	CoordTaskType getType() {
	  return this->type;
	}
	virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*,
							  TaskState*) = 0;

   private:
	CoordTaskType type;
  };
}

#endif
