/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "calx/ctrl-lib/misc/CircleGenerator.h"
#include "calx/ctrl-lib/plane/CoordPlane.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CalX {

	const char *COORD_CTRL_TAG = "CoordCtrl";

	class CoordStatusHandle {
	 public:
		CoordStatusHandle(CoordPlaneStatus *status, CoordPlaneStatus current)
		    : status(status) {
			*this->status = current;
		}
		~CoordStatusHandle() {
			*this->status = CoordPlaneStatus::Idle;
		}

	 private:
		CoordPlaneStatus *status;
	};

	CoordController::CoordController(ConfigurationCatalogue &config,
	                                 std::shared_ptr<MotorController> xaxis,
	                                 std::shared_ptr<MotorController> yaxis,
	                                 std::shared_ptr<InstrumentController> instr)
	    : xAxis(xaxis), yAxis(yaxis), instr(instr), config(config) {
		this->size = { 0, 0, 0, 0 };
		this->defWork = true;
		this->work = false;
		this->measured = false;
		this->status = CoordPlaneStatus::Idle;
		LOG(COORD_CTRL_TAG,
		    "New coordinate controller. X Axis: #" +
		        std::to_string(this->xAxis->getID()) + "; Y Axis: #" +
		        std::to_string(this->yAxis->getID()) + "; instrument: " +
		        std::string(instr != nullptr ? std::to_string(instr->getID())
		                                     : "no"));
	}

	std::ostream &CoordController::operator<<(std::ostream &os) const {
		os << "coord(" << this->xAxis->getID() << "; " << this->yAxis->getID()
		   << ")";
		return os;
	}

	std::shared_ptr<MotorController> CoordController::getXAxis() const {
		return this->xAxis;
	}

	std::shared_ptr<MotorController> CoordController::getYAxis() const {
		return this->yAxis;
	}

	std::shared_ptr<InstrumentController> CoordController::getInstrument() const {
		return this->instr;
	}

	ErrorCode CoordController::move(motor_point_t point, float speed, bool sync) {
		/* Prepare motors and determine movement destination on each axis */
		motor_coord_t dx = point.x - this->xAxis->getPosition();
		motor_coord_t dy = point.y - this->yAxis->getPosition();
		/* Check if motors are enabled */
		if (this->xAxis->getPowerState() == Power::NoPower ||
		    this->yAxis->getPowerState() == Power::NoPower) {
			return ErrorCode::PowerOff;
		}

		/* In jump(asynchronous) mode speed can be increased to the maximal value
		   Depending on configuration settings */
		if (!sync) {
			float maxspeed = this->config.getEntry(CalxConfiguration::Core)
			                     ->getReal(CalxCoreConfiguration::JumpSpeed, 0.0f);
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
		if (xAxis->isMoving() || yAxis->isMoving()) {
			return ErrorCode::MotorRunning;
		}

		/* Mark motors as active used */
		ResourceUse xAxisUse(*this->xAxis);
		ResourceUse yAxisUse(*this->yAxis);
		ResourceUse instrUse;

		/* Determine if instrument is present, mark it as used and enable it (if
		   necesarry). Even if the instrument is enabled here, it will continue work
		   after method finished
		   (for optimisation purposes) and will be disabled on session close */
		if (this->instr != nullptr) {
			instrUse.swap(*this->instr);
			ErrorCode errcode = this->instr->enable(sync);
			/* Error while enabling instrument, unuse all, halt */
			if (errcode != ErrorCode::NoError) {
				return errcode;
			}
		}

		/* Update current plane status */
		CoordStatusHandle statusHandle(
		    &this->status, sync ? CoordPlaneStatus::Move : CoordPlaneStatus::Jump);
		/* Start x-axis motor, emit event about it */
		ErrorCode errcode = ErrorCode::NoError;
		if ((errcode = xAxis->asyncMove(point.x, x_speed)) !=
		    ErrorCode::NoError) { /* Something went wrong. Unuse motors, set
			     status to idle, halt */
			xAxis->asyncStop(errcode, point.x, x_speed);
			return errcode;
		}

		/* Start y-axis motor, emit event about it */
		if ((errcode = yAxis->asyncMove(point.y, y_speed)) !=
		    ErrorCode::NoError) { /* Something went wrong. Stop
			                           x-axis, unuse motors, etc. */
			xAxis->asyncStop(errcode, point.x, x_speed);
			yAxis->asyncStop(errcode, point.y, y_speed);
			return errcode;
		}

		/* Emit event about coordinate plane using, mark it as used */
		CoordMoveEvent evt = { point, speed, sync };
		sendMovingEvent(evt);
		/* Wait while one of motors (or both) is running, detect possible errors */
		while (this->xAxis->isMoving() || this->yAxis->isMoving()) {
			if (this->xAxis->isMoving()) {
				ErrorCode code = xAxis->checkTrailers();
				if (code != ErrorCode::NoError) { /* Motor reached trailes, stop plane,
					                                   unuse devices, produce error */
					xAxis->asyncStop(code, point.x, x_speed);
					yAxis->asyncStop(code, point.y, y_speed);

					/* Emit necesarry errors */
					CoordErrorEvent eevt = { code };
					sendStoppedEvent(eevt);
					return code;
				}
			}
			if (this->yAxis->isMoving()) { /* Similar to above */
				ErrorCode code = yAxis->checkTrailers();
				if (code != ErrorCode::NoError) {
					xAxis->asyncStop(code, point.x, x_speed);
					yAxis->asyncStop(code, point.y, y_speed);

					CoordErrorEvent eevt = { code };
					sendStoppedEvent(eevt);
					return code;
				}
			}
		}

		/* Wait while one of the motors is still running.
		   Normally this loop will never execute, however
		   really motors can "lie" about their state */
		while (xAxis->isMoving() || yAxis->isMoving()) {
		}
		xAxis->asyncStop(ErrorCode::NoError, point.x, x_speed);
		yAxis->asyncStop(ErrorCode::NoError, point.y, y_speed);
		/* Reset status, usunse devices, emit events */

		sendMovedEvent(evt);
		return ErrorCode::NoError;
	}

	ErrorCode CoordController::calibrate(TrailerId tr) {
		/* Check if motors are idle. Otherwise return error
		   This should not happen on properly configured planes */
		if (this->xAxis->isMoving() || this->yAxis->isMoving()) {
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
		/* Reset work flag. Check if plane currently can be used. Set plane status
		 */
		work = defWork;
		if (!work) {
			return ErrorCode::NoError;
		}
		CoordStatusHandle statusHandle(&this->status, CoordPlaneStatus::Jump);
		/* Determne movement parameters. They are stored in configuration */
		int_conf_t roll_step =
		    config.getEntry(CalxConfiguration::Core)
		        ->getInt(CalxMotorConfiguration::RollStep, ROLL_STEP);
		int_conf_t roll_speed =
		    config.getEntry(CalxConfiguration::Core)
		        ->getInt(CalxMotorConfiguration::RollSpeed, ROLL_SPEED);
		int_conf_t comeback =
		    config.getEntry(CalxConfiguration::Core)
		        ->getInt(CalxMotorConfiguration::TrailerComeback, TRAILER_COMEBACK);
		/* Determine movement destination */
		int_conf_t dest = (tr == TrailerId::Trailer1 ? -roll_step : roll_step);
		/* Variables that are used to detect if motor reached trailer */
		bool xpr = false;
		bool ypr = false;
		/* Mark motors as used. Emit events */
		ResourceUse xAxisUse(*this->xAxis);
		ResourceUse yAxisUse(*this->yAxis);
		ResourceUse instrUse;
		if (this->instr != nullptr) {
			instrUse.swap(*this->instr);
		}
		CoordCalibrateEvent evt = { tr };
		sendCalibratingEvent(evt);
		/* Run loop while at least one motor have not reached trailer
		   and further work is allowed.
		   Motors move by series of jumps, because it's easier to
		   prevent corruption if trailer is not enough sensetive */
		ErrorCode errcode = ErrorCode::NoError;
		while (!(xpr && ypr) && work) {
			if (!xAxis->isTrailerPressed(tr)) {
				/* Trailer is not pressed, do one more motor jump if motor is idle */
				if (!xpr && !xAxis->isMoving()) {
					if ((errcode = xAxis->asyncMove(xAxis->getPosition() + dest,
					                                roll_speed)) != ErrorCode::NoError) {
						/* Error occured. Stop motors, emit errors, halt */;
						xAxis->asyncStop(errcode, xAxis->getPosition() + dest, roll_speed);
						yAxis->asyncStop(errcode, yAxis->getPosition() + dest, roll_speed);
						sendCalibratedEvent(evt);
						return ErrorCode::LowLevelError;
					}
				}
			} else {
				/* Trailer is pressed, flip appropriate flag */
				xpr = true;
				/* Stop motor if it's running */
				if (xAxis->isMoving()) {
					xAxis->stop();
				}
			}

			/* Similar as above */
			if (!yAxis->isTrailerPressed(tr)) {
				if (!ypr && !yAxis->isMoving()) {
					if ((errcode = yAxis->asyncMove(yAxis->getPosition() + dest,
					                                roll_speed)) != ErrorCode::NoError) {
						xAxis->asyncStop(errcode, xAxis->getPosition() + dest, roll_speed);
						yAxis->asyncStop(errcode, yAxis->getPosition() + dest, roll_speed);
						sendCalibratedEvent(evt);
						return ErrorCode::LowLevelError;
					}
				}
			} else {
				ypr = true;
				if (yAxis->isMoving()) {
					yAxis->stop();
				}
			}
		}
		/* Stop motors if they are still running.
		   Again, normally it would not happen, but
		   real motors may be more "complex" creatures */
		xAxis->asyncStop(errcode, xAxis->getPosition(), roll_speed);
		yAxis->asyncStop(errcode, yAxis->getPosition(), roll_speed);

		/* Do a little comeback jump without checking trailers.
		   Otherwise on the next movement software will detect trailer
		   as pressed and movement will be aborted */
		if (tr == TrailerId::Trailer2) {
			comeback *= -1;
		}
		if (work) {
			xAxis->startMove(xAxis->getPosition() + comeback, roll_speed);
			yAxis->startMove(yAxis->getPosition() + comeback, roll_speed);
			xAxis->waitWhileRunning();
			yAxis->waitWhileRunning();
		}
		/* Mark plane as idle, emit events, unuse devices */
		sendCalibratedEvent(evt);
		return ErrorCode::NoError;
	}

	motor_point_t CoordController::getPosition() const {
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
		std::size_t count = 0;
		if (!work) {
			return ErrorCode::NoError;
		}
		/* Mark all devices as actively used */
		ResourceUse xAxisUse(*this->xAxis);
		ResourceUse yAxisUse(*this->yAxis);
		ResourceUse instrUse;
		if (this->instr != nullptr) {
			instrUse.swap(*this->instr);
		}
		/* Execute algorithm until a point somewhere around destination is achieved.
		   After that motors will simply move to the destination. Often destination
		   point is not quite accurate (because of float-int conversions, scaling,
		   etc), sp it is the only way to determine loop end */
		do {
			/* Work should be stopped */
			if (!work) {
				break;
			}
			/* Get the next point */
			pnt = cir.next();
			/* Check received point gives us new chord. If so,
			   than move to it */
			if (count++ % splitter == 0) {
				ErrorCode err = this->move(pnt, speed, true);
				if (err != ErrorCode::NoError) {
					/* Something went wrong. Unuse devices, halt */
					return err;
				}
				/* Update position */
				pnt = getPosition();
			}
		} while (abs(dest.x - pnt.x) / scale > COMPARISON_RADIUS ||
		         abs(dest.y - pnt.y) / scale > COMPARISON_RADIUS);
		/* Make final move to the destination point */
		ErrorCode code = ErrorCode::NoError;
		if (work) {
			code = this->move(dest, speed, true);
		}
		return code;
	}

	motor_rect_t CoordController::getSize() const {
		return this->size;
	}

	bool CoordController::isMeasured() const {
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
	void CoordController::sendMovingEvent(CoordMoveEvent &evt) const {
		this->submitEvent(&CoordEventListener::moving, evt);
	}

	void CoordController::sendMovedEvent(CoordMoveEvent &evt) const {
		this->submitEvent(&CoordEventListener::moved, evt);
	}

	void CoordController::sendStoppedEvent(CoordErrorEvent &evt) const {
		this->submitEvent(&CoordEventListener::stopped, evt);
	}

	void CoordController::sendCalibratingEvent(CoordCalibrateEvent &evt) const {
		this->submitEvent(&CoordEventListener::calibrating, evt);
	}

	void CoordController::sendCalibratedEvent(CoordCalibrateEvent &evt) const {
		this->submitEvent(&CoordEventListener::calibrated, evt);
	}

	void CoordController::use() {
		UsableResource::use();
		this->submitEvent(&CoordEventListener::onUse);
		xAxis->use();
		yAxis->use();
		if (this->instr != nullptr) {
			this->instr->use();
		}
	}

	void CoordController::unuse() {
		UsableResource::unuse();
		this->submitEvent(&CoordEventListener::onUnuse);
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
		return std::make_unique<CoordController>(this->config, this->xAxis,
		                                         this->yAxis, this->instr);
	}

	CoordPlaneStatus CoordController::getStatus() const {
		return this->status;
	}

	bool CoordController::isSessionOpened() const {
		return SessionableResource::isSessionOpened();
	}

	/* Mark session as opened */
	ErrorCode CoordController::open_session() {
		use();
		SessionableResource::open_session();
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
		SessionableResource::close_session();
		LOG("CoordController", "Session closed");
		return err;
	}
}  // namespace CalX
