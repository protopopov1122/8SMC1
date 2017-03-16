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
#include "Device.h"

/* Device manager initialise existing controllers and keep track of usable devices */

namespace CalX {

	class DeviceManager {
		public:
			virtual ~DeviceManager();
			virtual void refresh() = 0;
			virtual void saveError() = 0;
			virtual Device *getDevice(device_id_t);
			virtual size_t getDeviceCount();
			virtual std::string getDeviceSerial(device_id_t) = 0;
			virtual std::string getDeviceVersion(device_id_t) = 0;
			virtual size_t getInstrumentCount();
			virtual Instrument *getInstrument(device_id_t);
			virtual bool hasError();
			virtual std::string pollError();
		protected:
			std::vector<std::string> error_queue;
			std::vector<Device*> dev;
			std::vector<Instrument*> instr;
	};
	
	#if defined (__WIN32) | defined(_WIN32) | defined (__WIN32__)
		#define EXPORT  __declspec(dllexport)
	#else
		#define EXPORT
	#endif
	
	extern "C" EXPORT DeviceManager *getDeviceManager();
	typedef DeviceManager* (*DeviceManager_getter)();
}

#endif
