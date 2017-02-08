#include "DeviceController.h"
#include <math.h>

namespace Controller {

	CoordController::CoordController(DeviceController *xaxis, DeviceController *yaxis) {
		this->xAxis = xaxis;
		this->yAxis = yaxis;
	}

	CoordController::~CoordController() {

	}

	DeviceController *CoordController::getXAxis() {
		return this->xAxis;
	}

	DeviceController *CoordController::getYAxis() {
		return this->yAxis;
	}

	err_code_t CoordController::move(motor_point_t point, float speed, int div, bool sync) {
		// TODO Enable proper motor syncing
		Device *xDev = this->xAxis->getDevice();
		Device *yDev = this->yAxis->getDevice();
		motor_coord_t dx = point.x - xDev->getPosition();
		motor_coord_t dy = point.y - yDev->getPosition();
		float y_speed = abs(sync ? speed * ((float) dy / dx) : speed);
		this->xAxis->startMove(point.x, speed, div, false);
		this->yAxis->startMove(point.y, y_speed, div, false);
		while (xDev->isRunning() || yDev->isRunning()) {
			if (xDev->isTrailerPressed(1) || yDev->isTrailerPressed(1)) {
				xDev->stop();
				yDev->stop();
				return ErrorCode::Trailer1Pressed;
			}
			if (xDev->isTrailerPressed(2) || yDev->isTrailerPressed(2)) {
				xDev->stop();
				yDev->stop();
				return ErrorCode::Trailer2Pressed;
			}
		}
		return ErrorCode::NoError;
	}
}
