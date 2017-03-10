#include "8SMC1DeviceManager.h"
#include <stdio.h>
#include <iostream>


namespace _8SMC1 {
	
	_8SMC1DeviceManager::_8SMC1DeviceManager()
		: DeviceManager::DeviceManager() {
		this->refresh();
		for (size_t i = 0; i < devs.NOD; i++) {
			this->dev.push_back(new _8SMC1Device((device_id_t) i, this));
		}
	}

	_8SMC1DeviceManager::~_8SMC1DeviceManager() {
		for (size_t i = 0; i < this->dev.size(); i++) {
			delete this->dev.at(i);
		}
		this->dev.clear();
		if (USMC_Close()) {
			saveError();
		}
	}

	void _8SMC1DeviceManager::refresh() {
		if (USMC_Init(this->devs)) {
			saveError();
		}
	}

	void _8SMC1DeviceManager::saveError() {
		char er[101];
		USMC_GetLastErr(er,100);
		er[100] = '\0';
		this->error_queue.push_back(std::string(er));
	}

	std::string _8SMC1DeviceManager::getDeviceSerial(device_id_t id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Serial[id]);
	}

	std::string _8SMC1DeviceManager::getDeviceVersion(device_id_t id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Version[id]);
	}
}
