/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_CTRL_LIB_MOTOR_CONTROLLER_H_
#define CALX_CTRL_LIB_MOTOR_CONTROLLER_H_

#include "calx/ctrl-lib/ctrl/DeviceController.h"

/* Motor controller offers some more complex actions on top of
   motor device drivers. */

namespace CalX {

	static constexpr unsigned int ROLL_STEP = 30000;
	static constexpr float ROLL_SPEED = 4000.0f;
	static constexpr unsigned int TRAILER_COMEBACK = 800;

	class MotorController
	    : public DeviceController,
	      public EventSource<std::shared_ptr<MotorEventListener>> {
	 public:
		MotorController(ConfigurationCatalogue &, Motor &);
		Motor &getMotor() const;

		Power getPowerState() const;
		ErrorCode enablePower(bool);
		ErrorCode flipPower();

		ErrorCode moveToTrailer(TrailerId);
		ErrorCode startMove(motor_coord_t, float);
		ErrorCode startRelativeMove(motor_coord_t, float);
		ErrorCode asyncMove(motor_coord_t, float, bool = false);
		ErrorCode asyncRelativeMove(motor_coord_t, float, bool = false);
		void asyncStop(ErrorCode, motor_coord_t, float, bool = false);
		void stop();

		motor_coord_t getPosition() const;
		bool isMoving() const;

		void use() override;
		void unuse() override;

		ErrorCode waitWhileRunning();
		ErrorCode checkTrailers();
		bool isTrailerPressed(TrailerId) const;

	 private:
		void sendMovingEvent(MotorMoveEvent &);
		void sendMovedEvent(MotorMoveEvent &);
		void sendStoppedEvent(MotorErrorEvent &);
		void sendRollingEvent(MotorRollEvent &);
		void sendRolledEvent(MotorRollEvent &);

		MoveType destination;
		bool work;
		Motor &motor;
	};
}  // namespace CalX

#endif
