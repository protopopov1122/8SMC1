#include "CoordTranslator.h"
#include <math.h>
#include <iostream>

namespace _8SMC1 {

	LinearCoordTranslator::LinearCoordTranslator(motor_point_t cen, motor_size_t sc) {
		this->center = cen;
		this->scale = sc;
	}

	LinearCoordTranslator::~LinearCoordTranslator() {
	}

	motor_point_t LinearCoordTranslator::getCenter() {
		return this->center;
	}

	motor_size_t LinearCoordTranslator::getScale() {
		return this->scale;
	}

	motor_point_t LinearCoordTranslator::get(long double x, long double y) {
		int64_t xtr = x * this->scale.w;
		int64_t ytr = y * this->scale.h;
		motor_point_t pnt = {0, 0};
		pnt.x = xtr;
		pnt.y = ytr;
		pnt.x += center.x;
		pnt.y += center.y;
		return pnt;
	}
	
	motor_point_t LinearCoordTranslator::get(int64_t x, int64_t y) {
		motor_point_t pnt = {x, y};
		pnt.x /= this->scale.w;
		pnt.y /= this->scale.h;
		pnt.x += this->center.x;
		pnt.y += this->center.y;
		return pnt;
	}
}
