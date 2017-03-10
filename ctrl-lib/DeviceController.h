#ifndef _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_
#define _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_

#include <cinttypes>
#include <vector>
#include "device/DeviceManager.h"
#include "CtrlCore.h"
#include "EventListener.h"

/* This file contains definitions of high-level device controller library.
   Library purpose is controlling device state, speed, complex
   moves and synchronization.
   This class was made at the start of project, but
   now almost all its functional is migrated to CooordController.
   It is used only by CLI.*/

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
			device_id_t getID();

			ErrorCode moveToTrailer(int, int);
			ErrorCode startMove(motor_coord_t, float, int, bool = false);
			ErrorCode startRelativeMove(motor_coord_t, float, int, bool = false);
			void stop();

			motor_coord_t getPosition();
			
			void addEventListener(MotorEventListener*);
			void removeEventListener(MotorEventListener*);
			void use();
			void unuse();
		protected:
			void sendMovingEvent(MotorMoveEvent&);
			void sendMovedEvent(MotorMoveEvent&);
			void sendStoppedEvent(MotorErrorEvent&);
			void sendRollingEvent(MotorRollEvent&);
			void sendRolledEvent(MotorRollEvent&);
		private:
			bool work;
			ErrorCode waitWhileRunning();
			ErrorCode checkTrailers();
			
			Device *dev;
			MoveType dest;
			
			std::vector<MotorEventListener*> listeners;
	};
}

#endif
