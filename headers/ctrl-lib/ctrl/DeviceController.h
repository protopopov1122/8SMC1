/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_CTRL_LIB_DEVICE_CONTROLLER_H_
#define CALX_CTRL_LIB_DEVICE_CONTROLLER_H_

#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/Resource.h"
#include "ctrl-lib/EventListener.h"
#include "ctrl-lib/conf/ConfigManager.h"
#include "ctrl-lib/device/DeviceManager.h"
#include <cinttypes>
#include <vector>

namespace CalX {

	class DeviceController : public UsableResource {
	 public:
		DeviceController(ConfigManager &, Device &);
		virtual ~DeviceController() = default;
		device_id_t getID();
		ConfigManager &getConfiguration();
		Device &getDevice();

	 protected:
		ConfigManager &config;

	 private:
		Device &device;
	};
}  // namespace CalX

#endif
