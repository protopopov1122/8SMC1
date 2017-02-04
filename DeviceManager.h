#ifndef _8SMC1_DEVICE_MANAGER_H_
#define _8SMC1_DEVICE_MANAGER_H_

#include <vector>
#include <string>
#include "Device.h"
#include "USMCDLL.h"

/* Device manager initialise existing controllers and keep track of usable devices */

namespace Controller {
	class DeviceManager {
		public:
			DeviceManager();
			virtual ~DeviceManager();
			void refresh();				// USMC_Init
			void printError();			// USMC_GetLastErr
			Device *getDevice(DWORD);
			unsigned int getDeviceCount();
			std::string getDeviceSerial(DWORD);
			std::string getDeviceVersion(DWORD);
			// To support fast motor stop
			void setLastDevice(Device*);
			Device *getLastDevice();
		private:
			USMC_Devices devs;
			std::vector<Device*> dev;
			Device *last_device;
	};
}

#endif
