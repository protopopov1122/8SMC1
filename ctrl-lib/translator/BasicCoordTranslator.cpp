/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/translator/CoordTranslator.h"
#include <cmath>

namespace CalX {

	BasicCoordTranslator::BasicCoordTranslator(motor_point_t cen, motor_size_t sc)
	    : CoordTranslator::CoordTranslator(CoordType::BasicCoord),
	      center(cen),
	      scale(sc) {}

	motor_point_t BasicCoordTranslator::getCenter() const {
		return this->center;
	}

	motor_size_t BasicCoordTranslator::getScale() const {
		return this->scale;
	}

	void BasicCoordTranslator::setCenter(motor_point_t c) {
		this->center = c;
	}

	void BasicCoordTranslator::setScale(motor_size_t s) {
		this->scale = s;
	}

	motor_point_t BasicCoordTranslator::get(double x, double y) const {
		int64_t xtr = static_cast<int64_t>(x * this->scale.w);
		int64_t ytr = static_cast<int64_t>(y * this->scale.h);
		motor_point_t pnt = { 0, 0 };
		pnt.x = xtr;
		pnt.y = ytr;
		pnt.x += center.x;
		pnt.y += center.y;
		return pnt;
	}

	coord_point_t BasicCoordTranslator::get(motor_point_t pnt) const {
		pnt.x -= center.x;
		pnt.y -= center.y;
		coord_point_t out = { (double) pnt.x, (double) pnt.y };
		out.x /= this->scale.w;
		out.y /= this->scale.h;
		return out;
	}

	coord_point_t BasicCoordTranslator::floatGet(double x, double y) const {
		coord_point_t pnt = { x, y };
		pnt.x *= this->scale.w;
		pnt.y *= this->scale.h;
		pnt.x += this->center.x;
		pnt.y += this->center.y;
		return pnt;
	}

	coord_point_t BasicCoordTranslator::floatGet(coord_point_t pnt) const {
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
}  // namespace CalX
