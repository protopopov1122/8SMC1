#include "SystemManager.h"

namespace Controller {

	SystemManager::SystemManager(DeviceManager *devman) {
		this->devman = devman;
		for (DWORD d = 0; d < devman->getDeviceCount(); d++) {
			this->dev.push_back(new DeviceController(devman->getDevice(d)));
		}
	}

	SystemManager::~SystemManager() {
		for (size_t i = 0; i < this->tasks.size(); i++) {
			delete this->tasks.at(i);
		}
		for (size_t i = 0; i < this->coords.size(); i++) {
			delete this->coords.at(i);
		}
		for (DWORD d = 0; d < this->devman->getDeviceCount(); d++) {
			delete this->dev.at(d);
		}
	}

	DeviceManager *SystemManager::getDeviceManager() {
		return this->devman;
	}

	DeviceController *SystemManager::getDeviceController(DWORD d) {
		if (d >= this->devman->getDeviceCount()) {
			return nullptr;
		}
		return this->dev.at(d);
	}

	size_t SystemManager::getDeviceCount() {
		return this->devman->getDeviceCount();
	}

	size_t SystemManager::getTaskCount() {
		return this->tasks.size();
	}

	CoordTask *SystemManager::getTask(size_t i) {
		if (i >= this->tasks.size()) {
			return nullptr;
		}
		return this->tasks.at(i);
	}

	CoordTask *SystemManager::createTask() {
		CoordTask *task = new CoordTask();
		this->tasks.push_back(task);
		return task;
	}

	bool SystemManager::removeTask(size_t i) {
		if (i >= this->tasks.size()) {
			return false;
		}
		delete this->tasks.at(i);
		this->tasks.erase(this->tasks.begin() + i);
		return true;
	}

	size_t SystemManager::getCoordCount() {
		return this->coords.size();
	}

	CoordController *SystemManager::getCoord(size_t c) {
		if (c >= this->coords.size()) {
			return nullptr;
		}
		return this->coords.at(c);
	}

	CoordController *SystemManager::createCoord(DWORD d1, DWORD d2) {
		if (d1 >= this->devman->getDeviceCount() || d2 >= this->devman->getDeviceCount()) {
			return nullptr;
		}

		CoordController *ctrl = new CoordController(this->getDeviceController(d1),
			this->getDeviceController(d2));
		this->coords.push_back(ctrl);
		return ctrl;
	}
}
