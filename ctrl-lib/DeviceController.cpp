#include "DeviceController.h"
#include <algorithm>
#include <iostream>

namespace _8SMC1 {

	DeviceController::DeviceController(Device *dev) {
		this->dev = dev;
		this->dest = MoveType::Stop;
	}

	DeviceController::~DeviceController() {
		for (const auto& l : this->listeners) {
			delete l;
		}
	}

	Device *DeviceController::getDevice() {
		return this->dev;
	}
	
	DWORD DeviceController::getID() {
		return this->dev->getID();
	}

	ErrorCode DeviceController::checkTrailers() {
		if (!this->dev->isRunning()) {
			return ErrorCode::NoError;
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
		MotorRollEvent evt = {tr};
		this->sendRollingEvent(evt);
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
		this->sendRolledEvent(evt);
		
		return ErrorCode::NoError;
	}

	ErrorCode DeviceController::startMove(motor_coord_t dest,
			float speed, int div, bool syncIn) {
		if (this->dev->isRunning()) {
			return ErrorCode::DeviceRunning;
		}
		this->dest = dest > this->dev->getPosition() ? MoveType::MoveUp :
				MoveType::MoveDown;
		MotorMoveEvent evt = {dest, speed, div};
		this->sendMovingEvent(evt);
		this->dev->start(dest, speed, div, syncIn);
		ErrorCode errcode =  this->waitWhileRunning();
		if (errcode != ErrorCode::NoError) {
			MotorErrorEvent sevt = {errcode};
			this->sendStoppedEvent(sevt);
		} else {
			this->sendMovedEvent(evt);
		}
		return errcode;
	}

	ErrorCode DeviceController::startRelativeMove(motor_coord_t reldest,
			float speed, int div, bool syncIn) {
		motor_coord_t dest = getPosition() + reldest;
		return startMove(dest, speed, div, syncIn);
	}

	void DeviceController::stop() {
		this->dest = MoveType::Stop;
		this->dev->stop();
		MotorErrorEvent sevt = {ErrorCode::NoError};
		this->sendStoppedEvent(sevt);
	}

	ErrorCode DeviceController::resetPosition() {
		this->dev->setCurrentPosition(0);
		return ErrorCode::NoError;
	}

	motor_coord_t DeviceController::getPosition() {
		return this->dev->getPosition();
	}
	
	void DeviceController::addEventListener(MotorEventListener *l) {
		this->listeners.push_back(l);
	}
	
	void DeviceController::removeEventListener(MotorEventListener *l) {
		this->listeners.erase(
			std::remove(this->listeners.begin(), this->listeners.end(),
				l), this->listeners.end());
		delete l;
	}
	
	void DeviceController::sendMovingEvent(MotorMoveEvent &evt) {
		for (const auto& l : this->listeners) {
			l->moving(evt);
		}
	}
	
	void DeviceController::sendMovedEvent(MotorMoveEvent &evt) {
		for (const auto& l : this->listeners) {
			l->moved(evt);
		}
	}
	
	void DeviceController::sendStoppedEvent(MotorErrorEvent &evt) {
		for (const auto& l : this->listeners) {
			l->stopped(evt);
		}
	}
	
	void DeviceController::sendRollingEvent(MotorRollEvent &evt) {
		for (const auto& l : this->listeners) {
			l->rolling(evt);
		}
	}
	
	void DeviceController::sendRolledEvent(MotorRollEvent &evt) {
		for (const auto& l : this->listeners) {
			l->rolled(evt);
		}
	}
	
	void DeviceController::sendUse() {
		for (const auto& l : this->listeners) {
			l->use();
		}
	}
	
	void DeviceController::sendUnuse() {
		for (const auto& l : this->listeners) {
			l->unuse();
		}
	}
}
