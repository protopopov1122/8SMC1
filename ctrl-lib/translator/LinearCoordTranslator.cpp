#include "CoordTranslator.h"

namespace _8SMC1 {

	LinearCoordTranslator::LinearCoordTranslator(coord_point_t cen, coord_scale_t sc, CoordTranslator *base) {
		this->base = base;
		this->offset = cen;
		this->scale = sc;
	}

	LinearCoordTranslator::~LinearCoordTranslator() {
	}

	coord_point_t LinearCoordTranslator::getOffset() {
		return this->offset;
	}

	coord_scale_t LinearCoordTranslator::getScale() {
		return this->scale;
	}
	
	CoordTranslator *LinearCoordTranslator::getBase() {
		return this->base;
	}
	
	void LinearCoordTranslator::setOffset(coord_point_t o) {
		this->offset = o;
	}
	
	void LinearCoordTranslator::setScale(coord_scale_t s) {
		this->scale = s;
	}

	motor_point_t LinearCoordTranslator::get(long double x, long double y) {
		long double nx = x * this->scale.x + this->offset.x;
		long double ny = y * this->scale.y + this->offset.y;
		if (this->base == nullptr) {
			motor_point_t pnt = {(int64_t) nx, (int64_t) ny};
			return pnt;
		} else {
			return this->base->get(nx, ny);
		}
	}
	
	coord_point_t LinearCoordTranslator::get(motor_point_t pnt) {
		coord_point_t out;
		if (this->base == nullptr) {
			out = {(long double) pnt.x, (long double) pnt.y};
		} else {
			out = this->base->get(pnt);
		}
		out.x -= this->offset.x;
		out.y -= this->offset.y;
		out.x /= this->scale.x;
		out.y /= this->scale.y;
		return out;
	}
}
