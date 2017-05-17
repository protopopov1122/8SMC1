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
#include <iostream>
#include <stdio.h>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CalX {

	CoordPlaneLinearizer::CoordPlaneLinearizer(CoordPlane *base) {
		this->base = base;
		this->work = false;
		this->defWork = true;
		INIT_LOG("CoordPlaneLinearizer");
	}
	
	CoordPlaneLinearizer::~CoordPlaneLinearizer() {
		DESTROY_LOG("CoordPlaneLinearizer");
	}
	
	CoordPlane *CoordPlaneLinearizer::getBase() {
		return this->base;
	}
	
	ErrorCode CoordPlaneLinearizer::move(motor_point_t dest, float speed, int div, bool sync) {
		return this->base->move(dest, speed, div, sync);
	}
	
	ErrorCode CoordPlaneLinearizer::arc(motor_point_t dest, motor_point_t center, int spl, float speed, int div, bool clockwise, bool strict) {
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
        int64_t count = 0;
		work = defWork;
		if (!work) {
			return ErrorCode::NoError;
		}
		use();
		do {
			if (!work) {
				break;
			}
			if (clockwise) {
				pnt = cir.getPrevElement();
			} else {
				pnt = cir.getNextElement();
			}
			if (count++ % splitter == 0) {
				ErrorCode err = this->move(pnt, speed, div, true);
				if (err != ErrorCode::NoError) {
					unuse();
					return err;
				}
				pnt = getPosition();
			}
		} while (abs(dest.x - pnt.x) > COMPARISON_RADIUS ||
			abs(dest.y - pnt.y) > COMPARISON_RADIUS);
		unuse();
		ErrorCode code = ErrorCode::NoError;
		if (work) {
			code = this->move(dest, speed, div, true);
		}
		return code;
	}
	
	ErrorCode CoordPlaneLinearizer::calibrate(TrailerId tr) {
		return this->base->calibrate(tr);
	}
	
	ErrorCode CoordPlaneLinearizer::measure(TrailerId tr) {
		return this->base->measure(tr);
	}
	
	motor_point_t CoordPlaneLinearizer::getPosition() {
		return this->base->getPosition();
	}
	
	motor_rect_t CoordPlaneLinearizer::getSize() {
		return this->base->getSize();
	}

	bool CoordPlaneLinearizer::isMeasured() {
		return this->base->isMeasured();
	}

	void CoordPlaneLinearizer::dump(std::ostream &os) {
		os << "linearizer";
	}
	
	void CoordPlaneLinearizer::use() {
		this->base->use();
	}
	
	void CoordPlaneLinearizer::unuse() {
		this->base->unuse();
	}
	
	void CoordPlaneLinearizer::stop() {
		this->base->stop();
	}
	
	CoordPlane *CoordPlaneLinearizer::clone(CoordPlane *base) {
		return new CoordPlaneLinearizer(base);
	}
	
	ErrorCode CoordPlaneLinearizer::open_session() {
		return this->base->open_session();
	}
	
	ErrorCode CoordPlaneLinearizer::close_session() {
		return this->base->close_session();
	}
}
