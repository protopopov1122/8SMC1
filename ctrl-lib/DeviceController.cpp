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


#include "DeviceController.h"
#include <algorithm>
#include <iostream>

namespace CalX {

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
	
	device_id_t DeviceController::getID() {
		return this->dev->getID();
	}

	ErrorCode DeviceController::checkTrailers() {
		ErrorCode errcode = ErrorCode::NoError;
		if (!this->dev->isRunning()) {
			return errcode;
		}
		if (this->dest == MoveType::MoveUp &&
				this->dev->isTrailerPressed(2)) {
			errcode = ErrorCode::Trailer2Pressed;
			if (!this->dev->stop()) {
				errcode = ErrorCode::LowLevelError;
			}
			this->dest = MoveType::Stop;
			return errcode;
		}
		if (this->dest == MoveType::MoveDown &&
				this->dev->isTrailerPressed(1)) {
			errcode = ErrorCode::Trailer1Pressed;
			if (!this->dev->stop()) {
				errcode = ErrorCode::LowLevelError;
			}
			this->dest = MoveType::Stop;
			return errcode;
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
		this->work = true;
		int dest = (tr == 1 ? -ROLL_STEP : ROLL_STEP);
		this->dest = (tr == 1 ? MoveType::RollDown : MoveType::RollUp);
		MotorRollEvent evt = {tr};
		use();
		this->sendRollingEvent(evt);
		while (!dev->isTrailerPressed(tr) && work) {
			if (!this->dev->isRunning()) {
				if (!this->dev->start(this->dev->getPosition() + dest, ROLL_SPEED, ROLL_DIV)) {
					this->sendRolledEvent(evt);
					this->unuse();
					this->dest = MoveType::Stop;
					this->work = false;
					return ErrorCode::LowLevelError;
				}
			}
		}
		if (this->dev->isRunning()) {
			if (!this->dev->stop()) {
				this->sendRolledEvent(evt);
				this->unuse();
				this->dest = MoveType::Stop;
				this->work = false;
				return ErrorCode::LowLevelError;
			}
		}
		this->dest = MoveType::Stop;
		if (tr == 2) {
			comeback *= -1;
		}
		ErrorCode errcode = ErrorCode::NoError;
		if (work) {
			errcode = this->startMove(this->dev->getPosition() + comeback, ROLL_SPEED, ROLL_DIV);
		}
		this->sendRolledEvent(evt);
		this->unuse();
		this->work = false;
		
		return errcode;
	}

	ErrorCode DeviceController::startMove(motor_coord_t dest,
			float speed, int div, bool syncIn) {
		if (this->dev->isRunning()) {
			return ErrorCode::DeviceRunning;
		}
		this->work = true;
		this->dest = dest > this->dev->getPosition() ? MoveType::MoveUp :
				MoveType::MoveDown;
		MotorMoveEvent evt = {dest, speed, div};
		this->use();
		this->sendMovingEvent(evt);
		ErrorCode errcode = ErrorCode::NoError;
		if (!this->dev->start(dest, speed, div, syncIn)) {
			errcode = ErrorCode::LowLevelError;
		}
		errcode = this->waitWhileRunning();
		if (errcode != ErrorCode::NoError) {
			MotorErrorEvent sevt = {errcode};
			this->sendStoppedEvent(sevt);
		} else {
			this->sendMovedEvent(evt);
		}
		this->unuse();
		this->work = false;
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
		this->work = false;
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
	
	void DeviceController::use() {
		for (const auto& l : this->listeners) {
			l->use();
		}
	}
	
	void DeviceController::unuse() {
		for (const auto& l : this->listeners) {
			l->unuse();
		}
	}
}
