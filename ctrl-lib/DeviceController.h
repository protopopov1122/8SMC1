#ifndef _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_
#define _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_

#include <cinttypes>
#include "DeviceManager.h"

/* This file contains definitions of high-level controller library.
   Library purpose is controlling device state, speed, complex
   moves and synchronization. */

typedef int32_t motor_coord_t;
typedef uint8_t err_code_t;

namespace Controller {

	enum ErrorCode {
		NoError = 0, WrongParameter = 1,
		Trailer1Pressed = 2, Trailer2Pressed = 3
	};

	struct motor_point_t {
		motor_coord_t x;
		motor_coord_t y;
	};	

	class DeviceController {
		public:
			DeviceController(Device*);
			virtual ~DeviceController();
			Device *getDevice();

			err_code_t waitWhileRunning();
			err_code_t moveToTrailer(int);
			err_code_t startMove(motor_coord_t, float, int, bool);
		private:
			Device *dev;
	};

	class CoordController {
		public:
			CoordController(DeviceController*, DeviceController*);
			virtual ~CoordController();
			DeviceController *getXAxis();
			DeviceController *getYAxis();
			err_code_t move(motor_point_t, float, int, bool);
		private:
			DeviceController *xAxis;
			DeviceController *yAxis;
	};
}

#endif
