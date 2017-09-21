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

#include "device/emulated/EmuDeviceManager.h"

namespace CalX {

	EmuDeviceManager::EmuDeviceManager() {
		this->motorConnectionType.push_back(DeviceConnectionType::SerialPort);
		this->instrumentConnectionType.push_back(DeviceConnectionType::SerialPort);
	}

	EmuDeviceManager::~EmuDeviceManager() {
		for (size_t i = 0; i < this->motors.size(); i++) {
			delete this->motors.at(i);
		}
		for (size_t i = 0; i < this->instruments.size(); i++) {
			delete this->instruments.at(i);
		}
	}

	void EmuDeviceManager::refresh() {}

	Motor *EmuDeviceManager::connectMotor(DeviceConnectionPrms *prms) {
		EmuMotor *motor = new EmuMotor((device_id_t) this->motors.size(), this);
		this->motors.push_back(motor);
		return motor;
	}

	Instrument *EmuDeviceManager::connectInstrument(DeviceConnectionPrms *prms) {
		EmuInstrument *instr =
		    new EmuInstrument((device_id_t) this->instruments.size(), this);
		this->instruments.push_back(instr);
		return instr;
	}

	extern "C" LIBEXPORT DeviceManager *getDeviceManager() {
		return new EmuDeviceManager();
	}
}  // namespace CalX
