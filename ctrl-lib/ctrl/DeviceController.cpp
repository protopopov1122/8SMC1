/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/ctrl/DeviceController.h"

namespace CalX {

	DeviceController::DeviceController(ConfigurationCatalogue &conf, Device &dev)
	    : UsableResource::UsableResource(), config(conf), device(dev) {}

	device_id_t DeviceController::getID() const {
		return this->device.getID();
	}

	ConfigurationCatalogue &DeviceController::getConfiguration() const {
		return this->config;
	}

	Device &DeviceController::getDevice() const {
		return this->device;
	}
}  // namespace CalX
