#ifndef _8SMC1_DEVICE_MANAGER_H_
#define _8SMC1_DEVICE_MANAGER_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <cinttypes>
#include "Device.h"

/* Device manager initialise existing controllers and keep track of usable devices */

namespace _8SMC1 {

	class DeviceManager {
		public:
			virtual ~DeviceManager();
			virtual void refresh() = 0;
			virtual void saveError() = 0;
			virtual Device *getDevice(device_id_t);
			virtual size_t getDeviceCount();
			virtual std::string getDeviceSerial(device_id_t) = 0;
			virtual std::string getDeviceVersion(device_id_t) = 0;
			virtual bool hasError();
			virtual std::string pollError();
		protected:
			std::vector<std::string> error_queue;
			std::vector<Device*> dev;
	};
}

#endif
