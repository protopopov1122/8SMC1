/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/device/emux/EmuDeviceManager.h"

namespace CalX {

	EmuDeviceManager::EmuDeviceManager() {
		this->motorConnectionType.push_back(DeviceConnectionType::SerialPort);
		this->instrumentConnectionType.push_back(DeviceConnectionType::SerialPort);
	}

	void EmuDeviceManager::refresh() {}

	Motor *EmuDeviceManager::connectMotor(DeviceConnectionPrms *prms) {
		std::unique_ptr<EmuMotor> motor =
		    std::make_unique<EmuMotor>((device_id_t) this->motors.size(), *this);
		this->motors.push_back(std::move(motor));
		return this->motors.at(this->motors.size() - 1).get();
	}

	Instrument *EmuDeviceManager::connectInstrument(DeviceConnectionPrms *prms) {
		std::unique_ptr<EmuInstrument> instr = std::make_unique<EmuInstrument>(
		    (device_id_t) this->instruments.size(), *this);
		this->instruments.push_back(std::move(instr));
		return this->instruments.at(this->instruments.size() - 1).get();
	}

	extern "C" LIBEXPORT DeviceManager *getDeviceManager() {
		return new EmuDeviceManager();
	}
}  // namespace CalX
