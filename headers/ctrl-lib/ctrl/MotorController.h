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

#ifndef CALX_CTRL_LIB_MOTOR_CONTROLLER_H_
#define CALX_CTRL_LIB_MOTOR_CONTROLLER_H_

#include "ctrl-lib/ctrl/DeviceController.h"

/* Motor controller offers some more complex actions on top of
   motor device drivers. */

namespace CalX {

#define ROLL_STEP 30000
#define ROLL_SPEED 4000.0f
#define TRAILER_COMEBACK 800

	class CoordController;  // Forward referencing

	class MotorController : public DeviceController {
		friend class CoordController;

	 public:
		MotorController(ConfigManager &, Motor *);
		Motor *getMotor();

		Power getPowerState();
		ErrorCode enablePower(bool);
		ErrorCode flipPower();

		ErrorCode moveToTrailer(TrailerId);
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

	 private:
		void sendMovingEvent(MotorMoveEvent &);
		void sendMovedEvent(MotorMoveEvent &);
		void sendStoppedEvent(MotorErrorEvent &);
		void sendRollingEvent(MotorRollEvent &);
		void sendRolledEvent(MotorRollEvent &);

		MoveType dest;
		bool work;
		Motor *dev;
		std::vector<std::shared_ptr<MotorEventListener>> listeners;
	};
}

#endif
