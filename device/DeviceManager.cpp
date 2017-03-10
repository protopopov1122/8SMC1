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


#include "DeviceManager.h"
#include <stdio.h>
#include <iostream>


namespace CalX {
	DeviceManager::~DeviceManager() {
		
	}
	
	bool DeviceManager::hasError() {
		return !this->error_queue.empty();
	}
	
	std::string DeviceManager::pollError() {
		if (!hasError()) {
			return "";
		}
		std::string err = this->error_queue.at(0);
		this->error_queue.erase(this->error_queue.begin());
		return err;
	}
	
	Device *DeviceManager::getDevice(device_id_t d) {
		if (d >= this->dev.size()) {
			return nullptr;
		}
		return this->dev.at(d);
	}

	size_t DeviceManager::getDeviceCount() {
		return this->dev.size();
	}
	
}
