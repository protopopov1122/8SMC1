#ifndef CALX_STANDART_DEVICE_MANAGER_H_
#define CALX_STANDART_DEVICE_MANAGER_H_

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

	class StandartDeviceManager : public DeviceManager {
		public:
			StandartDeviceManager();
			virtual ~StandartDeviceManager();
			virtual void refresh();				// USMC_Init
			virtual std::string getMotorSerial(device_id_t);
			virtual std::string getMotorVersion(device_id_t);
			virtual Motor *connectMotor(DeviceConnectionPrms*);
			virtual Instrument *connectInstrument(DeviceConnectionPrms*);
			virtual void saveMotorError();
			virtual void saveInstrumentError();
		private:
			USMC_Devices devs;
	};
}

#endif