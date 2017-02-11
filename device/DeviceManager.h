#ifndef _8SMC1_DEVICE_MANAGER_H_
#define _8SMC1_DEVICE_MANAGER_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <cinttypes>
#include "Device.h"

/* Device manager initialise existing controllers and keep track of usable devices */

namespace Controller {

	class DeviceController;	// For forward referencing
	class CoordController;
	class CoordTask;

	class DeviceManager {
		public:
			DeviceManager();
			virtual ~DeviceManager();
			void refresh();				// USMC_Init
			void saveError();			// USMC_GetLastErr
			Device *getDevice(DWORD);
			unsigned int getDeviceCount();
			std::string getDeviceSerial(DWORD);
			std::string getDeviceVersion(DWORD);
			// To support fast motor stop
			void setLastDevice(Device*);
			Device *getLastDevice();
			// Error handling
			bool hasError();
			std::string pollError();
		private:
			USMC_Devices devs;
			std::vector<Device*> dev;
			std::vector<std::string> error_queue;
			Device *last_device;
	};
}

#endif
