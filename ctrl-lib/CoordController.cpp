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


#include "plane/CoordPlane.h"
#include "misc/CircleGenerator.h"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CalX {

	const char *COORD_CTRL_TAG = "CoordCtrl";

	CoordController::CoordController(MotorController *xaxis,
			MotorController *yaxis, ConfigManager *config, InstrumentController *instr) {
		this->xAxis = xaxis;
		this->yAxis = yaxis;
		this->instr = instr;
		this->config = config;
		this->size = {0, 0, 0, 0};
		this->defWork = true;
		this->measured = false;
		LOG(COORD_CTRL_TAG, "New coordinate controller. X Axis: #" + std::to_string(this->xAxis->getID()) + "; Y Axis: #" + std::to_string(this->yAxis->getID()) + "; instrument: " + std::string(instr != nullptr ? std::to_string(instr->getID()) : "no"));
		INIT_LOG("CoordController");
	}

	CoordController::~CoordController() {
		for (const auto& l : this->listeners) {
			delete l;
		}
		DESTROY_LOG("CoordController");
	}

	void CoordController::dump(std::ostream &os) {
		os << "coord(" << this->xAxis->getID() << "; " << this->yAxis->getID() << ")";
	}

	MotorController *CoordController::getXAxis() {
		return this->xAxis;
	}

	MotorController *CoordController::getYAxis() {
		return this->yAxis;
	}
	
	InstrumentController *CoordController::getInstrument() {
		return this->instr;
	}

	ErrorCode CoordController::move(motor_point_t point, float speed, int div,
			bool sync) {

		// TODO Enable proper motor syncing
		Motor *xDev = this->xAxis->getMotor();
		Motor *yDev = this->yAxis->getMotor();
		motor_coord_t dx = point.x - xDev->getPosition();
		motor_coord_t dy = point.y - yDev->getPosition();
		if (this->xAxis->getPowerState() == Power::NoPower ||
			this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}

		// Calculate x and y axis speed v=sqrt(vx^2 + vy^2); vy = n*vx

		float x_speed;
		float y_speed;
		if (!sync || dx == dy) {
			x_speed = speed;
			y_speed = speed;
		} else if (dx == 0) {
			x_speed = 0;
			y_speed = speed;
		} else if (dy == 0) {
			x_speed = speed;
			y_speed = 0;
		} else {
			float ddx = (float) dx;
			float ddy = (float) dy;
			float ncoef = fabs(ddy / ddx);
			x_speed = sqrt(speed * speed / (1 + ncoef * ncoef));
			y_speed = ncoef * x_speed;
		}
		work = defWork;
		if (!work) {
			return ErrorCode::NoError;
		}
		
		if (xAxis->dev->isRunning() || yAxis->dev->isRunning()) {
			return ErrorCode::MotorRunning;
		}
		xAxis->dest = point.x > xAxis->dev->getPosition() ? MoveType::MoveUp :
				MoveType::MoveDown;
		xAxis->use();
		yAxis->use();
		
		
		if (this->instr != nullptr && sync) {
			this->instr->use();
			ErrorCode errcode = this->instr->enable(true);
			if (errcode != ErrorCode::NoError) {
				xAxis->unuse();
				yAxis->unuse();
				instr->unuse();
				return errcode;
			}
		}
		MotorMoveEvent xmevt = {point.x, x_speed, div};
		if (!xAxis->dev->start(point.x, x_speed, div, false)) {
			xAxis->unuse();
			yAxis->unuse();
			xAxis->dest = MoveType::Stop;
			return ErrorCode::LowLevelError;
		}
		xAxis->sendMovingEvent(xmevt);
		
		MotorMoveEvent ymevt = {point.y, y_speed, div};
		yAxis->dest = point.y > yAxis->dev->getPosition() ? MoveType::MoveUp :
				MoveType::MoveDown;
		if (!yAxis->dev->start(point.y, y_speed, div, false)) {
			xAxis->unuse();
			yAxis->unuse();
			yAxis->dest = MoveType::Stop;
			xAxis->stop();
			xAxis->dest = MoveType::Stop;
			xAxis->sendMovedEvent(xmevt);
			return ErrorCode::LowLevelError;
		}
		yAxis->sendMovingEvent(ymevt);
		
		CoordMoveEvent evt = {point, speed, div, sync};
		sendMovingEvent(evt);
		use();
		while (xDev->isRunning() || yDev->isRunning()) {
			if (xDev->isRunning()) {
				ErrorCode code = xAxis->checkTrailers();
				if (code != ErrorCode::NoError) {
					xAxis->stop();
					yAxis->stop();
		
					if (this->instr != nullptr && sync) {
						this->instr->enable(false);
						this->instr->unuse();
					}
					
					MotorErrorEvent merrevt = {code};
					xAxis->sendStoppedEvent(merrevt);
					yAxis->sendStoppedEvent(merrevt);
					CoordErrorEvent eevt = {code};
					sendStoppedEvent(eevt);
					xAxis->unuse();
					yAxis->unuse();
					unuse();
					return code;
				}
			}
			if (yDev->isRunning()) {
				ErrorCode code = yAxis->checkTrailers();
				if (code != ErrorCode::NoError) {
					xAxis->stop();
					yAxis->stop();
		
					if (this->instr != nullptr && sync) {
						this->instr->enable(false);
						this->instr->unuse();
					}
					
					MotorErrorEvent merrevt = {code};
					xAxis->sendStoppedEvent(merrevt);
					yAxis->sendStoppedEvent(merrevt);
					CoordErrorEvent eevt = {code};
					sendStoppedEvent(eevt);
					xAxis->unuse();
					yAxis->unuse();
					unuse();
					return code;
				}
			}
		}
		
		ErrorCode errcode = ErrorCode::NoError;
		if (this->instr != nullptr && sync) {
			errcode = this->instr->enable(false);
			this->instr->unuse();
		}
		xAxis->sendMovedEvent(xmevt);
		yAxis->sendMovedEvent(ymevt);
		sendMovedEvent(evt);
		xAxis->unuse();
		yAxis->unuse();
		unuse();
		return errcode;
	}

	ErrorCode CoordPlane::relativeMove(motor_point_t relpoint, float speed, int div,
			bool sync) {
		motor_point_t point = getPosition();
		point.x += relpoint.x;
		point.y += relpoint.y;
		return move(point, speed, div, sync);
	}

	ErrorCode CoordController::calibrate(TrailerId tr) {
		if (this->xAxis->getMotor()->isRunning()
				|| this->yAxis->getMotor()->isRunning()) {
			return ErrorCode::MotorRunning;
		}
		if (this->xAxis->getPowerState() == Power::NoPower ||
			this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}
		if (tr != TrailerId::Trailer1 && tr != TrailerId::Trailer2) {
			return ErrorCode::WrongParameter;
		}
		work = defWork;
		if (!work) {
			return ErrorCode::NoError;
		}
		int roll_step = config->getEntry("core")->getInt("roll_step", ROLL_STEP);
		int roll_speed = config->getEntry("core")->getInt("roll_speed", ROLL_SPEED);
		int roll_div = config->getEntry("core")->getInt("roll_div", ROLL_DIV);
		int comeback = config->getEntry("core")->getInt("trailer_comeback", TRAILER_COMEBACK);
		int dest = (tr == TrailerId::Trailer1 ? -roll_step : roll_step);
		xAxis->dest = (tr == TrailerId::Trailer1 ? MoveType::RollDown : MoveType::RollUp);
		yAxis->dest = (tr == TrailerId::Trailer1 ? MoveType::RollDown : MoveType::RollUp);
		bool xpr = false;
		bool ypr = false;
		MotorRollEvent mevt = {static_cast<int>(tr)};
		if (this->instr != nullptr) {
			this->instr->use();
		}
		xAxis->use();
		yAxis->use();
		xAxis->sendRollingEvent(mevt);
		yAxis->sendRollingEvent(mevt);
		CoordCalibrateEvent evt = {tr};
		sendCalibratingEvent(evt);
		use();
		while (!(xpr && ypr) && work) {
			if (!xAxis->getMotor()->isTrailerPressed(static_cast<int>(tr))) {
				if (!xpr && !xAxis->getMotor()->isRunning()) {
					if (!xAxis->getMotor()->start(
							xAxis->getMotor()->getPosition() + dest,
							roll_speed, roll_div)) {
						xAxis->stop();
						yAxis->stop();
						xAxis->sendRolledEvent(mevt);
						yAxis->sendRolledEvent(mevt);
						if (this->instr != nullptr) {
							this->instr->unuse();
						}
						xAxis->unuse();
						yAxis->unuse();
						sendCalibratedEvent(evt);
						unuse();
						return ErrorCode::LowLevelError;
					}
				}
			} else  {
				xpr = true;
				if (xAxis->getMotor()->isRunning()) {
					xAxis->getMotor()->stop();
				}
			}
				

			if (!yAxis->getMotor()->isTrailerPressed(static_cast<int>(tr))) {
				if (!ypr && !yAxis->getMotor()->isRunning()) {
					if (!yAxis->getMotor()->start(
							yAxis->getMotor()->getPosition() + dest,
							roll_speed, roll_div)){
						xAxis->stop();
						yAxis->stop();
						xAxis->sendRolledEvent(mevt);
						yAxis->sendRolledEvent(mevt);
						if (this->instr != nullptr) {
							this->instr->unuse();
						}
						xAxis->unuse();
						yAxis->unuse();
						sendCalibratedEvent(evt);
						unuse();
						return ErrorCode::LowLevelError;
					}
				}
			} else {
				ypr = true;
				if (yAxis->getMotor()->isRunning()) {
					yAxis->getMotor()->stop();
				}
			}
		}
		if (xAxis->getMotor()->isRunning()) {
			xAxis->getMotor()->stop();
		}
		if (yAxis->getMotor()->isRunning()) {
			yAxis->getMotor()->stop();
		}

		xAxis->dest = MoveType::Stop;
		yAxis->dest = MoveType::Stop;
		if (tr == TrailerId::Trailer2) {
			comeback *= -1;
		}
		if (work) {
			xAxis->startMove(xAxis->getMotor()->getPosition() + comeback, roll_speed, roll_div);
			yAxis->startMove(yAxis->getMotor()->getPosition() + comeback, roll_speed, roll_div);
		}
		xAxis->sendRolledEvent(mevt);
		yAxis->sendRolledEvent(mevt);
		if (this->instr != nullptr) {
			this->instr->unuse();
		}
		xAxis->unuse();
		yAxis->unuse();
		sendCalibratedEvent(evt);
		unuse();
		return ErrorCode::NoError;
	}

	motor_point_t CoordController::getPosition() {
		motor_point_t pos;
		pos.x = xAxis->getPosition();
		pos.y = yAxis->getPosition();
		return pos;
	}

	ErrorCode CoordController::arc(motor_point_t dest, motor_point_t center, int spl,
				float speed, int div, bool clockwise, bool strict) {
		if (this->xAxis->getPowerState() == Power::NoPower ||
			this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}
		motor_point_t src = this->getPosition();
		double r1 = pow(src.x - center.x, 2) +
			     pow(src.y - center.y, 2);
		double r2 = pow(dest.x - center.x, 2) +
			     pow(dest.y - center.y, 2);
		if (fabs(sqrt(r1) - sqrt(r2)) >= COMPARISON_RADIUS) {
				return ErrorCode::ArcError;
		}
		double fullCircle = 2 * M_PI * sqrt(r1);
		int64_t splitter = (int64_t) ceil(fullCircle / spl);
		if (splitter == 0) {
			splitter = 1;
		}
		Circle cir(center, (int64_t) sqrt(r1), clockwise);
		if (!cir.skip(src)) {
			return ErrorCode::ArcError;
		}
		motor_point_t pnt;
		size_t count = 0;
		work = defWork;
		if (!work) {
			return ErrorCode::NoError;
		}
		if (this->instr != nullptr) {
			this->instr->use();
		}
		xAxis->use();
		yAxis->use();
		use();
		do {
			if (!work) {
				break;
			}
			if (clockwise) {
				pnt = cir.getPrevElement();
			} else {
				pnt = cir.getNextElement();
			}
			if (count++ % splitter == 0) {
				ErrorCode err = this->move(pnt, speed, div, true);
				if (err != ErrorCode::NoError) {
					if (this->instr != nullptr) {
						this->instr->unuse();
					}
					xAxis->unuse();
					yAxis->unuse();
					unuse();
					return err;
				}
				pnt = getPosition();
			}
		} while (abs(dest.x - pnt.x) > COMPARISON_RADIUS ||
			abs(dest.y - pnt.y) > COMPARISON_RADIUS);
		if (this->instr != nullptr) {
			this->instr->unuse();
		}
		xAxis->unuse();
		yAxis->unuse();
		unuse();
		ErrorCode code = ErrorCode::NoError;
		if (work) {
			code = this->move(dest, speed, div, true);
		}
		return code;
	}

	ErrorCode CoordPlane::relativeArc(motor_point_t reldest, motor_point_t relcenter, int splitter,
				float speed, int div, bool clockwise, bool strict) {
		motor_point_t dest = getPosition();
		motor_point_t center = getPosition();
		dest.x += reldest.x;
		dest.y += reldest.y;
		center.x += relcenter.x;
		center.y += relcenter.y;
		return arc(dest, center, splitter, speed, div, clockwise, strict);
	}
	
	motor_rect_t CoordController::getSize() {
		return this->size;
	}

	bool CoordController::isMeasured() {
		return this->measured;
	}
	
	ErrorCode CoordController::measure(TrailerId tid) {
		if (this->xAxis->getPowerState() == Power::NoPower ||
			this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}
		work = defWork;
		TrailerId tid1 = (tid == TrailerId::Trailer1 ? TrailerId::Trailer2 : TrailerId::Trailer1);
		TrailerId tid2 = tid;
		if (!work) {
			return ErrorCode::NoError;
		}
		ErrorCode errcode = this->calibrate(tid1);
		motor_point_t max = getPosition();
		if (!work || errcode != ErrorCode::NoError) {
			return errcode;
		}
		errcode = this->calibrate(tid2);
		motor_point_t min = getPosition();
		this->size = {min.x < max.x ? min.x : max.x, min.y < max.y ? min.y : max.y, abs(max.x - min.x), abs(max.y - min.y)};
		this->measured = true;
		return errcode;
	}
	
	void CoordController::addEventListener(CoordEventListener *l) {
		this->listeners.push_back(l);
	}
	
	void CoordController::removeEventListener(CoordEventListener *l) {
		this->listeners.erase(
			std::remove(this->listeners.begin(), this->listeners.end(),
				l), this->listeners.end());
		delete l;	
	}
	
	void CoordController::sendMovingEvent(CoordMoveEvent &evt) {
		for (const auto& l : this->listeners) {
			l->moving(evt);
		}
	}
	
	void CoordController::sendMovedEvent(CoordMoveEvent &evt) {
		for (const auto& l : this->listeners) {
			l->moved(evt);
		}
	}
	
	void CoordController::sendStoppedEvent(CoordErrorEvent &evt) {
		for (const auto& l : this->listeners) {
			l->stopped(evt);
		}
	}
	
	void CoordController::sendCalibratingEvent(CoordCalibrateEvent &evt) {
		for (const auto& l : this->listeners) {
			l->calibrating(evt);
		}
	}
	
	void CoordController::sendCalibratedEvent(CoordCalibrateEvent &evt) {
		for (const auto& l : this->listeners) {
			l->calibrated(evt);
		}
	}
	
	void CoordController::use() {
		for (const auto& l : this->listeners) {
			l->use();
		}
		xAxis->use();
		yAxis->use();
		if (this->instr != nullptr) {
			this->instr->use();
		}
	}
	
	void CoordController::unuse() {
		for (const auto& l : this->listeners) {
			l->unuse();
		}
		xAxis->unuse();
		yAxis->unuse();
		if (this->instr != nullptr) {
			this->instr->unuse();
		}
	}
	
	void CoordController::stop() {
		xAxis->stop();
		yAxis->stop();
		work = false;
	}
	
	void CoordController::kill() {
		defWork = false;
		stop();
	}
	
	CoordPlane *CoordController::clone(CoordPlane *base) {
		return new CoordController(this->xAxis, this->yAxis, this->config, this->instr);
	}	
}
