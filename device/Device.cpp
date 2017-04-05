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

/* Implementations of API wrappers */

namespace CalX {

	Device::~Device() {
		
	}

	DeviceManager *Device::getDeviceManager() {
		return this->devman;
	}

	device_id_t Device::getID() {
		return this->dev;
	}

	std::string Device::getSerial() {
		return this->devman->getDeviceSerial(this->dev);
	}

	std::string Device::getVersion() {
		return this->devman->getDeviceVersion(this->dev);
	}
	
	Instrument::Instrument() {
	}
	
	Instrument::~Instrument() {
		
	}
	
	device_id_t Instrument::getID() {
		return this->dev;
	}
	
	DeviceManager *Instrument::getDeviceManager() {
		return this->devman;
	}
	
	bool Instrument::hasErrors() {
		return this->errors.size() != 0;
	}
	
	std::string Instrument::pollError() {
		if (this->errors.size() == 0) {
			return "";
		}
		std::string err = this->errors.at(0);
		this->errors.erase(this->errors.begin());
		return err;
	}

	void Instrument::getModes(std::vector<std::string> &vec) {
		for (const auto& mode : this->modes) {
			vec.push_back(mode);
		}
	}
}
