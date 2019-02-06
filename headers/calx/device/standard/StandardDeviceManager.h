/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_DEVICE_STANDARD_STANDARD_DEVICE_MANAGER_H_
#define CALX_DEVICE_STANDARD_STANDARD_DEVICE_MANAGER_H_

#include "calx/ctrl-lib/device/DeviceManager.h"
#include "calx/device/standard/8SMC1Device.h"
#include "calx/device/standard/NL300Instrument.h"
#include <cinttypes>
#include <stdlib.h>
#include <cstring>
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
