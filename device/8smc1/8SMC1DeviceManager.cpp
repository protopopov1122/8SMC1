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


#include "8SMC1DeviceManager.h"
#include <stdio.h>
#include <string.h>
#include <iostream>


namespace CalX {
	
	_8SMC1DeviceManager::_8SMC1DeviceManager()
		: DeviceManager::DeviceManager() {
		this->refresh();
		for (size_t i = 0; i < devs.NOD; i++) {
			this->dev.push_back(new _8SMC1Device((device_id_t) i, this));
		}
		
		this->instrConType.push_back(DeviceConnectionType::SerialPort);
	}

	_8SMC1DeviceManager::~_8SMC1DeviceManager() {
		for (size_t i = 0; i < this->dev.size(); i++) {
			delete this->dev.at(i);
		}
		for (size_t i = 0; i < this->instr.size(); i++) {
			delete this->instr.at(i);
		}
		this->dev.clear();
		if (USMC_Close()) {
			saveError();
		}
	}

	void _8SMC1DeviceManager::refresh() {
		if (USMC_Init(this->devs)) {
			saveError();
		}
	}

	void _8SMC1DeviceManager::saveError() {
		char er[101];
		do {
			USMC_GetLastErr(er,100);
			er[100] = '\0';
			if (strlen(er) > 0) {
				this->error_queue.push_back(std::string(er));
			}
		} while (strlen(er) > 0);
		
		for (size_t i = 0; i < this->instr.size(); i++) {
			Instrument *in = this->instr.at(i);
			while (in->hasErrors()) {
				this->error_queue.push_back(in->pollError());
			}
		}
	}

	std::string _8SMC1DeviceManager::getDeviceSerial(device_id_t id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Serial[id]);
	}

	std::string _8SMC1DeviceManager::getDeviceVersion(device_id_t id) {
		if (id >= this->devs.NOD) {
			return "";
		}
		return std::string(this->devs.Version[id]);
	}
	
	Device *_8SMC1DeviceManager::connectDevice(DeviceConnectionPrms *prms) {
		return nullptr;
	}
	
	Instrument *_8SMC1DeviceManager::connectInstrument(DeviceConnectionPrms *_prms) {
		if (_prms->type != DeviceConnectionType::SerialPort) {
			return nullptr;
		}
		DeviceSerialPortConnectionPrms *prms = (DeviceSerialPortConnectionPrms*) _prms;
		_8SMC1Instrument *instr = new _8SMC1Instrument((device_id_t) this->instr.size(), prms, this);
		if (instr->hasErrors()) {
			this->saveError();
			delete instr;
			return nullptr;
		}
		this->instr.push_back(instr);
		return instr;
	}
	
	extern "C" LIBEXPORT DeviceManager *getDeviceManager() {
		return new _8SMC1DeviceManager();
	}
}
