#include <CoordTranslator.h>

namespace _8SMC1 {

	CoordTranslator::CoordTranslator(motor_point_t cen, motor_size_t sz, motor_size_t sc) {
		this->center = cen;
		this->size = sz;
		this->scale = sc;
	}

	CoordTranslator::~CoordTranslator() {
	}

	motor_point_t CoordTranslator::getCenter() {
		return this->center;
	}

	motor_size_t CoordTranslator::getSize() {
		return this->size;
	}

	motor_size_t CoordTranslator::getScale() {
		return this->scale;
	}

	motor_point_t CoordTranslator::get(float x, float y) {
		int64_t xtr = x * this->scale.w;
		int64_t ytr = y * this->scale.h;
		motor_point_t pnt = {0, 0};
		if (!(xtr > this->size.w / 2 ||
			xtr < -this->size.w / 2 ||
			ytr > this->size.h / 2 ||
			ytr < -this->size.h / 2)) {
			pnt.x = xtr;
			pnt.y = ytr;
		}
		pnt.x += center.x;
		pnt.y += center.y;
		return pnt;
	}
}
