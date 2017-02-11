#include "DeviceController.h"
#include "CoordTask.h"
#include <stdio.h>
#include <iostream>


namespace Controller {
	DeviceManager::DeviceManager() {
		this->refresh();
		for (DWORD d = 0; d < devs.NOD; d++) {
			this->dev.push_back(new DeviceController(new Device(d, this)));
		}
		this->last_device = nullptr;
	}

	DeviceManager::~DeviceManager() {
		for (size_t i = 0; i < this->tasks.size(); i++) {
			delete this->tasks.at(i);
		}
		for (size_t i = 0; i < this->coords.size(); i++) {
			delete this->coords.at(i);
		}
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
		return this->dev.at(d)->getDevice();
	}

	DeviceController *DeviceManager::getDeviceController(DWORD d) {
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

	size_t DeviceManager::getCoordCount() {
		return this->coords.size();
	}

	CoordController *DeviceManager::getCoord(size_t c) {
		if (c >= this->coords.size()) {
			return nullptr;
		}
		return this->coords.at(c);
	}

	CoordController *DeviceManager::createCoord(DWORD d1, DWORD d2) {
		if (d1 >= this->devs.NOD || d2 >= this->devs.NOD) {
			return nullptr;
		}

		CoordController *ctrl = new CoordController(this->dev.at(d1), this->dev.at(d2));
		this->coords.push_back(ctrl);
		return ctrl;
	}

	size_t DeviceManager::getTaskCount() {
		return this->tasks.size();
	}

	CoordTask *DeviceManager::getTask(size_t i) {
		if (i >= this->tasks.size()) {
			return nullptr;
		}
		return this->tasks.at(i);
	}

	CoordTask *DeviceManager::createTask() {
		CoordTask *task = new CoordTask();
		this->tasks.push_back(task);
		return task;
	}

	bool DeviceManager::removeTask(size_t i) {
		if (i >= this->tasks.size()) {
			return false;
		}
		delete this->tasks.at(i);
		this->tasks.erase(this->tasks.begin() + i);
		return true;
	}
}
