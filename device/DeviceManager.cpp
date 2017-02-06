#include "DeviceManager.h"
#include <stdio.h>
#include <iostream>


namespace Controller {
	DeviceManager::DeviceManager() {
		refresh();
		for (DWORD d = 0; d < devs.NOD; d++) {
			this->dev.push_back(new Device(d, this));
		}
		this->last_device = nullptr;
	}

	DeviceManager::~DeviceManager() {
		for (DWORD d = 0; d < devs.NOD; d++) {
			delete this->dev.at(d);
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

	void DeviceManager::setLastDevice(Device *dev) {
		this->last_device = dev;
	}

	Device *DeviceManager::getLastDevice() {
		return this->last_device;
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
