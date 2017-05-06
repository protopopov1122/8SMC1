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


#include "CoordPlane.h"
#include "ctrl-lib/misc/CircleGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CalX {
	
	VirtualCoordPlane::VirtualCoordPlane(motor_point_t pos, motor_rect_t size) {
		this->position = pos;
		this->size = size;
		INIT_LOG("VirtualCoordPlane");
	}
	
	VirtualCoordPlane::~VirtualCoordPlane() {
		DESTROY_LOG("VirtualCoordPlane");
	}

	void VirtualCoordPlane::dump(std::ostream &os) {
		os << "virtual_coord";
	}
	
	ErrorCode VirtualCoordPlane::move(motor_point_t point, float speed, int div,
			bool sync) {
		this->position = point;
		this->jump(point, sync);
		return ErrorCode::NoError;
	}

	ErrorCode VirtualCoordPlane::calibrate(TrailerId tr) {
		if (tr == TrailerId::Trailer1) {
			this->position = {this->size.x, this->size.y};
		} else {
			this->position = {this->size.x + this->size.w, this->size.y + this->size.h};
		}
		this->jump(this->position, false);
		return ErrorCode::NoError;
	}

	motor_point_t VirtualCoordPlane::getPosition() {
		return this->position;
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
        uint32_t splitter = (uint32_t) ceil(fullCircle / spl);
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

	bool VirtualCoordPlane::isMeasured() {
		return true;
	}
	
	ErrorCode VirtualCoordPlane::measure(TrailerId tid) {
		return ErrorCode::NoError;
	}
	
	
	void VirtualCoordPlane::use() {
	}
	
	void VirtualCoordPlane::unuse() {
	}
	
	void VirtualCoordPlane::stop() {
	}
}
