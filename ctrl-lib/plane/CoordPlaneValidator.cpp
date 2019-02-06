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

#include "calx/ctrl-lib/plane/CoordPlane.h"
#include <iostream>

namespace CalX {

	CoordPlaneValidator::CoordPlaneValidator(motor_point_t min, motor_point_t max,
	                                         float maxspeed,
	                                         std::shared_ptr<CoordPlane> cplane)
	    : ProxyCoordPlane::ProxyCoordPlane(cplane) {
		this->min = min;
		this->max = max;
		this->max_speed = maxspeed;
	}

	motor_point_t CoordPlaneValidator::getMinimum() const {
		return this->min;
	}

	motor_point_t CoordPlaneValidator::getMaximum() const {
		return this->max;
	}

	float CoordPlaneValidator::getMaxSpeed() const {
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

	ErrorCode CoordPlaneValidator::move(motor_point_t dest, float speed,
	                                    bool sync) {
		if (speed < 0 || speed > this->max_speed) {
			return ErrorCode::InvalidSpeed;
		}
		if (dest.x < this->min.x || dest.y < this->min.y || dest.x >= this->max.x ||
		    dest.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;
		}
		return this->base->move(dest, speed, sync);
	}

	ErrorCode CoordPlaneValidator::arc(motor_point_t dest, motor_point_t center,
	                                   int splitter, float speed, bool clockwise,
	                                   float scale) {
		if (speed < 0 || speed > this->max_speed) {
			return ErrorCode::InvalidSpeed;
		}
		if (dest.x < this->min.x || dest.y < this->min.y || dest.x >= this->max.x ||
		    dest.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;
		}
		if (center.x < this->min.x || center.y < this->min.y ||
		    center.x >= this->max.x || center.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;
		}
		return this->base->arc(dest, center, splitter, speed, clockwise, scale);
	}

	std::ostream &CoordPlaneValidator::operator<<(std::ostream &os) const {
		os << "validator(min=" << this->min.x << "x" << this->min.y
		   << "; max=" << this->max.x << "x" << this->max.y
		   << "; speed=" << this->max_speed << ")";
		return os;
	}

	std::unique_ptr<CoordPlane> CoordPlaneValidator::clone(
	    std::shared_ptr<CoordPlane> base) {
		return std::make_unique<CoordPlaneValidator>(this->min, this->max,
		                                             this->max_speed, base);
	}
}  // namespace CalX
