#include <math.h>
#include <iostream>
#include <iomanip>
#include "CoordTranslator.h"

namespace _8SMC1 {
	
	LogarithmicCoordTranslator::LogarithmicCoordTranslator(motor_scale_t scale, CoordTranslator *base) {
		this->base = base;
		this->scale = scale;
	}
	
	LogarithmicCoordTranslator::~LogarithmicCoordTranslator() {
	}
	
	CoordTranslator *LogarithmicCoordTranslator::getBaseCoord() {
		return this->base;
	}
	
	motor_scale_t LogarithmicCoordTranslator::getScale() {
		return this->scale;
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
	
}