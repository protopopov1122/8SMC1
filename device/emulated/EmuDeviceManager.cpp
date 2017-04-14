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


#include "EmuDeviceManager.h"


namespace CalX {
	
	
	EmuDeviceManager::EmuDeviceManager() {
		this->devConType.push_back(DeviceConnectionType::SerialPort);
		this->instrConType.push_back(DeviceConnectionType::SerialPort);
	}

	EmuDeviceManager::~EmuDeviceManager() {
		for (size_t i = 0; i < this->dev.size(); i++) {
			delete this->dev.at(i);
		}
		for (size_t i = 0; i < this->instr.size(); i++) {
			delete this->instr.at(i);
		}
	}
	
	void EmuDeviceManager::refresh() {
		
	}
	
	void EmuDeviceManager::saveError() {
		
	}
	
	Motor *EmuDeviceManager::connectMotor(DeviceConnectionPrms *prms) {
		EmuMotor *motor = new EmuMotor(this->dev.size(), this);
		this->dev.push_back(motor);
		return motor;
	}
	
	Instrument *EmuDeviceManager::connectInstrument(DeviceConnectionPrms *prms) {
		EmuInstrument *instr = new EmuInstrument(this->instr.size(), this);
		this->instr.push_back(instr);
		return instr;
	}
	
	extern "C" LIBEXPORT DeviceManager *getDeviceManager() {
		return new EmuDeviceManager();
	}
}
