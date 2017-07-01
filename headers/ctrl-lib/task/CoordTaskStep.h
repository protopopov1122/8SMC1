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


#ifndef CALX_CTRL_LIB_TASK_COORD_TASK_STEP_H_
#define CALX_CTRL_LIB_TASK_COORD_TASK_STEP_H_

#include "ctrl-lib/task/AbstractCoordTask.h"

namespace CalX {

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
}

#endif
