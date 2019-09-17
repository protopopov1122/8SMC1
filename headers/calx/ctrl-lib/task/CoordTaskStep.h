/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_CTRL_LIB_TASK_COORD_TASK_STEP_H_
#define CALX_CTRL_LIB_TASK_COORD_TASK_STEP_H_

#include "calx/ctrl-lib/task/AbstractCoordTask.h"

namespace CalX {

	class MoveTaskStep : public TaskStep {
	 public:
		MoveTaskStep(motor_point_t, float, bool = false);
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;

		motor_point_t getPosition() const;
		float getSpeed() const;
		bool isRelative() const;

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
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;

		motor_point_t getPosition() const;
		float getSpeed() const;
		bool isRelative() const;

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
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;

	 private:
		TrailerId side;
	};

	class ArcTaskStep : public TaskStep {
	 public:
		ArcTaskStep(motor_point_t, motor_point_t, int, float, bool = false);
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;

		motor_point_t getDestination() const;
		motor_point_t getCenter() const;
		int getSplitter() const;
		float getSpeed() const;
		bool isClockwise() const;
		bool isRelative() const;

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
		ErrorCode perform(std::shared_ptr<CoordPlane>, TaskParameters &,
		                  SystemManager &, std::shared_ptr<TaskState>) override;
		void setClockwise(bool);

	 private:
		motor_point_t dest;
		motor_point_t center;
		int splitter;
		float speed;
		bool clockwise;
		bool rel;
	};
}  // namespace CalX

#endif
