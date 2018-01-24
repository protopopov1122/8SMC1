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

#include "device/standart/StandartDeviceManager.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

namespace CalX {

	StandartDeviceManager::StandartDeviceManager()
	    : DeviceManager::DeviceManager() {
		this->refresh();
		for (size_t i = 0; i < devs.NOD; i++) {
			this->motors.push_back(
			    std::make_unique<_8SMC1Motor>((device_id_t) i, *this));
		}

		this->instrumentConnectionType.push_back(DeviceConnectionType::SerialPort);
	}

	StandartDeviceManager::~StandartDeviceManager() {
		this->instruments.clear();
		this->motors.clear();
		if (USMC_Close()) {
			saveMotorError();
		}
	}

	void StandartDeviceManager::refresh() {
		if (USMC_Init(this->devs)) {
			saveMotorError();
		}
	}

	void StandartDeviceManager::saveMotorError() {
		char er[101];
		do {
			USMC_GetLastErr(er, 100);
			er[100] = '\0';
			if (strlen(er) > 0) {
				this->error_queue.push_back(std::string(er));
			}
		} while (strlen(er) > 0);

		for (size_t i = 0; i < this->motors.size(); i++) {
			Motor *m = this->motors.at(i).get();
			while (m->hasErrors()) {
				this->error_queue.push_back(m->pollError());
			}
		}
	}

	void StandartDeviceManager::saveInstrumentError() {
		for (size_t i = 0; i < this->instruments.size(); i++) {
			Instrument *ins = this->instruments.at(i).get();
			while (ins->hasErrors()) {
				this->error_queue.push_back(ins->pollError());
			}
		}
	}

	std::string StandartDeviceManager::getMotorSerial(device_id_t id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Serial[id]);
	}

	std::string StandartDeviceManager::getMotorVersion(device_id_t id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Version[id]);
	}

	Motor *StandartDeviceManager::connectMotor(DeviceConnectionPrms *prms) {
		return nullptr;
	}

	Instrument *StandartDeviceManager::connectInstrument(
	    DeviceConnectionPrms *_prms) {
		if (_prms->type != DeviceConnectionType::SerialPort) {
			return nullptr;
		}
		DeviceSerialPortConnectionPrms *prms =
		    (DeviceSerialPortConnectionPrms *) _prms;
		this->log("Connecting NL300 instrument on COM" +
		          std::to_string(prms->port));
		std::unique_ptr<NL300Instrument> instr = std::make_unique<NL300Instrument>(
		    (device_id_t) this->instruments.size(), *this);
		if (!instr->connect(prms) || instr->hasErrors()) {
			this->log("Error during NL300 instrument connection on COM" +
			          std::to_string(prms->port));
			this->saveInstrumentError();
			return nullptr;
		}
		this->instruments.push_back(std::move(instr));
		this->log("Connected NL300 instrument on COM" + std::to_string(prms->port));
		return this->instruments.at(this->instruments.size() - 1).get();
	}

	extern "C" LIBEXPORT DeviceManager *getDeviceManager() {
		return new StandartDeviceManager();
	}
}  // namespace CalX
