#ifndef CALX_8SMC1_DEVICE_MANAGER_H_
#define CALX_8SMC1_DEVICE_MANAGER_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <cinttypes>
#include "8SMC1Device.h"
#include "NL300Instrument.h"
#include "device/DeviceManager.h"

/* Device manager initialise existing controllers and keep track of usable devices */

namespace CalX {

	class _8SMC1DeviceManager : public DeviceManager {
		public:
			_8SMC1DeviceManager();
			virtual ~_8SMC1DeviceManager();
			virtual void refresh();				// USMC_Init
			virtual void saveError();			// USMC_GetLastErr
			virtual std::string getMotorSerial(device_id_t);
			virtual std::string getMotorVersion(device_id_t);
			virtual Motor *connectMotor(DeviceConnectionPrms*);
			virtual Instrument *connectInstrument(DeviceConnectionPrms*);
		private:
			USMC_Devices devs;
	};
}

#endif