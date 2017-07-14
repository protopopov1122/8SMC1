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

#ifndef CALX_DEVICE_EMULATED_EMU_MOTOR_H_
#define CALX_DEVICE_EMULATED_EMU_MOTOR_H_

#include "ctrl-lib/device/Device.h"
#include <string>
#include <vector>

namespace CalX {

	class EmuDeviceManager;  // Forward referencing

#define EMU_MOTOR_SIZE 300000

	class EmuMotor : public Motor {
	 public:
		EmuMotor(device_id_t, EmuDeviceManager *);
		virtual ~EmuMotor();

		virtual DeviceManager *getDeviceManager();
		virtual std::string getDeviceInfo();
		virtual std::string getRuntimeInfo();
		virtual void terminate();

		virtual motor_coord_t getPosition();
		virtual bool isTrailerPressed(int);
		virtual bool isRunning();
		virtual Power getPowerState();
		virtual bool start(motor_coord_t, float);
		virtual bool stop();
		virtual bool enablePower(bool);

	 private:
		EmuDeviceManager *devman;
		motor_coord_t pos;
		Power power;
	};
}

#endif
