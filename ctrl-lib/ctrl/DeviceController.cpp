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

#include "ctrl-lib/ctrl/DeviceController.h"

namespace CalX {

	DeviceController::DeviceController(ConfigManager &conf, Device &dev)
	    : config(conf), device(dev) {}

	device_id_t DeviceController::getID() {
		return this->device.getID();
	}

	ConfigManager &DeviceController::getConfiguration() {
		return this->config;
	}

	Device &DeviceController::getDevice() {
		return this->device;
	}
}  // namespace CalX
