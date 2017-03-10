#include "CoordPlane.h"
#include "ctrl-lib/misc/CircleGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CalX {
	
	VirtualCoordPlane::VirtualCoordPlane(VirtualDevice *x, VirtualDevice *y) {
		this->xDev = x;
		this->yDev = y;
		this->size = {0, 0, 0, 0};
	}
	
	VirtualCoordPlane::~VirtualCoordPlane() {
		delete this->xDev;
		delete this->yDev;
	}
	
	VirtualDevice *VirtualCoordPlane::getXAxis() {
		return this->xDev;
	}
	
	VirtualDevice *VirtualCoordPlane::getYAxis() {
		return this->yDev;
	}

	void VirtualCoordPlane::dump(std::ostream &os) {
		os << "virtual_coord";
	}
	
	ErrorCode VirtualCoordPlane::move(motor_point_t point, float speed, int div,
			bool sync) {
		motor_coord_t dx = point.x - xDev->getPosition();
		motor_coord_t dy = point.y - yDev->getPosition();

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
	
		xDev->start(point.x, x_speed, div);
		yDev->start(point.y, y_speed, div);
		if (xDev->isTrailerPressed(TrailerId::Trailer1) ||
			yDev->isTrailerPressed(TrailerId::Trailer1)) {
			return ErrorCode::Trailer1Pressed;	
		}
		if (xDev->isTrailerPressed(TrailerId::Trailer2) ||
			yDev->isTrailerPressed(TrailerId::Trailer2)) {
			return ErrorCode::Trailer2Pressed;	
		}
		return ErrorCode::NoError;
	}

	ErrorCode VirtualCoordPlane::calibrate(TrailerId tr) {
		int comeback = TRAILER_COMEBACK;
		if (tr != 1 && tr != 2) {
			return ErrorCode::WrongParameter;
		}
		bool xpr = false;
		bool ypr = false;
		int dest = (tr == 1 ? -ROLL_STEP : ROLL_STEP);
		while (!(xpr && ypr)) {
			if (!xDev->isTrailerPressed(tr)) {
				if (!xpr) {
					xDev->start(xDev->getPosition() + dest,
							ROLL_SPEED, ROLL_DIV);
				}
			} else  {
				xpr = true;
			}
				

			if (!yDev->isTrailerPressed(tr)) {
				if (!ypr) {
					yDev->start(yDev->getPosition() + dest,
							ROLL_SPEED, ROLL_DIV);
				}
			} else {
				ypr = true;
			}
		}

		if (tr == 2) {
			comeback *= -1;
		}
		xDev->start(xDev->getPosition() + comeback,
			ROLL_SPEED, ROLL_DIV);
		yDev->start(yDev->getPosition() + comeback,
			ROLL_SPEED, ROLL_DIV);
			
		return ErrorCode::NoError;
	}

	motor_point_t VirtualCoordPlane::getPosition() {
		motor_point_t pos;
		pos.x = xDev->getPosition();
		pos.y = yDev->getPosition();
		return pos;
	}

	ErrorCode VirtualCoordPlane::arc(motor_point_t dest, motor_point_t center, int spl,
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
	
	motor_rect_t VirtualCoordPlane::getSize() {
		return this->size;
	}
	
	void VirtualCoordPlane::measure(TrailerId tid) {
		TrailerId tid1 = (tid == TrailerId::Trailer1 ? TrailerId::Trailer2 : TrailerId::Trailer1);
		TrailerId tid2 = tid;
		this->calibrate(tid1);
		motor_point_t min = getPosition();
		this->calibrate(tid2);
		motor_point_t max = getPosition();
		this->size = {min.x, min.y, abs(max.x - min.x), abs(max.y - min.y)};
	}
	
	
	void VirtualCoordPlane::use() {
	}
	
	void VirtualCoordPlane::unuse() {
	}
	
	void VirtualCoordPlane::stop() {
	}
}