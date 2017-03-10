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


#ifndef CALX_CTRL_LIB_EVENT_LISTENER_H_
#define CALX_CTRL_LIB_EVENT_LISTENER_H_

#include "CtrlCore.h"

namespace CalX {
	
	struct MotorMoveEvent {
		motor_coord_t destination;
		float speed;
		int divisor;
	};
	
	struct MotorErrorEvent {
		ErrorCode errcode;
	};
	
	struct MotorRollEvent {
		int trailer;
	};
	
	class MotorEventListener {
		public:
			virtual ~MotorEventListener() {}
			virtual void use() {}
			virtual void unuse() {}
			virtual void moving(MotorMoveEvent&) {}
			virtual void moved(MotorMoveEvent&) {}
			virtual void stopped(MotorErrorEvent&) {}
			virtual void rolling(MotorRollEvent&) {}
			virtual void rolled(MotorRollEvent&) {}
	};
	
	struct CoordMoveEvent {
		motor_point_t destination;
		float speed;
		int divisor;
		bool synchrone;
	};
	
	struct CoordErrorEvent {
		ErrorCode errcode;
	};
	
	struct CoordCalibrateEvent {
		TrailerId trailer;
	};
	
	class CoordEventListener {
		public:
			virtual ~CoordEventListener() {}
			virtual void use() {}
			virtual void unuse() {}
			virtual void moving(CoordMoveEvent&) {}
			virtual void moved(CoordMoveEvent&) {}
			virtual void stopped(CoordErrorEvent&) {}
			virtual void calibrating(CoordCalibrateEvent&) {}
			virtual void calibrated(CoordCalibrateEvent&) {}
	};
}


#endif