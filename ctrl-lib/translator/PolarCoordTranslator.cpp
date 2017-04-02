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


#include <math.h>
#include "CoordTranslator.h"

namespace CalX {
	
	PolarCoordTranslator::PolarCoordTranslator(CoordTranslator *base)
		: CoordTranslator::CoordTranslator(CoordType::PolarCoord) {
		this->base = base;
		INIT_LOG("PolarCoordTranslator");
	}
	
	PolarCoordTranslator::~PolarCoordTranslator() {
		DESTROY_LOG("PolarCoordTranslator");
	}
	
	CoordTranslator *PolarCoordTranslator::getBase() {
		return this->base;
	}
	
	void PolarCoordTranslator::setBase(CoordTranslator *t) {
		this->base = t;
	}
	
	motor_point_t PolarCoordTranslator::get(double x, double y) {
		double nx = x * cos(y);
		double ny = x * sin(y);
		if (this->base == nullptr) {
			motor_point_t pnt = {(int64_t) nx, (int64_t) ny};
			return pnt;
		} else {
			return this->base->get(nx, ny);
		}
	}
	
	coord_point_t PolarCoordTranslator::get(motor_point_t pnt) {
		coord_point_t out;
		if (this->base == nullptr) {
			out = {(double) pnt.x, (double) pnt.y};
		} else {
			out = this->base->get(pnt);
		}
		double p = sqrt(pow(out.x, 2) + pow(out.y, 2));
		double f = atan2(out.y, out.x);
		out = {p, f};
		return out;
	}
}