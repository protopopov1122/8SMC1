/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "calx/ctrl-lib/translator/CoordTranslator.h"

namespace CalX {

	LinearCoordTranslator::LinearCoordTranslator(
	    coord_point_t cen, coord_scale_t sc, std::shared_ptr<CoordTranslator> bs)
	    : CoordTranslator::CoordTranslator(CoordType::LinearCoord),
	      base(bs),
	      offset(cen),
	      scale(sc) {}

	coord_point_t LinearCoordTranslator::getOffset() const {
		return this->offset;
	}

	coord_scale_t LinearCoordTranslator::getScale() const {
		return this->scale;
	}

	std::shared_ptr<CoordTranslator> LinearCoordTranslator::getBase() const {
		return this->base;
	}

	void LinearCoordTranslator::setBase(std::shared_ptr<CoordTranslator> t) {
		this->base = t;
	}

	void LinearCoordTranslator::setOffset(coord_point_t o) {
		this->offset = o;
	}

	void LinearCoordTranslator::setScale(coord_scale_t s) {
		this->scale = s;
	}

	motor_point_t LinearCoordTranslator::get(double x, double y) const {
		double nx = x * this->scale.x + this->offset.x;
		double ny = y * this->scale.y + this->offset.y;
		if (this->base == nullptr) {
			motor_point_t pnt = { (int64_t) nx, (int64_t) ny };
			return pnt;
		} else {
			return this->base->get(nx, ny);
		}
	}

	coord_point_t LinearCoordTranslator::get(motor_point_t pnt) const {
		coord_point_t out;
		if (this->base == nullptr) {
			out = { (double) pnt.x, (double) pnt.y };
		} else {
			out = this->base->get(pnt);
		}
		out.x -= this->offset.x;
		out.y -= this->offset.y;
		out.x /= this->scale.x;
		out.y /= this->scale.y;
		return out;
	}

	coord_point_t LinearCoordTranslator::floatGet(double x, double y) const {
		coord_point_t pnt = { x, y };
		pnt.x *= this->scale.x;
		pnt.y *= this->scale.y;
		pnt.x += this->offset.x;
		pnt.y += this->offset.y;
		if (this->base == nullptr) {
			return pnt;
		} else {
			return this->base->floatGet(pnt.x, pnt.y);
		}
	}

	coord_point_t LinearCoordTranslator::floatGet(coord_point_t pnt) const {
		coord_point_t out;
		if (this->base == nullptr) {
			out = pnt;
		} else {
			out = this->base->floatGet(pnt);
		}
		out.x -= this->offset.x;
		out.y -= this->offset.y;
		out.x /= this->scale.x;
		out.y /= this->scale.y;
		return out;
	}

	std::unique_ptr<CoordTranslator> LinearCoordTranslator::clone(
	    std::shared_ptr<CoordTranslator> base) {
		if (base == nullptr && this->base != nullptr) {
			base = this->base->clone(nullptr);
		}
		return std::make_unique<LinearCoordTranslator>(this->offset, this->scale,
		                                               base);
	}
}  // namespace CalX
