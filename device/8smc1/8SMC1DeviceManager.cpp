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


#include "8SMC1DeviceManager.h"
#include <stdio.h>
#include <iostream>


namespace CalX {
	
	CALXDeviceManager::CALXDeviceManager()
		: DeviceManager::DeviceManager() {
		this->refresh();
		for (size_t i = 0; i < devs.NOD; i++) {
			this->dev.push_back(new CALXDevice((device_id_t) i, this));
		}
		
		this->instr.push_back(new CALXInstrument(0, this));
	}

	CALXDeviceManager::~CALXDeviceManager() {
		for (size_t i = 0; i < this->dev.size(); i++) {
			delete this->dev.at(i);
		}
		for (size_t i = 0; i < this->instr.size(); i++) {
			delete this->instr.at(i);
		}
		this->dev.clear();
		if (USMC_Close()) {
			saveError();
		}
	}

	void CALXDeviceManager::refresh() {
		if (USMC_Init(this->devs)) {
			saveError();
		}
	}

	void CALXDeviceManager::saveError() {
		char er[101];
		USMC_GetLastErr(er,100);
		er[100] = '\0';
		this->error_queue.push_back(std::string(er));
	}

	std::string CALXDeviceManager::getDeviceSerial(device_id_t id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Serial[id]);
	}

	std::string CALXDeviceManager::getDeviceVersion(device_id_t id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Version[id]);
	}
	
	extern "C" LIBEXPORT DeviceManager *getDeviceManager() {
		return new CALXDeviceManager();
	}
}
