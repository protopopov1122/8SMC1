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
#include <mutex>
#include <string>

/* This file contains Device API interfaces. */

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

	enum class Power {
		NoPower, HalfPower, FullPower
	};

	enum class InstrumentMode {
		Off, Prepare, Full
	};

	enum class DeviceType {
		Motor, Instrument
	};

	class Device {
		public:
			Device(DeviceType, device_id_t);
			virtual ~Device();
			DeviceType getType();
			void lock();
			void unlock();
			bool tryLock();
			virtual device_id_t getID();
			virtual bool hasErrors();
			virtual std::string pollError();
			virtual ConfigManager *getConfiguration();
			virtual DeviceManager *getDeviceManager() = 0;
			virtual std::string getDeviceInfo() = 0;
			virtual std::string getRuntimeInfo() = 0;
		protected:
			virtual void log(std::string);

			device_id_t id;
			ConfigManager config;
			std::vector<std::string> errors;
		private:
			DeviceType type;
			std::mutex dev_mutex;
	};

	class Motor : public Device {
		public:
			Motor(device_id_t);
			virtual ~Motor();
            virtual motor_coord_t getPosition() = 0;
			virtual bool isTrailerPressed(int) = 0;
			virtual bool isRunning() = 0;
			virtual Power getPowerState() = 0;
            virtual bool start(motor_coord_t, float,
					unsigned char, bool  = false) = 0;
			virtual bool stop() = 0;
			virtual bool enablePower(bool) = 0;
	};


	class Instrument : public Device {
		public:
			Instrument(device_id_t);
			virtual ~Instrument();
			virtual bool open_session() = 0;
			virtual bool close_session() = 0;
			virtual bool enable(bool) = 0;
			virtual bool enabled() = 0;
			virtual InstrumentMode getWorkingMode() = 0;
			virtual bool setWorkingMode(InstrumentMode) = 0;
		protected:
			virtual void log(std::string);
	};
}

#endif
