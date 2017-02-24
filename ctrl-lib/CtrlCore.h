#ifndef _8SMC1_CTRL_LIB_CTRL_CORE_H_
#define _8SMC1_CTRL_LIB_CTRL_CORE_H_

#include <cinttypes>

/*This file contains basic structures, enums and constants that commonly used by other library parts. */

namespace _8SMC1 {
	typedef int64_t motor_coord_t;
	
	enum ErrorCode {
		NoError = 0, WrongParameter = 1,
		Trailer1Pressed = 2, Trailer2Pressed = 3,
		DeviceRunning = 4, DeviceStopped = 5,
		ArcError = 6,
		InvalidCoordinates = 7, InvalidSpeed = 8,
		MathExprError = 9
	};

	enum MoveType {
		Stop = 0, MoveDown = -1, MoveUp = 1,
		RollDown = -2, RollUp = 2
	};

	struct motor_point_t {
		motor_coord_t x;
		motor_coord_t y;
	};	

	struct motor_size_t {
		motor_coord_t w;
		motor_coord_t h;
	};
	
	struct motor_scale_t {
		long double x;
		long double y;
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
	
	struct coord_point_t {
		long double x;
		long double y;
	};
	
	struct coord_size_t {
		long double w;
		long double h;
	};
	
	struct coord_scale_t {
		long double x;
		long double y;
	};
}


#endif