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

/* This file contains definitions of Device and Instrument classes.
   They are main Device API abstractions that represent different
   types of motors and cutting instruments. */

namespace CalX {

	class DeviceManager;	// For forward referencing

	typedef int64_t device_id_t;

	// Device power indicator
	enum Power {
		NoPower, HalfPower, FullPower
	};
	
	/* Abstract device.
	   It contains two method types:
	       * methods that are actually performing some actions or return useful information - Functional.
		   * methods that are optional and may return fake values - Optional.
	*/
	class Device {
		public:
			virtual ~Device();
			virtual DeviceManager *getDeviceManager();
			virtual device_id_t getID();		// Defined by device manager.
			virtual std::string getSerial();	// Optional. Depends on device manager.
			virtual std::string getVersion();	// Optional. Depends on device manager.
			virtual int getPosition() = 0;		// Functional. Returns motor position in motor steps.
			virtual bool isTrailerPressed(int) = 0;	// Functional. Provides info about device trailers.
			virtual bool isRunning() = 0;		// Functional. Provides info about device state.
			virtual Power getPowerState() = 0;	// Functional. Provides info about device power state.
			virtual bool start(int, float,
					unsigned char, bool  = false) = 0;	// Functional. Controls device movement.
			virtual bool stop() = 0;			// Functional. Controls device movement.
			virtual bool flipPower() = 0;		// Functional. Controls device power.
			virtual float getTemperature() = 0;	// Optional. Provides info about motor temperature.
			virtual float getVoltage() = 0;		// Optional. Provides info about motor voltage.
		protected:
			device_id_t dev;
			DeviceManager *devman;
	};

	/* Abstract instrument.
	   Has few methods to enable/disable and check state. They all are functional. */
	class Instrument {
		public:
			virtual ~Instrument();
			virtual device_id_t getID();	// Defined by device manager
			virtual DeviceManager *getDeviceManager();
			virtual bool enable(bool) = 0;
			virtual bool enabled() = 0;
		protected:
			device_id_t dev;
			DeviceManager *devman;
	};
}

#endif
