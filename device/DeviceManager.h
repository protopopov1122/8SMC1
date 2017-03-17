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
#include "platform.h"
#include "Device.h"

/* Device manager initialise existing controllers and keep track of usable devices and instruments.
   Some of device manager functionality are common for all possible managers(however it can be overrided), some is specific.*/

namespace CalX {

	class DeviceManager {
		public:
			virtual ~DeviceManager();
			virtual void refresh() = 0;				// Refresh device list, reinitialise them.
			virtual void saveError() = 0;			// Poll all device generated errors and save them into queue
			virtual Device *getDevice(device_id_t);	// Return device by id
			virtual size_t getDeviceCount();		// Get device count
			virtual std::string getDeviceSerial(device_id_t) = 0;	// Optional. Return device serial number
			virtual std::string getDeviceVersion(device_id_t) = 0;	// Optional. Return device version.
			virtual size_t getInstrumentCount();	// Get instrument count
			virtual Instrument *getInstrument(device_id_t);	// Get instrument by id
			virtual bool hasError();				// Check errors
			virtual std::string pollError();		// Return error from queue
		protected:
			std::vector<std::string> error_queue;
			std::vector<Device*> dev;
			std::vector<Instrument*> instr;
	};
	
	/* Used as exported function in Device API implementations.*/
	extern "C" LIBEXPORT DeviceManager *getDeviceManager();
	typedef DeviceManager* (*DeviceManager_getter)();
}

#endif
