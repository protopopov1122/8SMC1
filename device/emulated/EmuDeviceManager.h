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


#ifndef CALX_DEVICE_EMULATED_EMU_DEVICE_MANAGER_H_
#define CALX_DEVICE_EMULATED_EMU_DEVICE_MANAGER_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <cinttypes>
#include "EmuMotor.h"
#include "EmuInstrument.h"
#include "ctrl-lib/device/DeviceManager.h"


namespace CalX {

	class EmuDeviceManager : public DeviceManager {
		public:
			EmuDeviceManager();
			virtual ~EmuDeviceManager();
			virtual void refresh();
			virtual Motor *connectMotor(DeviceConnectionPrms*);
			virtual Instrument *connectInstrument(DeviceConnectionPrms*);
	};
}

#endif
