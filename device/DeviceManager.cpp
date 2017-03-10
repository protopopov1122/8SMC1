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
