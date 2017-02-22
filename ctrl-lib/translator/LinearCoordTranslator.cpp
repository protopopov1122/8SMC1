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
	
	motor_point_t LinearCoordTranslator::get(decimal_number x, decimal_number y) {
			
		int8_t xs = x.s == 0 ? 1 : -1;
		int8_t ys = y.s == 0 ? 1 : -1;
		int32_t ix = x.i;
		int32_t jx = x.j;
		int32_t iy = y.i;
		int32_t jy = y.j;
				
				
		motor_point_t out;
		out.x = ix * this->scale.w;
		out.y = iy * this->scale.h;
				
				
		uint32_t divisor = 10;
		uint32_t ax = 0;
		while (jx > 0) {
			ax += (jx % 10) * this->scale.w / divisor;
			divisor *= 10;
			jx /= 10;
		}
		out.x += ax;
		
		uint32_t ay = 0;
		divisor = 10;
		while (jy > 0) {
			ay += (jy % 10) * this->scale.h / divisor;
			divisor *= 10;
			jy /= 10;
		}
		out.y += ay;
		
		out.x *= xs;
		out.y *= ys;
				
		out.x += this->center.x;
		out.y += this->center.y;
		
		return out;
	}
}
