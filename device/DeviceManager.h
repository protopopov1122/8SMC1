#ifndef _8SMC1_DEVICE_MANAGER_H_
#define _8SMC1_DEVICE_MANAGER_H_

#include <vector>
#include <string>
#include <cinttypes>
#include "Device.h"
#include "USMCDLL.h"

/* Device manager initialise existing controllers and keep track of usable devices */

namespace Controller {

	class DeviceController;	// For forward referencing
	class CoordController;

	class DeviceManager {
		public:
			DeviceManager();
			virtual ~DeviceManager();
			void refresh();				// USMC_Init
			void saveError();			// USMC_GetLastErr
			Device *getDevice(DWORD);
			DeviceController *getDeviceController(DWORD);
			unsigned int getDeviceCount();
			std::string getDeviceSerial(DWORD);
			std::string getDeviceVersion(DWORD);
			size_t getCoordCount();
			CoordController *getCoord(size_t);
			CoordController *createCoord(DWORD, DWORD);
			// To support fast motor stop
			void setLastDevice(Device*);
			Device *getLastDevice();
			// Error handling
			bool hasError();
			std::string pollError();
		private:
			USMC_Devices devs;
			std::vector<DeviceController*> dev;
			std::vector<CoordController*> coords;
			std::vector<std::string> error_queue;
			Device *last_device;
	};
}

#endif
