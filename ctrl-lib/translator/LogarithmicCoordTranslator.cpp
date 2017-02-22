#include <math.h>
#include <iostream>
#include <iomanip>
#include "CoordTranslator.h"

namespace _8SMC1 {
	
	LogarithmicCoordTranslator::LogarithmicCoordTranslator(CoordTranslator *base, motor_scale_t scale) {
		this->base = base;
		this->scale = scale;
	}
	
	LogarithmicCoordTranslator::~LogarithmicCoordTranslator() {
		delete this->base;
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
		return this->base->get(x, y);
	}
	
	motor_point_t LogarithmicCoordTranslator::get(int64_t x, int64_t y) {
		if (this->scale.x > 0) {
			x = log(x) / log(this->scale.x);
		}
		if (this->scale.y > 0) {
			y = log(y) / log(this->scale.y);
		}
		return this->base->get(x, y);
	}
	
}