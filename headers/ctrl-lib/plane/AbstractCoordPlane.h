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

#ifndef CALX_CTRL_LIB_PLANE_ABSTRACT_COORD_PLANE_H_
#define CALX_CTRL_LIB_PLANE_ABSTRACT_COORD_PLANE_H_

#include <iostream>
#include <vector>
#include <string>
#include "ctrl-lib/conf/ConfigManager.h"
#include "ctrl-lib/EventListener.h"

/*  Coordinate plane has several elementar actions:
      * linear movement - synchrone and asynchrone
	  * arc movement
	  * relative linear and arc movement - all coordinates are calculated using current position and offset
	  * calibration - both motors are moved to the start or end of axis
	  * measurement - plane performs double calibration to determine available size
    All actions should be performed during session. To open/close session use
	appropriate methods.
*/

namespace CalX {
  enum class CoordPlaneStatus { Idle, Jump, Move };

  // Main interface
  class CoordPlane {
   public:
	virtual ~CoordPlane() {}
	virtual ErrorCode move(motor_point_t, float, bool) = 0;
	virtual ErrorCode relativeMove(motor_point_t, float, bool);
	virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
						  float = 1.0f) = 0;
	virtual ErrorCode relativeArc(motor_point_t, motor_point_t, int, float,
								  bool, float = 1.0f);
	virtual ErrorCode calibrate(TrailerId) = 0;
	virtual ErrorCode measure(TrailerId) = 0;
	virtual motor_point_t getPosition() = 0;
	virtual motor_rect_t getSize() = 0;
	virtual bool isMeasured() = 0;
	virtual bool isUsed() = 0;
	virtual void use() = 0;
	virtual void unuse() = 0;
	virtual void stop() = 0;
	virtual void dump(std::ostream &) = 0;
	virtual CoordPlane *clone(CoordPlane *) = 0;
	virtual CoordPlaneStatus getStatus() = 0;
	virtual ErrorCode open_session();
	virtual ErrorCode close_session();
  };

  struct TaskState {
	CoordPlane *plane;
	bool work;

	void stop();
  };
}

#endif
