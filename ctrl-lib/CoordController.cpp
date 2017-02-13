#include "DeviceController.h"
#include "CircleGenerator.h"
#include <iostream>
#include <stdio.h>
#include <math.h>

namespace _8SMC1 {

	CoordController::CoordController(DeviceController *xaxis,
			DeviceController *yaxis) {
		this->xAxis = xaxis;
		this->yAxis = yaxis;
		this->xAxis->getDevice()->setSyncInputMode(true);
		this->yAxis->getDevice()->setOutputSyncEnabled(true);
	}

	CoordController::~CoordController() {

	}

	DeviceController *CoordController::getXAxis() {
		return this->xAxis;
	}

	DeviceController *CoordController::getYAxis() {
		return this->yAxis;
	}

	ErrorCode CoordController::move(motor_point_t point, float speed, int div,
			bool sync) {
		// TODO Enable proper motor syncing
		Device *xDev = this->xAxis->getDevice();
		Device *yDev = this->yAxis->getDevice();
		motor_coord_t dx = point.x - xDev->getPosition();
		motor_coord_t dy = point.y - yDev->getPosition();

		// Calculate x and y axis speed v=sqrt(vx^2 + vy^2); vy = n*vx

		float x_speed;
		float y_speed;
		if (!sync) {
			x_speed = speed;
			y_speed = speed;
		} else if (dx == 0) {
			x_speed = 0;
			y_speed = speed;
		} else if (dy == 0) {
			x_speed = speed;
			y_speed = 0;
		} else {
			float ncoef = ((float) dy) / dx;
			x_speed = sqrt(speed * speed / (1 + ncoef * ncoef));
			y_speed = ncoef * x_speed;
		}

		//this->xAxis->startMove(point.x, x_speed, div, sync);
		//this->yAxis->startMove(point.y, y_speed, div, false);
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
//		if (xDev->isTrailerPressed(1) || yDev->isTrailerPressed(1)) {
//			xDev->stop();
//			yDev->stop();
//			return ErrorCode::Trailer1Pressed;
//		}
//		if (xDev->isTrailerPressed(2) || yDev->isTrailerPressed(2)) {
//			xDev->stop();
//			yDev->stop();
//			return ErrorCode::Trailer2Pressed;
//		}
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

	ErrorCode CoordController::calibrate(int tr) {
		int comeback = TRAILER_COMEBACK;
		if (this->xAxis->getDevice()->isRunning()
				|| this->yAxis->getDevice()->isRunning()) {
			return ErrorCode::DeviceRunning;
		}
		if (tr != 1 && tr != 2) {
			return ErrorCode::WrongParameter;
		}
//			xAxis->moveToTrailer(tr, TRAILER_COMEBACK);
//			yAxis->moveToTrailer(tr, TRAILER_COMEBACK);
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
		pos.x = xAxis->getPosition();
		pos.y = yAxis->getPosition();
		return pos;
	}

	ErrorCode CoordController::arc(motor_point_t dest, motor_point_t center, int splitter,
				float speed, int div, bool clockwise) {
		motor_point_t src = this->getPosition();
		int r1 = (src.x - center.x) * (src.x - center.x) +
			     (src.y - center.y) * (src.y - center.y);
		int r2 = (dest.x - center.x) * (dest.x - center.x) +
			     (dest.y - center.y) * (dest.y - center.y);
		if (r1 != r2) {
			return ErrorCode::ArcError;
		}
		Circle cir(center, (int) sqrt(r1), clockwise);
		/*int add = clockwise ? 1 : -1;
		int start = cir.getFullSize() - 1;
		while (cir.getElement(start).x != src.x ||
			cir.getElement(start).y != src.y) {
			start += add;
		}
		std::vector<motor_point_t> path;
		while (cir.getElement(start).x != dest.x ||
			cir.getElement(start).y != dest.y) {
			motor_point_t point = cir.getElement(start);
			path.push_back(point);
			start += add;
		}
		for (size_t i = 0; i < (unsigned  int) splitter; i++) {
			motor_point_t pnt = path.at(path.size() / splitter * i);
			ErrorCode err = this->move(path.at(path.size() / splitter * i), speed, div, true);
			if (err !=ErrorCode::NoError) {
				return err;
			}
		}*/
		if (!cir.skip(src)) {
			return ErrorCode::ArcError;
		}
		motor_point_t pnt;
		int count = 0;
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
			}
		} while (pnt.x != dest.x || pnt.y != dest.y);
		return this->move(dest, speed, div, true);
	}
}
