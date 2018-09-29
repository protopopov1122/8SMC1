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
			motorControllerSet(*this->conf, *this->devman, this),
			instrumentControllerSet(*this->conf, *this->devman, this),
			planeSet(*this->conf, this), taskSet(this) {
		LOG(SYSMAN_TAG, "System startup. Found " +
		                    std::to_string(this->motorControllerSet.getDeviceCount()) +
		                    " motors and " +
		                    std::to_string(this->instrumentControllerSet.getDeviceCount()) +
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

	FunctionEngine &SystemManager::getFunctionEngine() {
		return this->engine;
	}
	
	TaskSet &SystemManager::getTaskSet() {
		return this->taskSet;
	}

	CoordPlaneSet &SystemManager::getCoordPlaneSet() {
		return this->planeSet;
	}
	
	MotorControllerSet &SystemManager::getMotorControllerSet() {
		return this->motorControllerSet;
	}
	
	InstrumentControllerSet &SystemManager::getInstrumentControllerSet() {
		return this->instrumentControllerSet;
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
	
	void SystemManager::deviceAdded(std::shared_ptr<DeviceController> controller) {
		if (controller->getDevice().getType() == DeviceType::Motor) {
			if (this->ext_engine != nullptr) {
				this->ext_engine->motorConnected(std::dynamic_pointer_cast<MotorController>(controller));
			}
			LOG(SYSMAN_TAG,
				"Connected new motor #" + std::to_string(controller->getID()));
		} else {
			if (this->ext_engine != nullptr) {
				this->ext_engine->instrumentConnected(std::dynamic_pointer_cast<InstrumentController>(controller));
			}
			LOG(SYSMAN_TAG,
				"Connected new instrument #" + std::to_string(controller->getID()));
		}
	}
}  // namespace CalX
