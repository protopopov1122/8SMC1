#include "ctrl-lib/ctrl/DeviceControllerSet.h"

namespace CalX {
	MotorControllerSet::MotorControllerSet(ConfigManager &config, DeviceManager &devman, DeviceControllerSetListener *listener)
		: config(config), devman(devman), listener(listener) {
		for (device_id_t deviceId = 0;
		     deviceId < static_cast<device_id_t>(this->devman.getMotorCount());
			 deviceId++) {
			this->motors.push_back(std::make_shared<MotorController>(
				this->config, *this->devman.getMotor(deviceId)));
		}
	}
	
	std::weak_ptr<MotorController> MotorControllerSet::connectDevice(
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
	
	std::weak_ptr<MotorController> MotorControllerSet::getDeviceController(device_id_t deviceId) const {
		if (deviceId < 0 || static_cast<std::size_t>(deviceId) >= this->motors.size()) {
			return std::weak_ptr<MotorController>();
		} else {
			return this->motors.at(deviceId);
		}
	}
	
	std::size_t MotorControllerSet::getDeviceCount() const {
		return this->motors.size();
	}
	
	InstrumentControllerSet::InstrumentControllerSet(ConfigManager &config, DeviceManager &devman, DeviceControllerSetListener *listener)
		: config(config), devman(devman), listener(listener) {
		for (device_id_t deviceId = 0;
		     deviceId < static_cast<device_id_t>(this->devman.getInstrumentCount());
			 deviceId++) {
			this->instruments.push_back(std::make_shared<InstrumentController>(
			    this->config, *this->devman.getInstrument(deviceId)));
		}	
	}
		
	std::weak_ptr<InstrumentController> InstrumentControllerSet::connectDevice(
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
	
	std::weak_ptr<InstrumentController> InstrumentControllerSet::getDeviceController(device_id_t deviceId) const {
		if (deviceId < 0 || static_cast<std::size_t>(deviceId) >= this->instruments.size()) {
			return std::weak_ptr<InstrumentController>();
		} else {
			return this->instruments.at(deviceId);
		}
	}
	
	std::size_t InstrumentControllerSet::getDeviceCount() const {
		return this->instruments.size();
	}
}