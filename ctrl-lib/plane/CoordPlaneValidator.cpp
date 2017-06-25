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


#include "ctrl-lib/plane/CoordPlane.h"

namespace CalX {

	CoordPlaneValidator::CoordPlaneValidator(motor_point_t min, motor_point_t max, float maxspeed, CoordPlane *cplane) {
		this->plane = cplane;
		this->min = min;
		this->max = max;
		this->max_speed = maxspeed;
		INIT_LOG("CoordPlaneValidator");
	}

	CoordPlaneValidator::~CoordPlaneValidator() {
		DESTROY_LOG("CoordPlaneValidator");
	}

	CoordPlane *CoordPlaneValidator::getBase() {
		return this->plane;
	}

	motor_point_t CoordPlaneValidator::getMinimum() {
		return this->min;
	}

	motor_point_t CoordPlaneValidator::getMaximum() {
		return this->max;
	}

	float CoordPlaneValidator::getMaxSpeed() {
		return this->max_speed;
	}

	void CoordPlaneValidator::setMinimum(motor_point_t p) {
		this->min = p;
	}

	void CoordPlaneValidator::setMaximum(motor_point_t p) {
		this->max = p;
	}

	void CoordPlaneValidator::setMaxSpeed(float s) {
		this->max_speed = s;
	}

	ErrorCode CoordPlaneValidator::move(motor_point_t dest, float speed, int div, bool sync) {
		if (speed < 0 || speed > this->max_speed) {
			return ErrorCode::InvalidSpeed;
		}
		if (dest.x < this->min.x ||
			dest.y < this->min.y ||
			dest.x >= this->max.x ||
			dest.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;
		}
		return this->plane->move(dest, speed, div, sync);
	}

	ErrorCode CoordPlaneValidator::arc(motor_point_t dest, motor_point_t center, int splitter, float speed, int div, bool clockwise, float scale, bool strict) {
		if (speed < 0 || speed > this->max_speed) {
			return ErrorCode::InvalidSpeed;
		}
		if (dest.x < this->min.x ||
			dest.y < this->min.y ||
			dest.x >= this->max.x ||
			dest.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;
		}
		if (center.x < this->min.x ||
			center.y < this->min.y ||
			center.x >= this->max.x ||
			center.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;
		}
		return this->plane->arc(dest, center, splitter, speed, div, clockwise, scale, strict);
	}

	ErrorCode CoordPlaneValidator::calibrate(TrailerId tr) {
		return this->plane->calibrate(tr);
	}

	ErrorCode CoordPlaneValidator::measure(TrailerId tr) {
		return this->plane->measure(tr);
	}

	motor_point_t CoordPlaneValidator::getPosition() {
			return this->plane->getPosition();
	}

	motor_rect_t CoordPlaneValidator::getSize() {
		return this->plane->getSize();
	}

	bool CoordPlaneValidator::isMeasured() {
		return this->plane->isMeasured();
	}

	void CoordPlaneValidator::dump(std::ostream &os) {
		os << "validator(min=" << this->min.x << "x" << this->min.y
			<< "; max=" << this->max.x << "x" << this->max.y
			<< "; speed=" << this->max_speed << ")";
	}

	void CoordPlaneValidator::use() {
		this->plane->use();
	}

	void CoordPlaneValidator::unuse() {
		this->plane->unuse();
	}

	void CoordPlaneValidator::stop() {
		this->plane->stop();
	}

	CoordPlane *CoordPlaneValidator::clone(CoordPlane *base) {
		return new CoordPlaneValidator(this->min, this->max, this->max_speed, base);
	}

	ErrorCode CoordPlaneValidator::open_session() {
		return this->plane->open_session();
	}

	ErrorCode CoordPlaneValidator::close_session() {
		return this->plane->close_session();
	}
}
