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

#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/ConfigManager.h"
#include <cinttypes>
#include <string>

/* This file contains definitions of Device and Instrument classes.
   They are main Device API abstractions that represent different
   types of motors and cutting instruments. */

namespace CalX {

	class DeviceManager;	// For forward referencing

	enum class DeviceConnectionType {
		SerialPort
	};
	
	struct DeviceConnectionPrms {
		public:
			DeviceConnectionPrms(DeviceConnectionType t) {this->type = t;}
			
			DeviceConnectionType type;
	};
	
	enum class SerialPortParity {
		No = 0, Odd = 1,
		Even = 2, Mark = 3,
		Space = 4
	};
	
	struct DeviceSerialPortConnectionPrms : public DeviceConnectionPrms {
		public:
			DeviceSerialPortConnectionPrms() :
				DeviceConnectionPrms::DeviceConnectionPrms(DeviceConnectionType::SerialPort) {}
				
			uint8_t port;
			uint32_t speed;
			SerialPortParity parity;
	};
	
	typedef int64_t device_id_t;
	#define DEVICE_ID_FMT PRId64

	// Device power indicator
	enum class Power {
		NoPower, HalfPower, FullPower
	};
	
	/* Abstract device.
	   It contains two method types:
	       * methods that are actually performing some actions or return useful information - Functional.
		   * methods that are optional and may return fake values - Optional.
	*/
	class Motor {
		public:
			virtual ~Motor();
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
			Instrument();
			virtual ~Instrument();
			virtual device_id_t getID();	// Defined by device manager
			virtual DeviceManager *getDeviceManager();
			virtual bool enable(bool) = 0;
			virtual bool enabled() = 0;
			virtual std::string getInfo() = 0;
			virtual bool hasErrors();
			virtual std::string pollError();
			virtual void getModes(std::vector<std::string>&);
			virtual bool setMode(size_t) = 0;
			virtual size_t getMode() = 0;
			virtual ConfigManager *getConfiguration();
		protected:
			device_id_t dev;
			DeviceManager *devman;
			std::vector<std::string> errors;
			std::vector<std::string> modes;
			ConfigManager config;
	};
}

#endif
