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

#include "ctrl-lib/device/DeviceManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdio.h>

namespace CalX {
	DeviceManager::~DeviceManager() {}

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

	Motor *DeviceManager::getMotor(device_id_t d) {
		if (d >= (device_id_t) this->motors.size() || d < 0) {
			return nullptr;
		}
		return this->motors.at((size_t) d);
	}

	size_t DeviceManager::getMotorCount() {
		return this->motors.size();
	}

	size_t DeviceManager::getInstrumentCount() {
		return this->instruments.size();
	}

	Instrument *DeviceManager::getInstrument(device_id_t id) {
		if (id >= (device_id_t) this->instruments.size() || id < 0) {
			return nullptr;
		}
		return this->instruments.at((size_t) id);
	}

	void DeviceManager::getConnectionTypes(
	    std::vector<DeviceConnectionType> &dev,
	    std::vector<DeviceConnectionType> &instr) {
		for (const auto &d : this->motorConnectionType) {
			dev.push_back(d);
		}
		for (const auto &i : this->instrumentConnectionType) {
			instr.push_back(i);
		}
	}

	bool DeviceManager::canMotorConnect(DeviceConnectionType d) {
		return std::find(motorConnectionType.begin(), motorConnectionType.end(),
		                 d) != motorConnectionType.end();
	}

	bool DeviceManager::canInstrumentConnect(DeviceConnectionType d) {
		return std::find(instrumentConnectionType.begin(),
		                 instrumentConnectionType.end(),
		                 d) != instrumentConnectionType.end();
	}

	bool DeviceManager::loadConfiguration(std::string path, ConfigManager *conf) {
		std::ifstream cnf(path);
		if (!cnf.good()) {
			return false;
		} else {
			ConfigManager::load(&cnf, &std::cout, conf);
		}
		cnf.close();
		return true;
	}

	void DeviceManager::log(std::string msg) {
		LOG("DevMan", msg);
	}

	void DeviceManager::lock() {}

	void DeviceManager::unlock() {}

	bool DeviceManager::tryLock() {
		return true;
	}

	void DeviceManager::terminate() {
		for (const auto &instr : this->instruments) {
			instr->terminate();
		}
		for (const auto &motor : this->motors) {
			motor->terminate();
		}
	}
}
