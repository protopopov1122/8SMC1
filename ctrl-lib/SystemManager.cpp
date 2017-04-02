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


#include "SystemManager.h"

namespace CalX {

	const char *SYSMAN_TAG = "SysMan";

	SystemManager::SystemManager(DeviceManager *devman, ConfigManager *conf) {
		this->devman = devman;
		this->conf = conf;
		for (device_id_t d = 0; d < devman->getDeviceCount(); d++) {
			this->dev.push_back(new DeviceController(devman->getDevice(d), this->conf));
		}
		for (device_id_t i = 0; i < devman->getInstrumentCount(); i++) {
			this->instr.push_back(new InstrumentController(devman->getInstrument(i)));
		}
		LOG(SYSMAN_TAG, "System startup. Found " + std::to_string(devman->getDeviceCount()) + " motors and " + std::to_string(devman->getInstrumentCount()) + " instruments.");
		FunctionEngine_add_default_functions(&this->engine);
		INIT_LOG("SystemManager");
	}

	SystemManager::~SystemManager() {
		LOG(SYSMAN_TAG, "System exiting");
		for (size_t i = 0; i < this->tasks.size(); i++) {
			delete this->tasks.at(i);
		}
		for (size_t i = 0; i < this->coords.size(); i++) {
			delete this->coords.at(i);
		}
		for (device_id_t i = 0; i < this->devman->getInstrumentCount(); i++) {
			delete this->instr.at(i);
		}
		for (device_id_t d = 0; d < this->devman->getDeviceCount(); d++) {
			delete this->dev.at(d);
		}
		delete this->conf;
		DESTROY_LOG("SystemManager");
	}

	DeviceManager *SystemManager::getDeviceManager() {
		return this->devman;
	}
	
	ConfigManager *SystemManager::getConfiguration() {
		return this->conf;
	}

	DeviceController *SystemManager::getDeviceController(device_id_t d) {
		if (d >= this->devman->getDeviceCount() || d < 0) {
			return nullptr;
		}
		return this->dev.at(d);
	}
	
	FunctionEngine *SystemManager::getFunctionEngine() {
		return &this->engine;
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
	
	size_t SystemManager::addTask(CoordTask *task) {
		this->tasks.push_back(task);
		LOG(SYSMAN_TAG, "Added new task #" + std::to_string(this->tasks.size() - 1) + ". Task count: " + std::to_string(this->tasks.size()));
		return this->tasks.size() - 1;
	}

	ProgrammedCoordTask *SystemManager::createProgrammedTask() {
		ProgrammedCoordTask *task = new ProgrammedCoordTask();
		this->tasks.push_back(task);
		LOG(SYSMAN_TAG, "Added new programmed task #" + std::to_string(this->tasks.size() - 1) + ". Task count: " + std::to_string(this->tasks.size()));
		return task;
	}

	bool SystemManager::removeTask(size_t i) {
		if (i >= this->tasks.size()) {
			return false;
		}
		delete this->tasks.at(i);
		this->tasks.erase(this->tasks.begin() + i);
		LOG(SYSMAN_TAG, "Removed task # " + std::to_string(i) + ". Task count: " + std::to_string(this->tasks.size()));
		return true;
	}

	size_t SystemManager::getCoordCount() {
		return this->coords.size();
	}

	CoordHandle *SystemManager::getCoord(size_t c) {
		if (c >= this->coords.size()) {
			return nullptr;
		}
		return this->coords.at(c);
	}

	CoordHandle *SystemManager::createCoord(device_id_t d1, device_id_t d2, device_id_t instr) {
		if (d1 >= this->devman->getDeviceCount() || d2 >= this->devman->getDeviceCount()) {
			return nullptr;
		}

		CoordController *ctrl = new CoordController(this->getDeviceController(d1),
			this->getDeviceController(d2), this->conf, this->getInstrumentController(instr));
		CoordHandle *handle = new CoordHandle(this->coords.size(), ctrl);
		this->coords.push_back(handle);
		LOG(SYSMAN_TAG, "New coordinate plane #" + std::to_string(this->coords.size() - 1) + ". Devices: #" + std::to_string(d1) + ", #" + std::to_string(d2) + "; instrument: " + std::string(getInstrumentController(instr) != nullptr ? "#" + std::to_string(instr) : "no") + ".");
		return handle;
	}
	
	void SystemManager::removeCoord(size_t id) {
		if (id < this->coords.size()) {
			delete this->coords.at(id);
			this->coords.erase(this->coords.begin() + id);
			LOG(SYSMAN_TAG, "Removed coord #" + id);
		}
	}
	
	size_t SystemManager::getInstrumentCount() {
		return this->devman->getInstrumentCount();
	}
	
	InstrumentController *SystemManager::getInstrumentController(device_id_t i) {
		if (i >= this->devman->getInstrumentCount() || i < 0) {
			return nullptr;
		}
		return this->instr.at(i);
	}
	
	DeviceController *SystemManager::connectDevice(DeviceConnectionPrms *prms) {
		Device *d = devman->connectDevice(prms);
		if (d == nullptr) {
			return nullptr;
		}
		devman->refresh();
		DeviceController *ctrl = new DeviceController(d, this->conf);
		this->dev.push_back(ctrl);
		LOG(SYSMAN_TAG, "Connected new device #" + std::to_string(this->dev.size() - 1));
		return ctrl;
	}
	
	InstrumentController *SystemManager::connectInstrument(DeviceConnectionPrms *prms) {
		Instrument *i = devman->connectInstrument(prms);
		if (i == nullptr) {
			return nullptr;
		}
		devman->refresh();
		InstrumentController *ctrl = new InstrumentController(i);
		this->instr.push_back(ctrl);
		LOG(SYSMAN_TAG, "Connected new instrument #" + std::to_string(this->instr.size() - 1));
		return ctrl;
	}
}
