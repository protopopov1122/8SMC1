/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/misc/CircleGenerator.h"
#include "calx/ctrl-lib/plane/CoordPlane.h"
#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CalX {

	VirtualCoordPlane::VirtualCoordPlane(motor_point_t pos, motor_rect_t size) {
		this->position = pos;
		this->size = size;
	}

	std::ostream &VirtualCoordPlane::operator<<(std::ostream &os) const {
		os << "virtual_coord";
		return os;
	}

	ErrorCode VirtualCoordPlane::move(motor_point_t point, float speed,
	                                  bool sync) {
		this->position = point;
		if (this->jump(point, sync)) {
			return ErrorCode::NoError;
		} else {
			return ErrorCode::InternalInterrupted;
		}
	}

	ErrorCode VirtualCoordPlane::calibrate(TrailerId tr) {
		if (tr == TrailerId::Trailer1) {
			this->position = { this->size.x, this->size.y };
		} else {
			this->position = { this->size.x + this->size.w,
				                 this->size.y + this->size.h };
		}
		this->jump(this->position, false);
		return ErrorCode::NoError;
	}

	motor_point_t VirtualCoordPlane::getPosition() const {
		return this->position;
	}

	ErrorCode VirtualCoordPlane::arc(motor_point_t dest, motor_point_t center,
	                                 int spl, float speed, bool clockwise,
	                                 float scale) {
		motor_point_t src = this->getPosition();
		double r1 = pow(src.x - center.x, 2) + pow(src.y - center.y, 2);
		double r2 = pow(dest.x - center.x, 2) + pow(dest.y - center.y, 2);
		if (fabs(sqrt(r1) - sqrt(r2)) / scale >= COMPARISON_RADIUS) {
			return ErrorCode::ArcError;
		}
		double fullCircle = 2 * M_PI * sqrt(r1);
		uint32_t splitter = (uint32_t) ceil(fullCircle / spl);
		if (splitter == 0) {
			splitter = 1;
		}
		Circle cir(center, (int64_t) sqrt(r1), clockwise, scale);
		if (!cir.skip(src, nullptr)) {
			return ErrorCode::ArcError;
		}
		motor_point_t pnt;
		std::size_t count = 0;
		do {
			pnt = cir.next();
			if (count++ % splitter == 0) {
				ErrorCode err = this->move(pnt, speed, true);
				if (err != ErrorCode::NoError) {
					return err;
				}
				pnt = getPosition();
			}
		} while (abs(dest.x - pnt.x) / scale > COMPARISON_RADIUS ||
		         abs(dest.y - pnt.y) / scale > COMPARISON_RADIUS);
		return this->move(dest, speed, true);
	}

	motor_rect_t VirtualCoordPlane::getSize() const {
		return this->size;
	}

	bool VirtualCoordPlane::isMeasured() const {
		return true;
	}

	ErrorCode VirtualCoordPlane::measure(TrailerId tid) {
		return ErrorCode::NoError;
	}

	void VirtualCoordPlane::use() {}

	void VirtualCoordPlane::unuse() {}

	void VirtualCoordPlane::stop() {}

	CoordPlaneStatus VirtualCoordPlane::getStatus() const {
		return CoordPlaneStatus::Idle;
	}

	ErrorCode VirtualCoordPlane::open_session() {
		return ErrorCode::NoError;
	}

	ErrorCode VirtualCoordPlane::close_session() {
		return ErrorCode::NoError;
	}

	bool VirtualCoordPlane::isSessionOpened() const {
		return false;
	}
}  // namespace CalX
