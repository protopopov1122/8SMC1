#ifndef CALX_DEVICE_STANDARD_STANDARD_DEVICE_MANAGER_H_
#define CALX_DEVICE_STANDARD_STANDARD_DEVICE_MANAGER_H_

#include "ctrl-lib/device/DeviceManager.h"
#include "device/standard/8SMC1Device.h"
#include "device/standard/NL300Instrument.h"
#include <cinttypes>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

/* Device manager initialise existing controllers and keep track of usable
 * devices */

namespace CalX {

	class StandardDeviceManager : public DeviceManager {
	 public:
		StandardDeviceManager();
		virtual ~StandardDeviceManager();
		virtual void refresh();  // USMC_Init
		virtual std::string getMotorSerial(device_id_t);
		virtual std::string getMotorVersion(device_id_t);
		virtual Motor *connectMotor(DeviceConnectionPrms *);
		virtual Instrument *connectInstrument(DeviceConnectionPrms *);
		virtual void saveMotorError();
		virtual void saveInstrumentError();

	 private:
		USMC_Devices devs;
	};
}  // namespace CalX

#endif