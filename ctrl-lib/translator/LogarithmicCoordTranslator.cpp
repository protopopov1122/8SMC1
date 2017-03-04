#include <math.h>
#include <iostream>
#include <iomanip>
#include "CoordTranslator.h"

namespace _8SMC1 {
	
	LogarithmicCoordTranslator::LogarithmicCoordTranslator(coord_scale_t scale, CoordTranslator *base)
		: CoordTranslator::CoordTranslator(CoordType::LogarithmicCoord) {
		this->base = base;
		this->scale = scale;
	}
	
	LogarithmicCoordTranslator::~LogarithmicCoordTranslator() {
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
	
	motor_point_t LogarithmicCoordTranslator::get(long double x, long double y) {
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
			out = {(long double) pnt.x, (long double) pnt.y};
		} else {
			out = this->base->get(pnt);
		}
		out.x = pow(this->scale.x, out.x);
		out.y = pow(this->scale.x, out.y);
		return out;
	}
	
}