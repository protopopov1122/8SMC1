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


#ifndef CALX_DEVICE_MANAGER_H_
#define CALX_DEVICE_MANAGER_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <cinttypes>
#include <mutex>
#include "platform.h"
#include "ctrl-lib/device/Device.h"

/* Device manager initialise existing controllers and keep track of usable devices and instruments.
   Some of device manager functionality are common for all possible managers(however it can be overrided), some is specific.*/

namespace CalX {

	class DeviceManager {
		public:
			virtual ~DeviceManager();
			virtual void refresh() = 0;				// Refresh device list, reinitialise them.
			virtual Motor *getMotor(device_id_t);	// Return device by id
			virtual size_t getMotorCount();		// Get device count
			virtual size_t getInstrumentCount();	// Get instrument count
			virtual Instrument *getInstrument(device_id_t);	// Get instrument by id
			virtual bool hasError();				// Check errors
			virtual std::string pollError();		// Return error from queue

			virtual Motor *connectMotor(DeviceConnectionPrms*) = 0;
			virtual Instrument *connectInstrument(DeviceConnectionPrms*) = 0;
			virtual void getConnectionTypes(std::vector<DeviceConnectionType>&, std::vector<DeviceConnectionType>&);
			virtual bool canMotorConnect(DeviceConnectionType);
			virtual bool canInstrumentConnect(DeviceConnectionType);
			virtual bool loadConfiguration(std::string, ConfigManager*);
			void lock();
			void unlock();
			bool tryLock();
		protected:
			virtual void log(std::string);

			std::vector<std::string> error_queue;
			std::vector<Motor*> motors;
			std::vector<Instrument*> instruments;

			std::vector<DeviceConnectionType> motorConnectionType;
			std::vector<DeviceConnectionType> instrumentConnectionType;
		private:
			std::mutex devman_mutex;
	};

	/* Used as exported function in Device API implementations.*/
	extern "C" LIBEXPORT DeviceManager *getDeviceManager();
	typedef DeviceManager* (*DeviceManager_getter)();
}

#endif
