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
	}

	DeviceController::DeviceController(Device *dev) {
		this->dev = dev;
		this->dest = MoveType::Stop;
		this->exists = true;
		this->wait_for_thread = true;
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

	err_code_t DeviceController::checkTrailers() {
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

	err_code_t DeviceController::waitWhileRunning() {
		while (this->dev->isRunning()) {
			err_code_t code = this->checkTrailers();
			if (code != ErrorCode::NoError) {
				return code;
			}
		}
		return ErrorCode::NoError;
	}

	err_code_t DeviceController::moveToTrailer(int tr, int comeback) {
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

	err_code_t DeviceController::startMove(motor_coord_t dest,
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
}
