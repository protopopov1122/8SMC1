#include "plane/CoordPlane.h"
#include "misc/CircleGenerator.h"
#include <iostream>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace _8SMC1 {

	CoordController::CoordController(DeviceController *xaxis,
			DeviceController *yaxis) {
		this->xAxis = xaxis;
		this->yAxis = yaxis;
		this->xAxis->getDevice()->setSyncInputMode(true);
		this->yAxis->getDevice()->setOutputSyncEnabled(true);
		this->size = {0, 0, 0, 0};
	}

	CoordController::~CoordController() {

	}

	void CoordController::dump(std::ostream &os) {
		os << "coord(" << this->xAxis->getID() << "; " << this->yAxis->getID() << ")";
	}

	DeviceController *CoordController::getXAxis() {
		return this->xAxis;
	}

	DeviceController *CoordController::getYAxis() {
		return this->yAxis;
	}

	ErrorCode CoordController::move(motor_point_t point, float speed, int div,
			bool sync) {
		//point.x /= this->scale.w;
		//point.y /= this->scale.h;
		// TODO Enable proper motor syncing
		Device *xDev = this->xAxis->getDevice();
		Device *yDev = this->yAxis->getDevice();
		motor_coord_t dx = point.x - xDev->getPosition();
		motor_coord_t dy = point.y - yDev->getPosition();

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

		if (xAxis->dev->isRunning() || yAxis->dev->isRunning()) {
			return ErrorCode::DeviceRunning;
		}
		xAxis->dest = point.x > xAxis->dev->getPosition() ? MoveType::MoveUp :
				MoveType::MoveDown;
		xAxis->dev->start(point.x, x_speed, div, false);
		
		yAxis->dest = point.y > yAxis->dev->getPosition() ? MoveType::MoveUp :
				MoveType::MoveDown;
		yAxis->dev->start(point.y, y_speed, div, false);
		while (xDev->isRunning() || yDev->isRunning()) {
			if (xDev->isRunning()) {
				ErrorCode code = xAxis->checkTrailers();
				if (code != ErrorCode::NoError) {
					xAxis->stop();
					yAxis->stop();
					return code;
				}
			}
			if (yDev->isRunning()) {
				ErrorCode code = yAxis->checkTrailers();
				if (code != ErrorCode::NoError) {
					xAxis->stop();
					yAxis->stop();
					return code;
				}
			}
		}
		return ErrorCode::NoError;
	}


	ErrorCode CoordPlane::relativeMove(motor_point_t relpoint, float speed, int div,
			bool sync) {
		motor_point_t point = getPosition();
		point.x += relpoint.x;
		point.y += relpoint.y;
		return move(point, speed, div, sync);
	}

	ErrorCode CoordController::calibrate(TrailerId tr) {
		int comeback = TRAILER_COMEBACK;
		if (this->xAxis->getDevice()->isRunning()
				|| this->yAxis->getDevice()->isRunning()) {
			return ErrorCode::DeviceRunning;
		}
		if (tr != 1 && tr != 2) {
			return ErrorCode::WrongParameter;
		}
		int dest = (tr == 1 ? -ROLL_STEP : ROLL_STEP);
		xAxis->dest = (tr == 1 ? MoveType::RollDown : MoveType::RollUp);
		yAxis->dest = (tr == 1 ? MoveType::RollDown : MoveType::RollUp);
		bool xpr = false;
		bool ypr = false;
		while (!(xpr && ypr)) {
			if (!xAxis->getDevice()->isTrailerPressed(tr)) {
				if (!xpr && !xAxis->getDevice()->isRunning()) {
					xAxis->getDevice()->start(
							xAxis->getDevice()->getPosition() + dest,
							ROLL_SPEED, ROLL_DIV);
				}
			} else  {
				xpr = true;
				if (xAxis->getDevice()->isRunning()) {
					xAxis->getDevice()->stop();
				}
			}
				

			if (!yAxis->getDevice()->isTrailerPressed(tr)) {
				if (!ypr && !yAxis->getDevice()->isRunning()) {
					yAxis->getDevice()->start(
							yAxis->getDevice()->getPosition() + dest,
							ROLL_SPEED, ROLL_DIV);
				}
			} else {
				ypr = true;
				if (yAxis->getDevice()->isRunning()) {
					yAxis->getDevice()->stop();
				}
			}
		}
		if (xAxis->getDevice()->isRunning()) {
			xAxis->getDevice()->stop();
		}
		if (yAxis->getDevice()->isRunning()) {
			yAxis->getDevice()->stop();
		}

		xAxis->dest = MoveType::Stop;
		yAxis->dest = MoveType::Stop;
		if (tr == 2) {
			comeback *= -1;
		}
		xAxis->startMove(xAxis->getDevice()->getPosition() + comeback,
		ROLL_SPEED, ROLL_DIV);
		yAxis->startMove(yAxis->getDevice()->getPosition() + comeback,
		ROLL_SPEED, ROLL_DIV);

		xAxis->resetPosition();
		yAxis->resetPosition();
		return ErrorCode::NoError;
	}

	motor_point_t CoordController::getPosition() {
		motor_point_t pos;
		pos.x = xAxis->getPosition();// * this->scale.w;
		pos.y = yAxis->getPosition();// * this->scale.h;
		return pos;
	}

	ErrorCode CoordController::arc(motor_point_t dest, motor_point_t center, int spl,
				float speed, int div, bool clockwise, bool strict) {
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
		do {
			if (clockwise) {
				pnt = cir.getPrevElement();
			} else {
				pnt = cir.getNextElement();
			}
			if (count++ % splitter == 0) {
				ErrorCode err = this->move(pnt, speed, div, true);
				if (err != ErrorCode::NoError) {
					return err;
				}
				pnt = getPosition();
			}
		} while (abs(dest.x - pnt.x) > COMPARISON_RADIUS ||
			abs(dest.y - pnt.y) > COMPARISON_RADIUS);
		return this->move(dest, speed, div, true);
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
	
	void CoordController::measure(TrailerId tid) {
		TrailerId tid1 = (tid == TrailerId::Trailer1 ? TrailerId::Trailer2 : TrailerId::Trailer1);
		TrailerId tid2 = tid;
		this->calibrate(tid1);
		motor_point_t min = getPosition();
		this->calibrate(tid2);
		motor_point_t max = getPosition();
		this->size = {min.x, min.y, abs(max.x - min.x), abs(max.y - min.y)};
	}
}
