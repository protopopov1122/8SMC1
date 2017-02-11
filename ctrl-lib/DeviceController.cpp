#include "DeviceController.h"
#include <iostream>

namespace Controller {

	// This thread stub may be used later
	// Each device controller has attached thread,
	// which monitors device state.
	void *device_control_thread(void *ptr) {
		DeviceController *dev = (DeviceController*) ptr;
//		while (dev->exists) {
//			if (dev->dest == MoveType::Stop) {
//				continue;
//			}
//			if ((dev->dest == MoveType::MoveUp ||
//					dev->dest == MoveType::MoveDown) &&
//					dev->dev->isRunning()) {
//				dev->checkTrailers();
//			}
//			sched_yield();
//		}
		dev->wait_for_thread = false;
		pthread_exit(nullptr);
		return nullptr;
	}

	DeviceController::DeviceController(Device *dev) {
		this->dev = dev;
		this->dest = MoveType::Stop;
		this->exists = true;
		this->wait_for_thread = true;
		this->length = 0;
		pthread_create(&this->dev_thread, nullptr, device_control_thread, this);
	}

	DeviceController::~DeviceController() {
		this->exists = false;
		while (this->wait_for_thread) {
			sched_yield();
		}
	}

	Device *DeviceController::getDevice() {
		return this->dev;
	}

	ErrorCode DeviceController::checkTrailers() {
		if (!this->dev->isRunning()) {
			return ErrorCode::DeviceStopped;
		}
		if (this->dest == MoveType::MoveUp &&
				this->dev->isTrailerPressed(2)) {
			this->dev->stop();
			this->dest = MoveType::Stop;
			return ErrorCode::Trailer2Pressed;
		}
		if (this->dest == MoveType::MoveDown &&
				this->dev->isTrailerPressed(1)) {
			this->dev->stop();
			this->dest = MoveType::Stop;
			return ErrorCode::Trailer1Pressed;
		}
		return ErrorCode::NoError;
	}

	ErrorCode DeviceController::waitWhileRunning() {
		while (this->dev->isRunning()) {
			ErrorCode code = this->checkTrailers();
			if (code != ErrorCode::NoError) {
				return code;
			}
		}
		return ErrorCode::NoError;
	}

	ErrorCode DeviceController::moveToTrailer(int tr, int comeback) {
		if (this->dev->isRunning()) {
			return ErrorCode::DeviceRunning;
		}
		if (tr != 1 && tr != 2) {
			return ErrorCode::WrongParameter;
		}
		int dest = (tr == 1 ? -ROLL_STEP : ROLL_STEP);
		this->dest = (tr == 1 ? MoveType::RollDown : MoveType::RollUp);
		while (!dev->isTrailerPressed(tr)) {
			if (!this->dev->isRunning()) {
				this->dev->start(this->dev->getPosition() + dest, ROLL_SPEED, ROLL_DIV);
			}
		}
		if (this->dev->isRunning()) {
			this->dev->stop();
		}
		this->dest = MoveType::Stop;
		if (tr == 2) {
			comeback *= -1;
		}	
		this->startMove(this->dev->getPosition() + comeback, ROLL_SPEED, ROLL_DIV);
		
		return ErrorCode::NoError;
	}

	ErrorCode DeviceController::startMove(motor_coord_t dest,
			float speed, int div, bool syncIn) {
		if (this->dev->isRunning()) {
			return ErrorCode::DeviceRunning;
		}
		this->dest = dest > this->dev->getPosition() ? MoveType::MoveUp :
				MoveType::MoveDown;
		this->dev->start(dest, speed, div, syncIn);
		return this->waitWhileRunning();
	}

	void DeviceController::stop() {
		this->dest = MoveType::Stop;
		this->dev->stop();
	}

	unsigned int DeviceController::getLength() {
		if (this->length == 0) {
			this->calculate_length();
		}
		return this->length;
	}

	ErrorCode DeviceController::resetPosition() {
		this->dev->setCurrentPosition(0);
		return ErrorCode::NoError;
	}

	void DeviceController::calculate_length() {
		this->length = 300000;	// Just constant
		// TODO Write proper calibration
	}
}
