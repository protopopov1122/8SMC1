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

#include "ctrl-lib/ctrl/DeviceControllerSet.h"

namespace CalX {
	VectorMotorControllerSet::VectorMotorControllerSet(ConfigManager &config, DeviceManager &devman, DeviceControllerSetListener *listener)
		: config(config), devman(devman), listener(listener) {
		for (device_id_t deviceId = 0;
		     deviceId < static_cast<device_id_t>(this->devman.getMotorCount());
			 deviceId++) {
			this->motors.push_back(std::make_shared<MotorController>(
				this->config, *this->devman.getMotor(deviceId)));
		}
	}
	
	std::weak_ptr<MotorController> VectorMotorControllerSet::connectDevice(
	    DeviceConnectionPrms *prms) {
		Motor *motorDevice = this->devman.connectMotor(prms);
		if (motorDevice == nullptr) {
			return std::weak_ptr<MotorController>();
		}
		devman.refresh();
		std::shared_ptr<MotorController> motorController =
		    std::make_shared<MotorController>(this->config, *motorDevice);
		this->motors.push_back(motorController);
		if (this->listener) {
			this->listener->onDeviceConnected(motorController);
		}
		return motorController;
	}
	
	std::weak_ptr<MotorController> VectorMotorControllerSet::getDeviceController(device_id_t deviceId) const {
		if (deviceId < 0 || static_cast<std::size_t>(deviceId) >= this->motors.size()) {
			return std::weak_ptr<MotorController>();
		} else {
			return this->motors.at(deviceId);
		}
	}
	
	std::size_t VectorMotorControllerSet::getDeviceCount() const {
		return this->motors.size();
	}
	
	VectorInstrumentControllerSet::VectorInstrumentControllerSet(ConfigManager &config, DeviceManager &devman, DeviceControllerSetListener *listener)
		: config(config), devman(devman), listener(listener) {
		for (device_id_t deviceId = 0;
		     deviceId < static_cast<device_id_t>(this->devman.getInstrumentCount());
			 deviceId++) {
			this->instruments.push_back(std::make_shared<InstrumentController>(
			    this->config, *this->devman.getInstrument(deviceId)));
		}	
	}
		
	std::weak_ptr<InstrumentController> VectorInstrumentControllerSet::connectDevice(
		DeviceConnectionPrms *prms) {
		Instrument *instrumentDevice = this->devman.connectInstrument(prms);
		if (instrumentDevice == nullptr) {
			return std::weak_ptr<InstrumentController>();
		}
		std::shared_ptr<InstrumentController> instrumentController =
		    std::make_shared<InstrumentController>(this->config, *instrumentDevice);
		this->instruments.push_back(instrumentController);
		if (this->listener) {
			this->listener->onDeviceConnected(instrumentController);
		}
		return instrumentController;
	}
	
	std::weak_ptr<InstrumentController> VectorInstrumentControllerSet::getDeviceController(device_id_t deviceId) const {
		if (deviceId < 0 || static_cast<std::size_t>(deviceId) >= this->instruments.size()) {
			return std::weak_ptr<InstrumentController>();
		} else {
			return this->instruments.at(deviceId);
		}
	}
	
	std::size_t VectorInstrumentControllerSet::getDeviceCount() const {
		return this->instruments.size();
	}
}