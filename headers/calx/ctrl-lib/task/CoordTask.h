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

#ifndef CALX_CTRL_LIB_TASK_COORD_TASK_H_
#define CALX_CTRL_LIB_TASK_COORD_TASK_H_

#include "calx/ctrl-lib/graph/MathEngine.h"
#include "calx/ctrl-lib/misc/GraphBuilder.h"
#include "calx/ctrl-lib/task/AbstractCoordTask.h"
#include "calx/ctrl-lib/task/CoordTaskStep.h"
#include "gcodelib/runtime/IR.h"

/* Coordinate task is abstraction that perform actions on coordiate plane
   according to implemenation. These are few implemenations available:
           * ProgrammedCoordTask - task sequention that stores list of tasks to
   execute.
                        (it also takes responsibility to free memory).
           * GraphCoordTask - task that uses saves graph builder and
                        parameters for it.
           * CoordTaskWrapper - wraps task and plane filter parameters. May be
   removed.*/

namespace gclr = GCodeLib::Runtime;

namespace CalX {

	class LinearCoordTask : public CoordTask {
	 public:
		LinearCoordTask(motor_rect_t, motor_coord_t, bool = true);
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;

		motor_rect_t getRectangle() const;
		void setRectangle(motor_rect_t);
		motor_coord_t getSpacing() const;
		void setSpacing(motor_coord_t);
		bool isVertical() const;
		void setVertical(bool);

	 private:
		motor_rect_t square;
		motor_coord_t spacing;
		bool vertical;
	};

	class ProgrammedCoordTask : public CoordTask {
	 public:
		ProgrammedCoordTask();
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;
		void addStep(std::shared_ptr<TaskStep>);
		std::size_t getSubCount() const;
		bool removeStep(std::size_t);
		std::shared_ptr<TaskStep> pollStep(std::size_t);
		bool insertStep(std::size_t, std::shared_ptr<TaskStep>);

	 private:
		std::vector<std::shared_ptr<TaskStep>> list;
	};

	class GraphCoordTask : public CoordTask {
	 public:
		GraphCoordTask(std::unique_ptr<GraphBuilder>,
		               std::shared_ptr<CoordTranslator>, float);
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;

	 private:
		std::unique_ptr<GraphBuilder> graph;
		std::shared_ptr<CoordTranslator> trans;
		float scale;
	};

	class GCodeCoordTask : public CoordTask {
	 public:
		GCodeCoordTask(std::istream &, std::shared_ptr<CoordTranslator>);
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;

		const std::string &getGCode() const;
		std::shared_ptr<CoordTranslator> getTranslator() const;

	 private:
		std::string code;
		std::unique_ptr<gclr::GCodeIRModule> module;
		std::shared_ptr<CoordTranslator> translator;
	};
}  // namespace CalX

#endif
