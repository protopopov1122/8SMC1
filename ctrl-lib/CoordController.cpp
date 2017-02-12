#include "DeviceController.h"
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
		float ncoef = ((float) dy) / dx;
		float x_speed = sqrt(speed * speed / (1 + ncoef * ncoef));
		float y_speed = ncoef * x_speed;
		if (!sync) {
			x_speed = speed;
			y_speed = speed;
		}

		this->xAxis->startMove(point.x, x_speed, div, sync);
		this->yAxis->startMove(point.y, y_speed, div, false);
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
		while (!(xAxis->getDevice()->isTrailerPressed(tr)
				&& yAxis->getDevice()->isTrailerPressed(tr))) {
			if (!xAxis->getDevice()->isTrailerPressed(tr)) {
				if (!xAxis->getDevice()->isRunning()) {
					xAxis->getDevice()->start(
							xAxis->getDevice()->getPosition() + dest,
							ROLL_SPEED, ROLL_DIV);
				}
			} else if (xAxis->getDevice()->isRunning()) {
				xAxis->getDevice()->stop();
			}

			if (!yAxis->getDevice()->isTrailerPressed(tr)) {
				if (!yAxis->getDevice()->isRunning()) {
					yAxis->getDevice()->start(
							yAxis->getDevice()->getPosition() + dest,
							ROLL_SPEED, ROLL_DIV);
				}
			} else if (yAxis->getDevice()->isRunning()) {
				yAxis->getDevice()->stop();
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
}
