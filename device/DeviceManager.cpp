#include "DeviceController.h"
#include "CoordTask.h"
#include <stdio.h>
#include <iostream>


namespace _8SMC1 {
	DeviceManager::DeviceManager() {
		this->refresh();
		for (size_t i = 0; i < devs.NOD; i++) {
			this->dev.push_back(new Device((DWORD) i, this));
		}
	}

	DeviceManager::~DeviceManager() {
		for (size_t i = 0; i < this->dev.size(); i++) {
			delete this->dev.at(i);
		}
		this->dev.clear();
		if (USMC_Close()) {
			saveError();
		}
	}

	void DeviceManager::refresh() {
		if (USMC_Init(this->devs)) {
			saveError();
		}
	}

	void DeviceManager::saveError() {
		char er[101];
		USMC_GetLastErr(er,100);
		er[100] = '\0';
		this->error_queue.push_back(std::string(er));
	}

	Device *DeviceManager::getDevice(DWORD d) {
		if (d >= this->devs.NOD) {
			return nullptr;
		}
		return this->dev.at(d);
	}

	unsigned int DeviceManager::getDeviceCount() {
		return this->devs.NOD;
	}

	std::string DeviceManager::getDeviceSerial(DWORD id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Serial[id]);
	}

	std::string DeviceManager::getDeviceVersion(DWORD id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Version[id]);
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
}
