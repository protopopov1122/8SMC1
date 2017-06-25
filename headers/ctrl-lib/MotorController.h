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


#ifndef CALX_CTRL_LIB_DEVICE_CONTROLLER_H_
#define CALX_CTRL_LIB_DEVICE_CONTROLLER_H_

#include <cinttypes>
#include <vector>
#include "ctrl-lib/device/DeviceManager.h"
#include "ctrl-lib/ConfigManager.h"
#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/EventListener.h"

/* This file contains definitions of high-level device controller library.
   Library purpose is controlling device state, speed, complex
   moves and synchronization.
   This class was made at the start of project, but
   now almost all its functional is migrated to CooordController.
   It is used only by CLI.*/

namespace CalX {

#define ROLL_STEP 30000
#define ROLL_SPEED 4000.0f
#define ROLL_DIV 8
#define TRAILER_COMEBACK 800

	
	class MotorController {
		public:
			friend class CoordController;

			MotorController(Motor*, ConfigManager*);
			virtual ~MotorController();
			Motor *getMotor();
			device_id_t getID();

			Power getPowerState();
			ErrorCode enablePower(bool);
			ErrorCode flipPower();
			
			ErrorCode moveToTrailer(int);
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
			
			Motor *dev;
			MoveType dest;
			ConfigManager *config;
			
			std::vector<MotorEventListener*> listeners;
	};
}

#endif
