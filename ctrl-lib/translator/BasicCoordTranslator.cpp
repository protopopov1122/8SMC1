#include "CoordTranslator.h"
#include <math.h>
#include <iostream>

namespace _8SMC1 {

	BasicCoordTranslator::BasicCoordTranslator(motor_point_t cen, motor_size_t sc) {
		this->center = cen;
		this->scale = sc;
	}

	BasicCoordTranslator::~BasicCoordTranslator() {
	}

	motor_point_t BasicCoordTranslator::getCenter() {
		return this->center;
	}

	motor_size_t BasicCoordTranslator::getScale() {
		return this->scale;
	}

	motor_point_t BasicCoordTranslator::get(long double x, long double y) {
		int64_t xtr = x * this->scale.w;
		int64_t ytr = y * this->scale.h;
		motor_point_t pnt = {0, 0};
		pnt.x = xtr;
		pnt.y = ytr;
		pnt.x += center.x;
		pnt.y += center.y;
		return pnt;
	}
	
	coord_point_t BasicCoordTranslator::get(motor_point_t pnt) {
		pnt.x -= center.x;
		pnt.y -= center.y;
		coord_point_t out = {(long double) pnt.x, (long double) pnt.y};
		out.x /= this->scale.w;
		out.y /= this->scale.h;
		return out;
	}
}
