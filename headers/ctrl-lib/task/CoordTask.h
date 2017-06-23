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


#ifndef CALX_CTRL_LIB_TASK_H_
#define CALX_CTRL_LIB_TASK_H_

#include <vector>
#include <iostream>
#include <sstream>
#include "ctrl-lib/plane/CoordPlane.h"
#include "ctrl-lib/translator/CoordTranslator.h"
#include "ctrl-lib/misc/GraphBuilder.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include "ctrl-lib/gcode/GCodeStream.h"

/* Coordinate task is abstraction that perform actions on coordiate plane
   according to implemenation. These are few implemenations available:
       * ProgrammedCoordTask - task sequention that stores list of tasks to execute.
			(it also takes responsibility to free memory).
	   * GraphCoordTask - task that uses saves graph builder and
			parameters for it.
	   * CoordTaskWrapper - wraps task and plane filter parameters. May be removed.*/

namespace CalX {
	
	class SystemManager; // Forward referencing

	struct TaskParameters {
		float speed;
	};

	class TaskStep {
		public:
			virtual ~TaskStep() {};
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*) = 0;
	};

	class MoveTaskStep : public TaskStep {
		public:
			MoveTaskStep(motor_point_t, float, bool = false);
			virtual ~MoveTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
			
			motor_point_t getPosition();
			float getSpeed();
			bool isRelative();
			
			void setPosition(motor_point_t);
			void setSpeed(float);
			void setRelative(bool);
		private:
			motor_point_t pos;
			float speed_coef;
			bool rel;
	};

	class JumpTaskStep : public TaskStep {
		public:
			JumpTaskStep(motor_point_t, float, bool = false);
			virtual ~JumpTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
			
			motor_point_t getPosition();
			float getSpeed();
			bool isRelative();
			
			void setPosition(motor_point_t);
			void setSpeed(float);
			void setRelative(bool);
		private:
			motor_point_t pos;
			float speed_coef;
			bool rel;
	};

	class CalibrateTaskStep : public TaskStep {
		public:
			CalibrateTaskStep(TrailerId);
			virtual ~CalibrateTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
		private:
			TrailerId side;
	};

	class ArcTaskStep : public TaskStep {
		public:
			ArcTaskStep(motor_point_t, motor_point_t, int, float, bool = false);
			virtual ~ArcTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
			
			motor_point_t getDestination();
			motor_point_t getCenter();
			int getSplitter();
			float getSpeed();
			bool isClockwise();
			bool isRelative();
			
			void setDestination(motor_point_t);
			void setCenter(motor_point_t);
			void setSplitter(int);
			void setSpeed(float);
			void setClockwise(bool);
			void setRelative(bool);
		private:
			motor_point_t dest;
			motor_point_t center;
			int splitter;
			float speed;
			bool clockwise;
			bool rel;
	};

	// Calculates center position relatively to current position
	class RelArcTaskStep : public TaskStep {
		public:
			RelArcTaskStep(motor_point_t, motor_point_t, int, float, bool = false);
			virtual ~RelArcTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
			void setClockwise(bool);
		private:
			motor_point_t dest;
			motor_point_t center;
			int splitter;
			float speed;
			bool clockwise;
			bool rel;
	};

	enum class CoordTaskType {
		ProgrammedTask, GraphTask, WrapperTask, GCodeTask, LinearTask
	};
	
	class CoordTask : public TaskStep {
		public:
			CoordTask(CoordTaskType tp) {this->type = tp;}
			virtual ~CoordTask() {}
			CoordTaskType getType() {return this->type;}
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*) = 0;
		private:
			CoordTaskType type;
	};
	
	class LinearCoordTask : public CoordTask {
		public:
			LinearCoordTask(motor_rect_t, motor_coord_t, bool = true);
			virtual ~LinearCoordTask();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
			
			motor_rect_t getRectangle();
			void setRectangle(motor_rect_t);
			motor_coord_t getSpacing();
			void setSpacing(motor_coord_t);
			bool isVertical();
			void setVertical(bool);
		private:
			motor_rect_t square;
			motor_coord_t spacing;
			bool vertical;
	};
	
	class ProgrammedCoordTask : public CoordTask {
		public:
			ProgrammedCoordTask();
			virtual ~ProgrammedCoordTask();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
			void addStep(TaskStep*);
			size_t getSubCount();
			bool removeStep(size_t);
			TaskStep *pollStep(size_t);
			bool insertStep(size_t, TaskStep*);
		private:
			std::vector<TaskStep*> list;
	};
	
	class GraphCoordTask : public CoordTask {
		public:
			GraphCoordTask(GraphBuilder*, CoordTranslator*, float);
			virtual ~GraphCoordTask();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
		private:
			GraphBuilder *graph;
			CoordTranslator *trans;
			float scale;
	};
	
	class GCodeCoordTask : public CoordTask {
		public:
			GCodeCoordTask(std::istream*, CoordTranslator*);
			virtual ~GCodeCoordTask();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*, TaskState*);
			
			std::string getGCode();
			CoordTranslator *getTranslator();
		private:
			std::string code;
			GCodeStream *stream;
			CoordTranslator *translator;
	};
}

#endif
