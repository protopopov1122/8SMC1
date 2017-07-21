/*
        Copyright (c) 2017 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ctrl-lib/translator/CoordTranslator.h"
#include <iostream>
#include <math.h>

namespace CalX {

	BasicCoordTranslator::BasicCoordTranslator(motor_point_t cen, motor_size_t sc)
	    : CoordTranslator::CoordTranslator(CoordType::BasicCoord) {
		this->center = cen;
		this->scale = sc;
		INIT_LOG("BasicCoordTranslator");
	}

	BasicCoordTranslator::~BasicCoordTranslator() {
		DESTROY_LOG("BasicCoordTranslator");
	}

	motor_point_t BasicCoordTranslator::getCenter() {
		return this->center;
	}

	motor_size_t BasicCoordTranslator::getScale() {
		return this->scale;
	}

	void BasicCoordTranslator::setCenter(motor_point_t c) {
		this->center = c;
	}

	void BasicCoordTranslator::setScale(motor_size_t s) {
		this->scale = s;
	}

	motor_point_t BasicCoordTranslator::get(double x, double y) {
		int64_t xtr = static_cast<int64_t>(x * this->scale.w);
		int64_t ytr = static_cast<int64_t>(y * this->scale.h);
		motor_point_t pnt = { 0, 0 };
		pnt.x = xtr;
		pnt.y = ytr;
		pnt.x += center.x;
		pnt.y += center.y;
		return pnt;
	}

	coord_point_t BasicCoordTranslator::get(motor_point_t pnt) {
		pnt.x -= center.x;
		pnt.y -= center.y;
		coord_point_t out = { (double) pnt.x, (double) pnt.y };
		out.x /= this->scale.w;
		out.y /= this->scale.h;
		return out;
	}

	coord_point_t BasicCoordTranslator::floatGet(double x, double y) {
		coord_point_t pnt = { x, y };
		pnt.x *= this->scale.w;
		pnt.y *= this->scale.h;
		pnt.x += this->center.x;
		pnt.y += this->center.y;
		return pnt;
	}

	coord_point_t BasicCoordTranslator::floatGet(coord_point_t pnt) {
		pnt.x -= this->center.x;
		pnt.y -= this->center.y;
		pnt.x /= this->scale.w;
		pnt.y /= this->scale.h;
		return pnt;
	}

	std::unique_ptr<CoordTranslator> BasicCoordTranslator::clone(
	    std::shared_ptr<CoordTranslator> base) {
		return std::make_unique<BasicCoordTranslator>(this->center, this->scale);
	}
}
