/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#include "ctrl-lib/misc/CircleGenerator.h"
#include "ctrl-lib/plane/CoordPlane.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CalX {

	CoordPlaneLinearizer::CoordPlaneLinearizer(std::shared_ptr<CoordPlane> base)
	    : ProxyCoordPlane::ProxyCoordPlane(base) {
		this->work = false;
		this->defWork = true;
	}

	ErrorCode CoordPlaneLinearizer::arc(motor_point_t dest, motor_point_t center,
	                                    int spl, float speed, bool clockwise,
	                                    float scale) {
		motor_point_t src = this->getPosition();
		work = defWork;
		double r1 = pow(src.x - center.x, 2) + pow(src.y - center.y, 2);
		double r2 = pow(dest.x - center.x, 2) + pow(dest.y - center.y, 2);
		if (fabs(sqrt(r1) - sqrt(r2)) / scale >= COMPARISON_RADIUS) {
			work = false;
			return ErrorCode::ArcError;
		}
		double fullCircle = 2 * M_PI * sqrt(r1);
		int64_t splitter = (int64_t) ceil(fullCircle / spl);
		if (splitter == 0) {
			splitter = 1;
		}
		Circle cir(center, (int64_t) sqrt(r1), clockwise, scale);
		if (!cir.skip(src, &work)) {
			work = false;
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
				ErrorCode err = this->move(pnt, speed, true);
				if (err != ErrorCode::NoError) {
					unuse();
					return err;
				}
				pnt = getPosition();
			}
		} while (abs(dest.x - pnt.x) / scale > COMPARISON_RADIUS ||
		         abs(dest.y - pnt.y) / scale > COMPARISON_RADIUS);
		unuse();
		ErrorCode code = ErrorCode::NoError;
		if (work) {
			code = this->move(dest, speed, true);
		}
		return code;
	}

	std::unique_ptr<CoordPlane> CoordPlaneLinearizer::clone(
	    std::shared_ptr<CoordPlane> base) {
		return std::make_unique<CoordPlaneLinearizer>(base);
	}

	std::ostream &CoordPlaneLinearizer::operator<<(std::ostream &os) const {
		os << "linearizer";
		return os;
	}
}  // namespace CalX
