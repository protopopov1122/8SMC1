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
	                             std::unique_ptr<ConfigManager> conf,
	                             std::unique_ptr<ExtEngine> ext_eng)
		: devman(std::move(devman)), conf(std::move(conf)),
			ext_engine(std::move(ext_eng)),
			planeSet(*this->conf, this), taskSet(this) {
		for (device_id_t d = 0; d < (device_id_t) this->devman->getMotorCount();
		     d++) {
			this->dev.push_back(std::make_shared<MotorController>(
			    this->getConfiguration(), *this->devman->getMotor(d)));
		}
		for (device_id_t i = 0;
		     i < (device_id_t) this->devman->getInstrumentCount(); i++) {
			this->instr.push_back(std::make_shared<InstrumentController>(
			    this->getConfiguration(), *this->devman->getInstrument(i)));
		}
		LOG(SYSMAN_TAG, "System startup. Found " +
		                    std::to_string(this->devman->getMotorCount()) +
		                    " motors and " +
		                    std::to_string(this->devman->getInstrumentCount()) +
		                    " instruments.");
		FunctionEngine_add_default_functions(this->engine);
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
		this->instr.clear();
		this->dev.clear();
		LOG(SYSMAN_TAG, "Sysman exited");
		DESTROY_LOG("SystemManager");
	}

	DeviceManager &SystemManager::getDeviceManager() const {
		return *this->devman;
	}

	ConfigManager &SystemManager::getConfiguration() const {
		return *this->conf;
	}

	ExtEngine &SystemManager::getExtEngine() const {
		return *this->ext_engine;
	}

	std::weak_ptr<MotorController> SystemManager::getMotorController(
	    device_id_t d) const {
		if (d >= (device_id_t) this->devman->getMotorCount() || d < 0) {
			return std::weak_ptr<MotorController>();
		}
		return this->dev.at((size_t) d);
	}

	FunctionEngine &SystemManager::getFunctionEngine() {
		return this->engine;
	}

	size_t SystemManager::getMotorCount() const {
		return this->devman->getMotorCount();
	}
	
	TaskSet &SystemManager::getTaskSet() {
		return this->taskSet;
	}

	CoordPlaneSet &SystemManager::getCoordPlaneSet() {
		return this->planeSet;
	}

	size_t SystemManager::getInstrumentCount() const {
		return this->devman->getInstrumentCount();
	}

	std::weak_ptr<InstrumentController> SystemManager::getInstrumentController(
	    device_id_t i) const {
		if (i >= (device_id_t) this->devman->getInstrumentCount() || i < 0) {
			return std::weak_ptr<InstrumentController>();
		}
		return this->instr.at((size_t) i);
	}

	std::weak_ptr<MotorController> SystemManager::connectMotor(
	    DeviceConnectionPrms *prms) {
		Motor *d = devman->connectMotor(prms);
		if (d == nullptr) {
			return std::weak_ptr<MotorController>();
		}
		devman->refresh();
		std::shared_ptr<MotorController> ctrl =
		    std::make_shared<MotorController>(this->getConfiguration(), *d);
		this->dev.push_back(ctrl);
		if (this->ext_engine != nullptr) {
			this->ext_engine->motorConnected(ctrl);
		}
		LOG(SYSMAN_TAG,
		    "Connected new device #" + std::to_string(this->dev.size() - 1));
		return ctrl;
	}

	std::weak_ptr<InstrumentController> SystemManager::connectInstrument(
	    DeviceConnectionPrms *prms) {
		Instrument *i = devman->connectInstrument(prms);
		if (i == nullptr) {
			return std::weak_ptr<InstrumentController>();
		}
		std::shared_ptr<InstrumentController> ctrl =
		    std::make_shared<InstrumentController>(this->getConfiguration(), *i);
		this->instr.push_back(ctrl);
		if (this->ext_engine != nullptr) {
			this->ext_engine->instrumentConnected(ctrl);
		}
		LOG(SYSMAN_TAG,
		    "Connected new instrument #" + std::to_string(this->instr.size() - 1));
		return ctrl;
	}
	
	void SystemManager::taskAdded(std::shared_ptr<CoordTask> task) {
		if (this->ext_engine != nullptr) {
			this->ext_engine->taskAdded(task);
		}
		LOG(SYSMAN_TAG, "Added new task #" +
		                    std::to_string(this->taskSet.getTaskCount() - 1) +
		                    ". Task count: " + std::to_string(this->taskSet.getTaskCount()));
	}
	
	void SystemManager::taskRemoved(std::size_t index) {
		if (this->ext_engine != nullptr) {
			this->ext_engine->taskRemoving(index);
		}

		LOG(SYSMAN_TAG, "Removed task # " + std::to_string(index) +
		                    ". Task count: " + std::to_string(this->taskSet.getTaskCount()));
	}
	
	void SystemManager::planeAdded(std::shared_ptr<CoordHandle> handle) {
		if (this->ext_engine != nullptr) {
			this->ext_engine->coordAdded(handle);
		}

		LOG(SYSMAN_TAG, "New coordinate plane #" +
		                    std::to_string(handle->getID()) +
		                    ". Devices: #" + std::to_string(handle->getController()->getXAxis()->getID()) + ", #" +
		                    std::to_string(handle->getController()->getYAxis()->getID()) + "; instrument: " +
		                    std::string(handle->getController()->getInstrument() != nullptr
		                                    ? "#" + std::to_string(handle->getController()->getInstrument()->getID())
		                                    : "no") +
		                    ".");
	}
	
	void SystemManager::planeRemoved(std::size_t index) {		
		if (this->ext_engine != nullptr) {
			this->ext_engine->coordRemoving(index);
		}
		LOG(SYSMAN_TAG, "Removed coord #" + std::to_string(index));
	}
}  // namespace CalX
