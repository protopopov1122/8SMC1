/*
        Copyright (c) 2017 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ctrl-lib/misc/CircleGenerator.h"
#include "ctrl-lib/plane/CoordPlane.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CalX {

	const char *COORD_CTRL_TAG = "CoordCtrl";

	CoordController::CoordController(
	    std::shared_ptr<MotorController> xaxis,
	    std::shared_ptr<MotorController> yaxis,
	    std::shared_ptr<ConfigManager> config,
	    std::shared_ptr<InstrumentController> instr) {
		this->xAxis = xaxis;
		this->yAxis = yaxis;
		this->instr = instr;
		this->config = config;
		this->size = { 0, 0, 0, 0 };
		this->defWork = true;
		this->work = false;
		this->measured = false;
		this->session_opened = false;
		this->status = CoordPlaneStatus::Idle;
		LOG(COORD_CTRL_TAG,
		    "New coordinate controller. X Axis: #" +
		        std::to_string(this->xAxis->getID()) + "; Y Axis: #" +
		        std::to_string(this->yAxis->getID()) + "; instrument: " +
		        std::string(instr != nullptr ? std::to_string(instr->getID())
		                                     : "no"));
	}

	void CoordController::dump(std::ostream &os) {
		os << "coord(" << this->xAxis->getID() << "; " << this->yAxis->getID()
		   << ")";
	}

	std::shared_ptr<MotorController> CoordController::getXAxis() {
		return this->xAxis;
	}

	std::shared_ptr<MotorController> CoordController::getYAxis() {
		return this->yAxis;
	}

	std::shared_ptr<InstrumentController> CoordController::getInstrument() {
		return this->instr;
	}

	ErrorCode CoordController::move(motor_point_t point, float speed, bool sync) {
		/* Prepare motors and determine movement destination on each axis */
		Motor *xDev = this->xAxis->getMotor();
		Motor *yDev = this->yAxis->getMotor();
		motor_coord_t dx = point.x - xDev->getPosition();
		motor_coord_t dy = point.y - yDev->getPosition();
		/* Check if motors are enabled */
		if (this->xAxis->getPowerState() == Power::NoPower ||
		    this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}

		/* In jump(asynchronous) mode speed can be increased to the maximal value
		   Depending on configuration settings */
		if (!sync) {
			float maxspeed =
			    this->config->getEntry("core")->getReal("jump_speed", 0.0f);
			if (maxspeed > 0 && speed < maxspeed) {
				LOG("CoordController", "Changing jump speed from " +
				                           std::to_string(speed) + " to " +
				                           std::to_string(maxspeed) + " steps/sec");
				speed = maxspeed;
			}
		}

		/* Calculate x and y axis speed
		   To achieve the requested final speed v=sqrt(vx^2 + vy^2); vy = n*vx */
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
		/* Reset work flag, check if plan can operate */
		work = defWork;
		if (!work) {
			return ErrorCode::NoError;
		}

		/* Check if one of the motors is currently running
		   This situation should not be possible if planes are
			 configured properly */
		if (xAxis->getMotor()->isRunning() || yAxis->getMotor()->isRunning()) {
			return ErrorCode::MotorRunning;
		}

		/* Mark motors as active used */
		xAxis->use();
		yAxis->use();

		/* Determine if instrument is present, mark it as used and enable it (if necesarry).
		   Even if the instrument is enabled here, it will continue work after method finished
			 (for optimisation purposes) and will be disabled on session close */
		if (this->instr != nullptr) {
			this->instr->use();
			ErrorCode errcode = this->instr->enable(sync);
			/* Error while enabling instrument, unuse all, halt */
			if (errcode != ErrorCode::NoError) {
				xAxis->unuse();
				yAxis->unuse();
				instr->unuse();
				return errcode;
			}
		}

		/* Update current plane status */
		this->status = sync ? CoordPlaneStatus::Move : CoordPlaneStatus::Jump;
		/* Start x-axis motor, emit event about it */
		MotorMoveEvent xmevt = { point.x, x_speed };
		xAxis->dest = point.x > xAxis->getMotor()->getPosition()
		                  ? MoveType::MoveUp
		                  : MoveType::MoveDown;
		if (!xAxis->getMotor()->start(point.x, x_speed)) { /* Something went wrong. Unuse motors, set status to idle, halt */
			this->status = CoordPlaneStatus::Idle;
			xAxis->unuse();
			yAxis->unuse();
			xAxis->dest = MoveType::Stop;
			return ErrorCode::LowLevelError;
		}
		xAxis->sendMovingEvent(xmevt);

		/* Start y-axis motor, emit event about it */
		MotorMoveEvent ymevt = { point.y, y_speed };
		yAxis->dest = point.y > yAxis->getMotor()->getPosition()
		                  ? MoveType::MoveUp
		                  : MoveType::MoveDown;

		if (!yAxis->getMotor()->start(point.y, y_speed)) { /* Something went wrong. Stop x-axis, unuse motors, etc. */
			this->status = CoordPlaneStatus::Idle;
			xAxis->unuse();
			yAxis->unuse();
			yAxis->dest = MoveType::Stop;
			xAxis->stop();
			xAxis->dest = MoveType::Stop;
			xAxis->sendMovedEvent(xmevt);
			return ErrorCode::LowLevelError;
		}
		yAxis->sendMovingEvent(ymevt);

		/* Emit event about coordinate plane using, mark it as used */
		CoordMoveEvent evt = { point, speed, sync };
		sendMovingEvent(evt);
		use();
		/* Wait while one of motors (or both) is running, detect possible errors */
		while (xDev->isRunning() || yDev->isRunning()) {
			if (xDev->isRunning()) {
				ErrorCode code = xAxis->checkTrailers();
				if (code != ErrorCode::NoError) { /* Motor reached trailes, stop plane, unuse devices, produce error */
					this->status = CoordPlaneStatus::Idle;
					xAxis->stop();
					yAxis->stop();

					if (this->instr != nullptr && sync) {
						this->instr->unuse();
					}

          /* Emit necesarry errors */
					MotorErrorEvent merrevt = { code };
					xAxis->sendStoppedEvent(merrevt);
					yAxis->sendStoppedEvent(merrevt);
					CoordErrorEvent eevt = { code };
					sendStoppedEvent(eevt);
					xAxis->unuse();
					yAxis->unuse();
					unuse();
					return code;
				}
			}
			if (yDev->isRunning()) { /* Similar to above */
				ErrorCode code = yAxis->checkTrailers();
				if (code != ErrorCode::NoError) {
					this->status = CoordPlaneStatus::Idle;
					xAxis->stop();
					yAxis->stop();

					if (this->instr != nullptr && sync) {
						this->instr->unuse();
					}

					MotorErrorEvent merrevt = { code };
					xAxis->sendStoppedEvent(merrevt);
					yAxis->sendStoppedEvent(merrevt);
					CoordErrorEvent eevt = { code };
					sendStoppedEvent(eevt);
					xAxis->unuse();
					yAxis->unuse();
					unuse();
					return code;
				}
			}
		}

		/* Wait while one of the motors is still running.
		   Normally this loop will never execute, however
			 really motors can "lie" about their state */
		while (xAxis->getMotor()->isRunning() || yAxis->getMotor()->isRunning()) {
		}
		/* Reset status, usunse devices, emit events */
		this->status = CoordPlaneStatus::Idle;

		ErrorCode errcode = ErrorCode::NoError;
		if (this->instr != nullptr) {
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

	ErrorCode CoordController::calibrate(TrailerId tr) {
		/* Check if motors are idle. Otherwise return error
		   This should not happen on properly configured planes */
		if (this->xAxis->getMotor()->isRunning() ||
		    this->yAxis->getMotor()->isRunning()) {
			return ErrorCode::MotorRunning;
		}
		/* Check if motors are enabled */
		if (this->xAxis->getPowerState() == Power::NoPower ||
		    this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}
		/* Legacy if statement */
		if (tr != TrailerId::Trailer1 && tr != TrailerId::Trailer2) {
			return ErrorCode::WrongParameter;
		}
		/* Reset work flag. Check if plane currently can be used. Set plane status */
		work = defWork;
		if (!work) {
			return ErrorCode::NoError;
		}
		this->status = CoordPlaneStatus::Jump;
		/* Determne movement parameters. They are stored in configuration */
		int_conf_t roll_step =
		    config->getEntry("core")->getInt("roll_step", ROLL_STEP);
		int_conf_t roll_speed =
		    config->getEntry("core")->getInt("roll_speed", ROLL_SPEED);
		int_conf_t comeback =
		    config->getEntry("core")->getInt("trailer_comeback", TRAILER_COMEBACK);
		/* Determine movement destination */
		int_conf_t dest = (tr == TrailerId::Trailer1 ? -roll_step : roll_step);
		xAxis->dest =
		    (tr == TrailerId::Trailer1 ? MoveType::RollDown : MoveType::RollUp);
		yAxis->dest =
		    (tr == TrailerId::Trailer1 ? MoveType::RollDown : MoveType::RollUp);
		/* Variables that are used to detect if motor reached trailer */
		bool xpr = false;
		bool ypr = false;
		/* Mark motors as used. Emit events */
		MotorRollEvent mevt = { tr };
		if (this->instr != nullptr) {
			this->instr->use();
		}
		xAxis->use();
		yAxis->use();
		xAxis->sendRollingEvent(mevt);
		yAxis->sendRollingEvent(mevt);
		CoordCalibrateEvent evt = { tr };
		sendCalibratingEvent(evt);
		use();
		/* Run loop while at least one motor have not reached trailer
		   and further work is allowed.
		   Motors move by series of jumps, because it's easier to
		   prevent corruption if trailer is not enough sensetive */
		while (!(xpr && ypr) && work) {
			if (!xAxis->getMotor()->isTrailerPressed(static_cast<int>(tr))) {
				/* Trailer is not pressed, do one more motor jump if motor is idle */
				if (!xpr && !xAxis->getMotor()->isRunning()) {
					if (!xAxis->getMotor()->start(xAxis->getMotor()->getPosition() + dest,
					                              roll_speed)) {
						/* Error occured. Stop motors, emit errors, halt */
						this->status = CoordPlaneStatus::Idle;
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
				/* Trailer is pressed, flip appropriate flag */
				xpr = true;
				/* Stop motor if it's running */
				if (xAxis->getMotor()->isRunning()) {
					xAxis->getMotor()->stop();
				}
			}

			/* Similar as above */
			if (!yAxis->getMotor()->isTrailerPressed(static_cast<int>(tr))) {
				if (!ypr && !yAxis->getMotor()->isRunning()) {
					if (!yAxis->getMotor()->start(yAxis->getMotor()->getPosition() + dest,
					                              roll_speed)) {
						this->status = CoordPlaneStatus::Idle;
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
		/* Stop motors if they are still running.
		   Again, normally it would not happen, but
			 real motors may be more "complex" creatures */
		if (xAxis->getMotor()->isRunning()) {
			xAxis->getMotor()->stop();
		}
		if (yAxis->getMotor()->isRunning()) {
			yAxis->getMotor()->stop();
		}

		/* Mark motors as stopped */
		xAxis->dest = MoveType::Stop;
		yAxis->dest = MoveType::Stop;
		/* Do a little comeback jump without checking trailers.
		   Otherwise on the next movement software will detect trailer
			 as pressed and movement will be aborted */
		if (tr == TrailerId::Trailer2) {
			comeback *= -1;
		}
		if (work) {
			xAxis->startMove(xAxis->getMotor()->getPosition() + comeback, roll_speed);
			yAxis->startMove(yAxis->getMotor()->getPosition() + comeback, roll_speed);
			xAxis->waitWhileRunning();
			yAxis->waitWhileRunning();
		}
		/* Mark plane as idle, emit events, unuse devices */
		this->status = CoordPlaneStatus::Idle;
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

	/* Arc movement uses modified Bresenham's algorithm implementation.
	   Movement is determined by destination point (must by on the circle)
		 and circle center. Circle is split into chords according to spl parameter.
		 Scale parameter determines possible "error" ranges in circle coordinates */
	ErrorCode CoordController::arc(motor_point_t dest, motor_point_t center,
	                               int spl, float speed, bool clockwise,
	                               float scale) {
		/* Check if motors are enabled */
		if (this->xAxis->getPowerState() == Power::NoPower ||
		    this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}
		/* Check given coordinates. Current point and destination point must be 
		   on the same circle with center in given center point */
		motor_point_t src = this->getPosition();
		work = defWork;
		double r1 = pow(src.x - center.x, 2) + pow(src.y - center.y, 2);
		double r2 = pow(dest.x - center.x, 2) + pow(dest.y - center.y, 2);
		if (fabs(sqrt(r1) - sqrt(r2)) / scale >= COMPARISON_RADIUS) {
			work = false;
			return ErrorCode::ArcError;
		}
		/* Calculate circle length and one chord length based on splitter */
		double fullCircle = 2 * M_PI * sqrt(r1);
		uint64_t splitter = (uint64_t) ceil(fullCircle / spl);
		if (splitter == 0) {
			splitter = 1;
		}
		/* Pass circle parameters to the bresenham's algorithm */
		Circle cir(center, (int64_t) sqrt(r1), clockwise, scale);
		/* Algorithm is iterative, so firstly it should move to the start point */
		if (!cir.skip(src, &work)) {
			work = false;
			return ErrorCode::ArcError;
		}
		/* Starting real work */
		motor_point_t pnt;
		size_t count = 0;
		if (!work) {
			return ErrorCode::NoError;
		}
		/* Mark all devices as actively used */
		if (this->instr != nullptr) {
			this->instr->use();
		}
		xAxis->use();
		yAxis->use();
		use();
		/* Execute algorithm until a point somewhere around destination is achieved.
		   After that motors will simply move to the destination. Often destination point
			 is not quite accurate (because of float-int conversions, scaling, etc), sp
			 it is the only way to determine loop end */
		do {
			/* Work should be stopped */
			if (!work) {
				break;
			}
			/* Get the next point */
			if (clockwise) {
				pnt = cir.getPrevElement();
			} else {
				pnt = cir.getNextElement();
			}
			/* Check received point gives us new chord. If so, 
			   than move to it */
			if (count++ % splitter == 0) {
				ErrorCode err = this->move(pnt, speed, true);
				if (err != ErrorCode::NoError) {
					/* Something went wrong. Unuse devices, halt */
					if (this->instr != nullptr) {
						this->instr->unuse();
					}
					xAxis->unuse();
					yAxis->unuse();
					unuse();
					return err;
				}
				/* Update position */
				pnt = getPosition();
			}
		} while (abs(dest.x - pnt.x) / scale > COMPARISON_RADIUS ||
		         abs(dest.y - pnt.y) / scale > COMPARISON_RADIUS);
		/* Unuse devices */
		if (this->instr != nullptr) {
			this->instr->unuse();
		}
		xAxis->unuse();
		yAxis->unuse();
		unuse();
		/* Make final move to the destination point */
		ErrorCode code = ErrorCode::NoError;
		if (work) {
			code = this->move(dest, speed, true);
		}
		return code;
	}

	motor_rect_t CoordController::getSize() {
		return this->size;
	}

	bool CoordController::isMeasured() {
		return this->measured;
	}

	ErrorCode CoordController::measure(TrailerId tid) {
		/* Check if motors are enabled */
		if (this->xAxis->getPowerState() == Power::NoPower ||
		    this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}
		work = defWork;
		/* Determine start and finish trailers */
		TrailerId tid1 = (tid == TrailerId::Trailer1 ? TrailerId::Trailer2
		                                             : TrailerId::Trailer1);
		TrailerId tid2 = tid;
		/* Check if work is possible, then perform calibration twice */
		if (!work) {
			return ErrorCode::NoError;
		}
		ErrorCode errcode = this->calibrate(tid1);
		/* Get first trailer coordinates */
		motor_point_t max = getPosition();
		if (!work || errcode != ErrorCode::NoError) {
			return errcode;
		}
		errcode = this->calibrate(tid2);
		/* Get second trailer coordinates */
		motor_point_t min = getPosition();
		/* Calculate size and mark plane as measured */
		this->size = { min.x < max.x ? min.x : max.x, min.y < max.y ? min.y : max.y,
			             abs(max.x - min.x), abs(max.y - min.y) };
		this->measured = true;
		return errcode;
	}

	/* Aux. event-related methods */
	void CoordController::addEventListener(
	    std::shared_ptr<CoordEventListener> l) {
		this->listeners.push_back(l);
	}

	void CoordController::removeEventListener(
	    std::shared_ptr<CoordEventListener> l) {
		this->listeners.erase(
		    std::remove(this->listeners.begin(), this->listeners.end(), l),
		    this->listeners.end());
	}

	void CoordController::sendMovingEvent(CoordMoveEvent &evt) {
		for (const auto &l : this->listeners) {
			l->moving(evt);
		}
	}

	void CoordController::sendMovedEvent(CoordMoveEvent &evt) {
		for (const auto &l : this->listeners) {
			l->moved(evt);
		}
	}

	void CoordController::sendStoppedEvent(CoordErrorEvent &evt) {
		for (const auto &l : this->listeners) {
			l->stopped(evt);
		}
	}

	void CoordController::sendCalibratingEvent(CoordCalibrateEvent &evt) {
		for (const auto &l : this->listeners) {
			l->calibrating(evt);
		}
	}

	void CoordController::sendCalibratedEvent(CoordCalibrateEvent &evt) {
		for (const auto &l : this->listeners) {
			l->calibrated(evt);
		}
	}

	void CoordController::use() {
		for (const auto &l : this->listeners) {
			l->use();
		}
		xAxis->use();
		yAxis->use();
		if (this->instr != nullptr) {
			this->instr->use();
		}
	}

	void CoordController::unuse() {
		for (const auto &l : this->listeners) {
			l->unuse();
		}
		xAxis->unuse();
		yAxis->unuse();
		if (this->instr != nullptr) {
			this->instr->unuse();
		}
	}

	/* Stop motors and current operation */
	void CoordController::stop() {
		xAxis->stop();
		yAxis->stop();
		work = false;
	}

	/* Mark further work as impossible and stop plane */
	void CoordController::kill() {
		defWork = false;
		stop();
	}

	std::unique_ptr<CoordPlane> CoordController::clone(
	    std::shared_ptr<CoordPlane> base) {
		return std::make_unique<CoordController>(this->xAxis, this->yAxis,
		                                         this->config, this->instr);
	}

	CoordPlaneStatus CoordController::getStatus() {
		return this->status;
	}

	bool CoordController::isUsed() {
		return this->session_opened;
	}

	/* Mark session as opened */
	ErrorCode CoordController::open_session() {
		use();
		this->session_opened = true;
		LOG("CoordController", "Session opened");
		return ErrorCode::NoError;
	}

	/* Close session and disable instrument if necesarry */
	ErrorCode CoordController::close_session() {
		ErrorCode err = ErrorCode::NoError;
		if (this->instr != nullptr) {
			if (this->instr->isEnabled()) {
				err = this->instr->enable(false);
			}
			ErrorCode errcode = this->instr->close_session();
			if (err == ErrorCode::NoError) {
				err = errcode;
			}
		}
		unuse();
		this->session_opened = false;
		LOG("CoordController", "Session closed");
		return err;
	}
}  // namespace CalX
