#ifndef _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_
#define _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_

#include <cinttypes>
#include <pthread.h>
#include "DeviceManager.h"

/* This file contains definitions of high-level controller library.
   Library purpose is controlling device state, speed, complex
   moves and synchronization. */

typedef int32_t motor_coord_t;
typedef uint8_t err_code_t;

namespace Controller {

	enum ErrorCode {
		NoError = 0, WrongParameter = 1,
		Trailer1Pressed = 2, Trailer2Pressed = 3,
		DeviceRunning = 4, DeviceStopped = 5
	};

	enum MoveType {
		Stop = 0, MoveDown = -1, MoveUp = 1,
		RollDown = -2, RollUp = 2
	};

	struct motor_point_t {
		motor_coord_t x;
		motor_coord_t y;
	};	

#define ROLL_STEP 30000
#define ROLL_SPEED 4000.0f
#define ROLL_DIV 8
#define TRAILER_COMEBACK 800

	class CoordController;

	class DeviceController {
		public:
			friend class CoordController;

			DeviceController(Device*);
			virtual ~DeviceController();
			Device *getDevice();

			err_code_t checkTrailers();
			err_code_t waitWhileRunning();
			err_code_t moveToTrailer(int, int);
			err_code_t startMove(motor_coord_t, float, int, bool = false);
			void stop();

			friend void *device_control_thread(void*);
		private:
			// Threading related
			pthread_t dev_thread;
			bool exists;
			bool wait_for_thread;

			Device *dev;
			MoveType dest;
	};

	class CoordController {
		public:
			CoordController(DeviceController*, DeviceController*);
			virtual ~CoordController();
			DeviceController *getXAxis();
			DeviceController *getYAxis();
			err_code_t move(motor_point_t, float, int, bool);
			err_code_t calibrate(int);
		private:
			DeviceController *xAxis;
			DeviceController *yAxis;
	};
}

#endif
