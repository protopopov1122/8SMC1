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

#ifndef CALX_CTRL_LIB_TASK_ABSTRACT_COORD_TASK_H_
#define CALX_CTRL_LIB_TASK_ABSTRACT_COORD_TASK_H_

#include "ctrl-lib/plane/CoordPlane.h"
#include "ctrl-lib/translator/CoordTranslator.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

/* Task is complex action which as can be performed on any coordinate plane.
   Usually task consists of a set of elementar actions, their speed is
   represented as coefficient(from 0 to 1).
*/

namespace CalX {

	class SystemManager;  // Forward referencing

	struct TaskParameters {
		float speed;  // Task build base speed
		// It is multiplied by task speed coeficients to get final speed
	};

	class TaskStep {
	 public:
		virtual ~TaskStep() = default;
		virtual ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                          SystemManager *, std::shared_ptr<TaskState>) = 0;
	};

	enum class CoordTaskType {
		ProgrammedTask,  // Sequence of elementar actions on plane
		GraphTask,       // Evaluates given function graph and builds it
		LinearTask,      // Builds sequence of lines on a given rectangle
		GCodeTask        // Executes GCode on given coordinate plane
	};

	class CoordTask : public TaskStep {
	 public:
		CoordTask(CoordTaskType tp) : type(tp) {}
		virtual ~CoordTask() = default;
		CoordTaskType getType() const {
			return this->type;
		}
		virtual ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                          SystemManager *, std::shared_ptr<TaskState>) = 0;
		std::pair<motor_point_t, bool> getStartPoint(motor_point_t, motor_rect_t,
		                                             SystemManager *);

	 private:
		CoordTaskType type;
	};
}  // namespace CalX

#endif
