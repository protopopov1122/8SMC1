#ifndef _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_
#define _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_

#include <cinttypes>
#include <pthread.h>
#include "DeviceManager.h"

/* This file contains definitions of high-level controller library.
   Library purpose is controlling device state, speed, complex
   moves and synchronization. */

typedef int32_t motor_coord_t;
namespace _8SMC1 {

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

			ErrorCode checkTrailers();
			ErrorCode waitWhileRunning();
			ErrorCode moveToTrailer(int, int);
			ErrorCode resetPosition();
			ErrorCode startMove(motor_coord_t, float, int, bool = false);
			void stop();
			unsigned int getLength();

			motor_coord_t getPosition();

			friend void *device_control_thread(void*);
		private:
			// Calibration
			void calculate_length();
			// Threading related
			pthread_t dev_thread;
			bool exists;
			bool wait_for_thread;

			Device *dev;
			MoveType dest;
			unsigned int length;
	};

	class CoordController {
		public:
			CoordController(DeviceController*, DeviceController*);
			virtual ~CoordController();
			DeviceController *getXAxis();
			DeviceController *getYAxis();
			ErrorCode move(motor_point_t, float, int, bool);
			ErrorCode calibrate(int);
			motor_point_t getPosition();
		private:
			DeviceController *xAxis;
			DeviceController *yAxis;
	};
}

#endif
