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
}