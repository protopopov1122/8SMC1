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
	
	Device::Device(DeviceType t) {
		this->type = t;
	}
	
	Device::~Device() {
	}
	
	DeviceType Device::getType() {
		return this->type;
	}
	
	device_id_t Device::getID() {
		return this->dev;
	}
	
	ConfigManager *Device::getConfiguration() {
		return &this->config;
	}
	
	bool Device::hasErrors() {
		return !this->errors.empty();
	}
	
	std::string Device::pollError() {
		if (this->errors.empty()) {
			return "";
		}
		std::string err = this->errors.at(0);
		this->errors.erase(this->errors.begin());
		return err;
	}
	
	void Device::log(std::string msg) {
		LOG("Device #" + std::to_string(this->getID()), msg);
	}
	
	void Device::lock() {
	}
	
	void Device::unlock() {
	}
	
	bool Device::tryLock() {
		return true;
	}
	
	Motor::Motor()
		: Device::Device(DeviceType::Motor) {
			
	}

	Motor::~Motor() {
		
	}
	
	Instrument::Instrument()
		: Device::Device(DeviceType::Instrument) {
	}
	
	Instrument::~Instrument() {
		
	}
	
	void Instrument::log(std::string msg) {
		LOG_INSTR(this->getID(), msg);
	}
}
