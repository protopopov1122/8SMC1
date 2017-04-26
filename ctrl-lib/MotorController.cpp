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


#include "MotorController.h"
#include <algorithm>
#include <iostream>

namespace CalX {

	MotorController::MotorController(Motor *dev, ConfigManager *config) {
		this->dev = dev;
		this->config = config;
		this->dest = MoveType::Stop;
		INIT_LOG("MotorController");
	}

	MotorController::~MotorController() {
		for (const auto& l : this->listeners) {
			delete l;
		}
		DESTROY_LOG("MotorController");
	}

	Motor *MotorController::getMotor() {
		return this->dev;
	}
	
	device_id_t MotorController::getID() {
		return this->dev->getID();
	}
	
	Power MotorController::getPowerState() {
		return this->dev->getPowerState();
	}
	
	ErrorCode MotorController::enablePower(bool p) {
		bool res = this->dev->enablePower(p);
		return res ? ErrorCode::NoError : ErrorCode::LowLevelError;
	}
	
	ErrorCode MotorController::flipPower() {
		return enablePower(getPowerState() == Power::NoPower);
	}

	ErrorCode MotorController::checkTrailers() {
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

	ErrorCode MotorController::waitWhileRunning() {
		while (this->dev->isRunning()) {
			ErrorCode code = this->checkTrailers();
			if (code != ErrorCode::NoError) {
				return code;
			}
		}
		return ErrorCode::NoError;
	}

	ErrorCode MotorController::moveToTrailer(int tr) {
		if (this->dev->isRunning()) {
			return ErrorCode::MotorRunning;
		}
		if (this->dev->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}
		if (tr != 1 && tr != 2) {
			return ErrorCode::WrongParameter;
		}
		this->work = true;
        int_conf_t roll_step = config->getEntry("core")->getInt("roll_step", ROLL_STEP);
        int_conf_t roll_speed = config->getEntry("core")->getInt("roll_speed", ROLL_SPEED);
        unsigned char roll_div = (unsigned char) config->getEntry("core")->getInt("roll_div", ROLL_DIV);
        int_conf_t comeback = config->getEntry("core")->getInt("trailer_comeback", TRAILER_COMEBACK);
		
        int_conf_t dest = (tr == 1 ? -roll_step : roll_step);
		this->dest = (tr == 1 ? MoveType::RollDown : MoveType::RollUp);
		MotorRollEvent evt = {tr};
		use();
		this->sendRollingEvent(evt);
		while (!dev->isTrailerPressed(tr) && work) {
			if (!this->dev->isRunning()) {
				if (!this->dev->start(this->dev->getPosition() + dest, roll_speed, roll_div)) {
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
			errcode = this->startMove(this->dev->getPosition() + comeback, roll_speed, roll_div);
		}
		this->sendRolledEvent(evt);
		this->unuse();
		this->work = false;
		
		return errcode;
	}

	ErrorCode MotorController::startMove(motor_coord_t dest,
            float speed, int idiv, bool syncIn) {
        unsigned char div = (unsigned char) idiv;
		if (this->dev->isRunning()) {
			return ErrorCode::MotorRunning;
		}
		if (this->dev->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
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
        if (errcode != ErrorCode::NoError) {
            errcode = this->waitWhileRunning();
        }
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

	ErrorCode MotorController::startRelativeMove(motor_coord_t reldest,
			float speed, int div, bool syncIn) {
		motor_coord_t dest = getPosition() + reldest;
		return startMove(dest, speed, div, syncIn);
	}

	void MotorController::stop() {
		this->dest = MoveType::Stop;
		this->dev->stop();
		this->work = false;
	}

	motor_coord_t MotorController::getPosition() {
		return this->dev->getPosition();
	}
	
	void MotorController::addEventListener(MotorEventListener *l) {
		this->listeners.push_back(l);
	}
	
	void MotorController::removeEventListener(MotorEventListener *l) {
		this->listeners.erase(
			std::remove(this->listeners.begin(), this->listeners.end(),
				l), this->listeners.end());
		delete l;
	}
	
	void MotorController::sendMovingEvent(MotorMoveEvent &evt) {
		for (const auto& l : this->listeners) {
			l->moving(evt);
		}
	}
	
	void MotorController::sendMovedEvent(MotorMoveEvent &evt) {
		for (const auto& l : this->listeners) {
			l->moved(evt);
		}
	}
	
	void MotorController::sendStoppedEvent(MotorErrorEvent &evt) {
		for (const auto& l : this->listeners) {
			l->stopped(evt);
		}
	}
	
	void MotorController::sendRollingEvent(MotorRollEvent &evt) {
		for (const auto& l : this->listeners) {
			l->rolling(evt);
		}
	}
	
	void MotorController::sendRolledEvent(MotorRollEvent &evt) {
		for (const auto& l : this->listeners) {
			l->rolled(evt);
		}
	}
	
	void MotorController::use() {
		for (const auto& l : this->listeners) {
			l->use();
		}
	}
	
	void MotorController::unuse() {
		for (const auto& l : this->listeners) {
			l->unuse();
		}
	}
}
