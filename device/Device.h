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


#ifndef CALX_DEVICE_H_
#define CALX_DEVICE_H_

#include <cinttypes>
#include <string>

/* Device class contains interface to abstract Device. */

namespace CalX {

	class DeviceManager;	// For forward referencing

	typedef int64_t device_id_t;

	enum Power {
		NoPower, HalfPower, FullPower
	};
	
	class Device {
		public:
			virtual ~Device();
			virtual DeviceManager *getDeviceManager();
			virtual device_id_t getID();
			virtual std::string getSerial();
			virtual std::string getVersion();
			virtual int getPosition() = 0;
			virtual bool isTrailerPressed(int) = 0;
			virtual bool isRunning() = 0;
			virtual Power getPowerState() = 0;
			virtual bool start(int, float,
					unsigned char, bool  = false) = 0;
			virtual bool stop() = 0;
			virtual bool flipPower() = 0;
			virtual float getTemperature() = 0;
			virtual float getVoltage() = 0;
		protected:
			device_id_t dev;
			DeviceManager *devman;
	};

}

#endif
