#ifndef _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_
#define _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_

#include <cinttypes>
#include <pthread.h>
#include "DeviceManager.h"
#include "CtrlCore.h"

/* This file contains definitions of high-level controller library.
   Library purpose is controlling device state, speed, complex
   moves and synchronization. */

namespace _8SMC1 {

#define ROLL_STEP 30000
#define ROLL_SPEED 4000.0f
#define ROLL_DIV 8
#define TRAILER_COMEBACK 800

	class CoordController;	// Forward referencing

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
			ErrorCode startRelativeMove(motor_coord_t, float, int, bool = false);
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
}

#endif
