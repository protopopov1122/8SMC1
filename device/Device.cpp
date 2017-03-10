#include "DeviceManager.h"

/* Implementations of API wrappers */

namespace _8SMC1 {

	Device::~Device() {
		
	}

	DeviceManager *Device::getDeviceManager() {
		return this->devman;
	}

	device_id_t Device::getID() {
		return this->dev;
	}

	std::string Device::getSerial() {
		return this->devman->getDeviceSerial(this->dev);
	}

	std::string Device::getVersion() {
		return this->devman->getDeviceVersion(this->dev);
	}

}
