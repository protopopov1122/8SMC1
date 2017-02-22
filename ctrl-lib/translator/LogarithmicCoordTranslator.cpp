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
	
	motor_point_t LogarithmicCoordTranslator::get(decimal_number x, decimal_number y) {		
		long double dx = x.i;
		long double div = 10;
		uint32_t j = x.j;
		while (j > 0) {
			dx += (j % 10) / div;
			div *= 10;
			j /= 10;
		}
		if (this->scale.x > 0) {
			dx = log(dx) / log(this->scale.x);
		}
		long double dy = y.i;
		div = 10;
		j = y.j;
		while (j > 0) {
			dy += (j % 10) / div;
			div *= 10;
			j /= 10;
		}
		if (this->scale.y > 0) {
			dy = log(dy) / log(this->scale.y);
		}
		return this->base->get(dx, dy);
	}
	
}