#include "DeviceManager.h"

namespace Controller {
	
	MotorTask::MotorTask(int dest, Device *dev) {
		this->dest = dest;
		this->dev = dev;
		this->speed = dev->getSpeed();
		this->step_divisor = dev->getStepDivisor();
		this->slow_start = dev->hasSlowStart();
		this->wait_sync = dev->waitsForSync();
	}

	MotorTask::~MotorTask() {

	}

	MotorTask *MotorTask::setSpeed(float speed) {
		this->speed = speed;
		return this;
	}

	MotorTask *MotorTask::setStepDivisor(int sdiv) {
		this->step_divisor = sdiv;
		return this;
	}

	MotorTask *MotorTask::setSlowStart(bool s) {
		this->slow_start = s;
		return this;
	}

	MotorTask *MotorTask::setWaitSync(bool ws) {
		this->wait_sync = ws;
		return this;
	}

	bool MotorTask::start() {
		USMC_StartParameters startPrms;
		if (USMC_GetStartParameters(dev->getID(), startPrms)) {
			dev->getDeviceManager()->printError();
			return false;
		}
		startPrms.SDivisor = this->step_divisor;
		startPrms.SlStart = this->slow_start;
		if (USMC_Start(dev->getID(), dest, speed, startPrms)) {
			dev->getDeviceManager()->printError();
			return false;
		}
		dev->getDeviceManager()->setLastDevice(this->dev);
		return true;
	}
}
