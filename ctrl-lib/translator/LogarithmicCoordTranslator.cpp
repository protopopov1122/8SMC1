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
#include <iostream>
#include <iomanip>
#include "CoordTranslator.h"

namespace CalX {
	
	LogarithmicCoordTranslator::LogarithmicCoordTranslator(coord_scale_t scale, CoordTranslator *base)
		: CoordTranslator::CoordTranslator(CoordType::LogarithmicCoord) {
		this->base = base;
		this->scale = scale;
		INIT_LOG("LogarithmicCoordTranslator");
	}
	
	LogarithmicCoordTranslator::~LogarithmicCoordTranslator() {
		DESTROY_LOG("LogarithmicCoordTranslator");
	}
	
	CoordTranslator *LogarithmicCoordTranslator::getBase() {
		return this->base;
	}
	
	void LogarithmicCoordTranslator::setBase(CoordTranslator *t) {
		this->base = t;
	}
	
	coord_scale_t LogarithmicCoordTranslator::getScale() {
		return this->scale;
	}
	
	void LogarithmicCoordTranslator::setScale(coord_scale_t s) {
		this->scale = s;
	}
	
	motor_point_t LogarithmicCoordTranslator::get(double x, double y) {
		if (this->scale.x > 0) {
			x = log(x) / log(this->scale.x);
		}
		if (this->scale.y > 0) {
			y = log(y) / log(this->scale.y);
		}
		if (this->base == nullptr) {
			motor_point_t pnt = {(int64_t) x, (int64_t) y};
			return pnt;
		} else {
			return this->base->get(x, y);
		}
	}
	
	coord_point_t LogarithmicCoordTranslator::get(motor_point_t pnt) {
		coord_point_t out;
		if (this->base == nullptr) {
			out = {(double) pnt.x, (double) pnt.y};
		} else {
			out = this->base->get(pnt);
		}
		if (this->scale.x != 0) {
			out.x = pow(this->scale.x, out.x);
		}
		if (this->scale.y != 0) {
			out.y = pow(this->scale.y, out.y);
		}
		return out;
	}

	CoordTranslator *LogarithmicCoordTranslator::clone(CoordTranslator *base) {
		if (base == nullptr) {
			base = this->base->clone(nullptr);
		}
		return new LogarithmicCoordTranslator(this->scale, base);
	}
	
}