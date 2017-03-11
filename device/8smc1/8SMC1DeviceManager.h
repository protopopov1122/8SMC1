#ifndef CALX_8SMC1_DEVICE_MANAGER_H_
#define CALX_8SMC1_DEVICE_MANAGER_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <cinttypes>
#include "8SMC1Device.h"
#include "device/DeviceManager.h"

/* Device manager initialise existing controllers and keep track of usable devices */

namespace CalX {

	class CALXDeviceManager : public DeviceManager {
		public:
			CALXDeviceManager();
			virtual ~CALXDeviceManager();
			virtual void refresh();				// USMC_Init
			virtual void saveError();			// USMC_GetLastErr
			virtual std::string getDeviceSerial(device_id_t);
			virtual std::string getDeviceVersion(device_id_t);
		private:
			USMC_Devices devs;
	};
}

#endif