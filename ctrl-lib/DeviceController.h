#ifndef _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_
#define _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_

#include <cinttypes>
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
			DWORD getID();

			ErrorCode checkTrailers();
			ErrorCode waitWhileRunning();
			ErrorCode moveToTrailer(int, int);
			ErrorCode resetPosition();
			ErrorCode startMove(motor_coord_t, float, int, bool = false);
			ErrorCode startRelativeMove(motor_coord_t, float, int, bool = false);
			void stop();
			unsigned int getLength();

			motor_coord_t getPosition();
		private:
			// Calibration
			void calculate_length();

			Device *dev;
			MoveType dest;
			unsigned int length;
	};
}

#endif
