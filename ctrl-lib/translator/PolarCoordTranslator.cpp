#include <math.h>
#include "CoordTranslator.h"

namespace _8SMC1 {
	
	PolarCoordTranslator::PolarCoordTranslator(CoordTranslator *base) {
		this->base = base;
	}
	
	PolarCoordTranslator::~PolarCoordTranslator() {
		
	}
	
	CoordTranslator *PolarCoordTranslator::getBaseCoord() {
		return this->base;
	}
	
	motor_point_t PolarCoordTranslator::get(long double x, long double y) {
		long double nx = x * cos(y);
		long double ny = x * sin(y);
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
			out = {(long double) pnt.x, (long double) pnt.y};
		} else {
			out = this->base->get(pnt);
		}
		long double p = sqrt(pow(out.x, 2) + pow(out.y, 2));
		long double f = atan2(out.y, out.x);
		out = {p, f};
		return out;
	}
}