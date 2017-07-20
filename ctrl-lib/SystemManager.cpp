/*
        Copyright (c) 2017 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ctrl-lib/SystemManager.h"

namespace CalX {

	const char *SYSMAN_TAG = "SysMan";

	SystemManager::SystemManager(std::unique_ptr<DeviceManager> devman,
	                             std::shared_ptr<ConfigManager> conf,
	                             std::unique_ptr<ExtEngine> ext_eng) {
		this->devman = std::move(devman);
		this->conf = conf;
		this->ext_engine = std::move(ext_eng);
		for (device_id_t d = 0; d < (device_id_t) this->devman->getMotorCount(); d++) {
			this->dev.push_back(std::make_shared<MotorController>(
			    this->devman->getMotor(d), this->getConfiguration().get()));
		}
		for (device_id_t i = 0; i < (device_id_t) this->devman->getInstrumentCount();
		     i++) {
			this->instr.push_back(std::make_shared<InstrumentController>(
			    this->devman->getInstrument(i)));
		}
		LOG(SYSMAN_TAG,
		    "System startup. Found " + std::to_string(this->devman->getMotorCount()) +
		        " motors and " + std::to_string(this->devman->getInstrumentCount()) +
		        " instruments.");
		FunctionEngine_add_default_functions(&this->engine);
		this->resolver = std::make_unique<RequestResolver>(this);
		RequestResolver_init_standart_providers(this->resolver.get());
		if (this->ext_engine != nullptr) {
			this->ext_engine->init(this);
		}
		INIT_LOG("SystemManager");
	}

	SystemManager::~SystemManager() {
		LOG(SYSMAN_TAG, "System exiting");
		if (this->ext_engine != nullptr) {
			this->ext_engine->destroy();
		}
		this->tasks.clear();
		this->coords.clear();
		this->instr.clear();
		this->dev.clear();
		LOG(SYSMAN_TAG, "Sysman exited");
		DESTROY_LOG("SystemManager");
	}

	DeviceManager *SystemManager::getDeviceManager() {
		return this->devman.get();
	}

	std::shared_ptr<ConfigManager> SystemManager::getConfiguration() {
		return this->conf;
	}

	ExtEngine *SystemManager::getExtEngine() {
		return this->ext_engine.get();
	}

	std::shared_ptr<MotorController> SystemManager::getMotorController(device_id_t d) {
		if (d >= (device_id_t) this->devman->getMotorCount() || d < 0) {
			return nullptr;
		}
		return this->dev.at((size_t) d);
	}

	FunctionEngine *SystemManager::getFunctionEngine() {
		return &this->engine;
	}

	RequestResolver *SystemManager::getRequestResolver() {
		return this->resolver.get();
	}

	size_t SystemManager::getMotorCount() {
		return this->devman->getMotorCount();
	}

	size_t SystemManager::getTaskCount() {
		return this->tasks.size();
	}

	CoordTask *SystemManager::getTask(size_t i) {
		if (i >= this->tasks.size()) {
			return nullptr;
		}
		return this->tasks.at(i).get();
	}

	size_t SystemManager::addTask(std::unique_ptr<CoordTask> task) {
		this->tasks.push_back(std::move(task));
		if (this->ext_engine != nullptr) {
			this->ext_engine->taskAdded(this->tasks.at(this->tasks.size() - 1).get());
		}
		LOG(SYSMAN_TAG,
		    "Added new task #" + std::to_string(this->tasks.size() - 1) +
		        ". Task count: " + std::to_string(this->tasks.size()));
		return this->tasks.size() - 1;
	}

	ProgrammedCoordTask *SystemManager::createProgrammedTask() {
		this->tasks.push_back(std::make_unique<ProgrammedCoordTask>());
		ProgrammedCoordTask *ptr =
		    (ProgrammedCoordTask *) this->getTask(this->getTaskCount() - 1);
		if (this->ext_engine != nullptr) {
			this->ext_engine->taskAdded(ptr);
		}
		LOG(SYSMAN_TAG,
		    "Added new programmed task #" + std::to_string(this->tasks.size() - 1) +
		        ". Task count: " + std::to_string(this->tasks.size()));
		return ptr;
	}

	bool SystemManager::removeTask(size_t i) {
		if (i >= this->tasks.size()) {
			return false;
		}
		if (this->ext_engine != nullptr) {
			this->ext_engine->taskRemoving(i);
		}

		this->tasks.erase(this->tasks.begin() + (std::ptrdiff_t) i);
		LOG(SYSMAN_TAG,
		    "Removed task # " + std::to_string(i) +
		        ". Task count: " + std::to_string(this->tasks.size()));
		return true;
	}

	size_t SystemManager::getCoordCount() {
		return this->coords.size();
	}

	CoordHandle *SystemManager::getCoord(size_t c) {
		if (c >= this->coords.size()) {
			return nullptr;
		}
		return this->coords.at(c).get();
	}

	CoordHandle *SystemManager::createCoord(device_id_t d1, device_id_t d2,
	                                        device_id_t instr) {
		if (d1 >= (device_id_t) this->devman->getMotorCount() ||
		    d2 >= (device_id_t) this->devman->getMotorCount()) {
			return nullptr;
		}

		std::shared_ptr<CoordController> ctrl = std::make_shared<CoordController>(
		    this->getMotorController(d1), this->getMotorController(d2),
		    this->getConfiguration().get(), this->getInstrumentController(instr));
		std::unique_ptr<CoordHandle> handle =
		    std::make_unique<CoordHandle>(this->coords.size(), ctrl);
		if (getConfiguration()->getEntry("core")->getBool("auto_power_motors",
		                                                  false)) {
			ctrl->getXAxis()->enablePower(true);
			ctrl->getYAxis()->enablePower(true);
		}
		this->coords.push_back(std::move(handle));
		CoordHandle *ptr = this->getCoord(this->getCoordCount() - 1);
		if (this->ext_engine != nullptr) {
			this->ext_engine->coordAdded(ptr);
		}
		LOG(SYSMAN_TAG,
		    "New coordinate plane #" + std::to_string(this->coords.size() - 1) +
		        ". Devices: #" + std::to_string(d1) + ", #" + std::to_string(d2) +
		        "; instrument: " + std::string(getInstrumentController(instr) !=
		                                               nullptr
		                                           ? "#" + std::to_string(instr)
		                                           : "no") +
		        ".");
		return ptr;
	}

	void SystemManager::removeCoord(size_t id) {
		if (id < this->coords.size()) {
			if (this->ext_engine != nullptr) {
				this->ext_engine->coordRemoving(id);
			}
			this->coords.erase(this->coords.begin() + (std::ptrdiff_t) id);
			LOG(SYSMAN_TAG, "Removed coord #" + std::to_string(id));
		}
	}

	size_t SystemManager::getInstrumentCount() {
		return this->devman->getInstrumentCount();
	}

	std::shared_ptr<InstrumentController> SystemManager::getInstrumentController(device_id_t i) {
		if (i >= (device_id_t) this->devman->getInstrumentCount() || i < 0) {
			return nullptr;
		}
		return this->instr.at((size_t) i);
	}

	std::shared_ptr<MotorController> SystemManager::connectMotor(DeviceConnectionPrms *prms) {
		Motor *d = devman->connectMotor(prms);
		if (d == nullptr) {
			return nullptr;
		}
		devman->refresh();
		std::shared_ptr<MotorController> ctrl =
		    std::make_shared<MotorController>(d, this->getConfiguration().get());
		this->dev.push_back(ctrl);
		if (this->ext_engine != nullptr) {
			this->ext_engine->motorConnected(ctrl.get());
		}
		LOG(SYSMAN_TAG,
		    "Connected new device #" + std::to_string(this->dev.size() - 1));
		return ctrl;
	}

	std::shared_ptr<InstrumentController> SystemManager::connectInstrument(
	    DeviceConnectionPrms *prms) {
		Instrument *i = devman->connectInstrument(prms);
		if (i == nullptr) {
			return nullptr;
		}
		std::shared_ptr<InstrumentController> ctrl =
		    std::make_shared<InstrumentController>(i);
		this->instr.push_back(ctrl);
		if (this->ext_engine != nullptr) {
			this->ext_engine->instrumentConnected(ctrl.get());
		}
		LOG(SYSMAN_TAG,
		    "Connected new instrument #" + std::to_string(this->instr.size() - 1));
		return ctrl;
	}
}
