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


#ifndef CALX_CTRL_LIB_CTRL_CORE_H_
#define CALX_CTRL_LIB_CTRL_CORE_H_

#include <cinttypes>
#include "platform.h"

/* This file contains basic structures, enums and constants that commonly used by other library parts. */

namespace CalX {
	typedef int64_t motor_coord_t;
	
	
	/* Ctrl-lib possible errors. It doesn't include all Device API low-level errors.*/
	enum ErrorCode {
		NoError = 0, WrongParameter = 1,
		Trailer1Pressed = 2, Trailer2Pressed = 3,
		DeviceRunning = 4, DeviceStopped = 5,
		ArcError = 6,
		InvalidCoordinates = 7, InvalidSpeed = 8,
		MathExprError = 9
	};

	/* Used in device controller. Describes current move type and state.*/
	enum MoveType {
		Stop = 0, MoveDown = -1, MoveUp = 1,
		RollDown = -2, RollUp = 2
	};

	
	/* Different structures that describe motor positions, position changes and state.*/
	struct motor_point_t {
		motor_coord_t x;
		motor_coord_t y;
	};	

	struct motor_size_t {
		motor_coord_t w;
		motor_coord_t h;
	};
	
	struct motor_scale_t {
		double x;
		double y;
	};
	
	struct motor_rect_t {
		motor_coord_t x;
		motor_coord_t y;
		motor_coord_t w;
		motor_coord_t h;
	};
	
	enum TrailerId {
		Trailer1 = 1, Trailer2 = 2
	};
	
	/* Different structures that describe coordinate plane points and scales.*/
	struct coord_point_t {
		double x;
		double y;
	};
	
	struct coord_size_t {
		double w;
		double h;
	};
	
	struct coord_scale_t {
		double x;
		double y;
	};
}


#endif