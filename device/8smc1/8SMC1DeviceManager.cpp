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
	}

	CALXDeviceManager::~CALXDeviceManager() {
		for (size_t i = 0; i < this->dev.size(); i++) {
			delete this->dev.at(i);
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
}
