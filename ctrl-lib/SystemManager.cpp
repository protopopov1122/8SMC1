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

	SystemManager::SystemManager(DeviceManager *devman) {
		this->devman = devman;
		for (device_id_t d = 0; d < devman->getDeviceCount(); d++) {
			this->dev.push_back(new DeviceController(devman->getDevice(d)));
		}
		for (device_id_t i = 0; i < devman->getInstrumentCount(); i++) {
			this->instr.push_back(new InstrumentController(devman->getInstrument(i)));
		}
		FunctionEngine_add_default_functions(&this->engine);
	}

	SystemManager::~SystemManager() {
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
	}

	DeviceManager *SystemManager::getDeviceManager() {
		return this->devman;
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
		return this->tasks.size() - 1;
	}

	ProgrammedCoordTask *SystemManager::createProgrammedTask() {
		ProgrammedCoordTask *task = new ProgrammedCoordTask();
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
			this->getDeviceController(d2), this->getInstrumentController(instr));
		CoordHandle *handle = new CoordHandle(this->coords.size(), ctrl);
		this->coords.push_back(handle);
		return handle;
	}
	
	void SystemManager::removeCoord(size_t id) {
		if (id < this->coords.size()) {
			delete this->coords.at(id);
			this->coords.erase(this->coords.begin() + id);
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
	
}
