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
		ArcError = 6
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
	
	enum TrailerId {
		Trailer1 = 1, Trailer2 = 2
	};
}


#endif