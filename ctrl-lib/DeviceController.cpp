#include "DeviceController.h"

namespace Controller {

	DeviceController::DeviceController(Device *dev) {
		this->dev = dev;
	}

	DeviceController::~DeviceController() {
		
	}

	Device *DeviceController::getDevice() {
		return this->dev;
	}

	err_code_t DeviceController::waitWhileRunning() {
		while (this->dev->isRunning()) {
			if (this->dev->isTrailerPressed(1)) {
				this->dev->stop();
				return ErrorCode::Trailer1Pressed;
			}
			if (this->dev->isTrailerPressed(2)) {
				this->dev->stop();
				return ErrorCode::Trailer2Pressed;
			}
		}
		return ErrorCode::NoError;
	}

	err_code_t DeviceController::moveToTrailer(int tr) {
		if (tr != 1 && tr != 2) {
			return ErrorCode::WrongParameter;
		}
		int dest = (tr == 1 ? 1000 : -1000);
		const float SPEED = 1500.0f;
		const unsigned char DIV = 8;
		while (!dev->isTrailerPressed(tr)) {
			if (!this->dev->isRunning()) {
				this->dev->start(this->dev->getPosition() + dest, SPEED, DIV);
			}
		}
		if (this->dev->isRunning()) {
			this->dev->stop();
		}
		return ErrorCode::NoError;
	}

	err_code_t DeviceController::startMove(motor_coord_t dest, float speed, int div, bool blocking) {
		this->dev->start(dest, speed, div);
		if (blocking) {
			return this->waitWhileRunning();
		} else {
			return ErrorCode::NoError;
		}
	}
}
