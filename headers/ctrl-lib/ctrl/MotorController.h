/*
        Copyright (c) 2017 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
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

#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/EventListener.h"
#include "ctrl-lib/conf/ConfigManager.h"
#include "ctrl-lib/device/DeviceManager.h"
#include <cinttypes>
#include <vector>

/* Motor controller offers some more complex actions on top of
   motor device drivers. */

namespace CalX {

#define ROLL_STEP 30000
#define ROLL_SPEED 4000.0f
#define TRAILER_COMEBACK 800

	class MotorController {
	 public:
		MotorController(Motor *, ConfigManager *);
		Motor *getMotor();
		device_id_t getID();

		Power getPowerState();
		ErrorCode enablePower(bool);
		ErrorCode flipPower();

		ErrorCode moveToTrailer(int);
		ErrorCode startMove(motor_coord_t, float);
		ErrorCode startRelativeMove(motor_coord_t, float);
		void stop();

		motor_coord_t getPosition();

		void addEventListener(std::shared_ptr<MotorEventListener>);
		void removeEventListener(std::shared_ptr<MotorEventListener>);
		void use();
		void unuse();

		ErrorCode waitWhileRunning();
		ErrorCode checkTrailers();

		void sendMovingEvent(MotorMoveEvent &);
		void sendMovedEvent(MotorMoveEvent &);
		void sendStoppedEvent(MotorErrorEvent &);
		void sendRollingEvent(MotorRollEvent &);
		void sendRolledEvent(MotorRollEvent &);

		MoveType dest;

	 private:
		bool work;

		Motor *dev;
		ConfigManager *config;

		std::vector<std::shared_ptr<MotorEventListener>> listeners;
	};
}

#endif
