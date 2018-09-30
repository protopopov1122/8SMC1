/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

	SystemManagerEventLogger::SystemManagerEventLogger(SystemManager &sysman)
	    : sysman(sysman) {}

	void SystemManagerEventLogger::onTaskAdded(std::shared_ptr<CoordTask> task) {
		if (this->sysman.getExtensionEngine() != nullptr) {
			this->sysman.getExtensionEngine()->taskAdded(task);
		}
		LOG(SYSMAN_TAG,
		    "Added new task #" +
		        std::to_string(this->sysman.getTaskSet().getTaskCount() - 1) +
		        ". Task count: " +
		        std::to_string(this->sysman.getTaskSet().getTaskCount()));
	}

	void SystemManagerEventLogger::onTaskRemoving(std::size_t index) {
		if (this->sysman.getExtensionEngine() != nullptr) {
			this->sysman.getExtensionEngine()->taskRemoving(index);
		}

		LOG(SYSMAN_TAG,
		    "Removed task # " + std::to_string(index) + ". Task count: " +
		        std::to_string(this->sysman.getTaskSet().getTaskCount()));
	}

	void SystemManagerEventLogger::onPlaneAdded(
	    std::shared_ptr<CoordHandle> handle) {
		if (this->sysman.getExtensionEngine() != nullptr) {
			this->sysman.getExtensionEngine()->coordAdded(handle);
		}

		LOG(SYSMAN_TAG,
		    "New coordinate plane #" + std::to_string(handle->getID()) +
		        ". Devices: #" +
		        std::to_string(handle->getController()->getXAxis()->getID()) +
		        ", #" +
		        std::to_string(handle->getController()->getYAxis()->getID()) +
		        "; instrument: " +
		        std::string(
		            handle->getController()->getInstrument() != nullptr
		                ? "#" +
		                      std::to_string(
		                          handle->getController()->getInstrument()->getID())
		                : "no") +
		        ".");
	}

	void SystemManagerEventLogger::onPlaneRemoving(std::size_t index) {
		if (this->sysman.getExtensionEngine() != nullptr) {
			this->sysman.getExtensionEngine()->coordRemoving(index);
		}
		LOG(SYSMAN_TAG, "Removed coord #" + std::to_string(index));
	}

	void SystemManagerEventLogger::onDeviceConnected(
	    std::shared_ptr<DeviceController> controller) {
		if (controller->getDevice().getType() == DeviceType::Motor) {
			if (this->sysman.getExtensionEngine() != nullptr) {
				this->sysman.getExtensionEngine()->motorConnected(
				    std::dynamic_pointer_cast<MotorController>(controller));
			}
			LOG(SYSMAN_TAG,
			    "Connected new motor #" + std::to_string(controller->getID()));
		} else {
			if (this->sysman.getExtensionEngine() != nullptr) {
				this->sysman.getExtensionEngine()->instrumentConnected(
				    std::dynamic_pointer_cast<InstrumentController>(controller));
			}
			LOG(SYSMAN_TAG,
			    "Connected new instrument #" + std::to_string(controller->getID()));
		}
	}

	DefaultSystemManager::DefaultSystemManager(
	    std::unique_ptr<DeviceManager> devman,
	    std::unique_ptr<ConfigManager> conf, std::unique_ptr<ExtEngine> ext_eng)
	    : devman(std::move(devman)),
	      conf(std::move(conf)),
	      ext_engine(std::move(ext_eng)),
	      eventLogger(*this),
	      motorControllerSet(*this->conf, *this->devman, &this->eventLogger),
	      instrumentControllerSet(*this->conf, *this->devman, &this->eventLogger),
	      planeSet(*this->conf, &this->eventLogger),
	      taskSet(&this->eventLogger) {
		LOG(SYSMAN_TAG,
		    "System startup. Found " +
		        std::to_string(this->motorControllerSet.getDeviceCount()) +
		        " motors and " +
		        std::to_string(this->instrumentControllerSet.getDeviceCount()) +
		        " instruments.");
		if (this->ext_engine != nullptr) {
			this->ext_engine->init(this);
		}
		INIT_LOG("DefaultSystemManager");
	}

	DefaultSystemManager::~DefaultSystemManager() {
		LOG(SYSMAN_TAG, "System exiting");
		if (this->ext_engine != nullptr) {
			this->ext_engine->destroy();
		}
		LOG(SYSMAN_TAG, "Sysman exited");
		DESTROY_LOG("DefaultSystemManager");
	}

	DeviceManager &DefaultSystemManager::getDeviceManager() const {
		return *this->devman;
	}

	ConfigManager &DefaultSystemManager::getConfiguration() const {
		return *this->conf;
	}

	ExtEngine *DefaultSystemManager::getExtensionEngine() const {
		return this->ext_engine.get();
	}

	MathEngine &DefaultSystemManager::getMathEngine() {
		return this->engine;
	}

	TaskSet &DefaultSystemManager::getTaskSet() {
		return this->taskSet;
	}

	CoordPlaneSet &DefaultSystemManager::getCoordPlaneSet() {
		return this->planeSet;
	}

	MotorControllerSet &DefaultSystemManager::getMotorControllerSet() {
		return this->motorControllerSet;
	}

	InstrumentControllerSet &DefaultSystemManager::getInstrumentControllerSet() {
		return this->instrumentControllerSet;
	}
}  // namespace CalX
