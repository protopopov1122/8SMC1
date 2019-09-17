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

#include "calx/ctrl-lib/plane/CoordPlane.h"
#include <iostream>

namespace CalX {

	CoordPlaneMap::CoordPlaneMap(motor_point_t offset, motor_scale_t scale,
	                             float speedSc, std::shared_ptr<CoordPlane> plane)
	    : ProxyCoordPlane::ProxyCoordPlane(plane) {
		this->offset = offset;
		this->scale = scale;
		this->speedScale = speedSc;
	}

	motor_point_t CoordPlaneMap::getOffset() const {
		return this->offset;
	}

	motor_scale_t CoordPlaneMap::getScale() const {
		return this->scale;
	}

	float CoordPlaneMap::getSpeedScale() const {
		return this->speedScale;
	}

	void CoordPlaneMap::setOffset(motor_point_t p) {
		this->offset = p;
	}

	void CoordPlaneMap::setScale(motor_scale_t s) {
		this->scale = s;
	}

	void CoordPlaneMap::setSpeedScale(float sc) {
		this->speedScale = sc;
	}

	ErrorCode CoordPlaneMap::move(motor_point_t dest, float speed, bool sync) {
		dest.x *= this->scale.x;
		dest.y *= this->scale.y;
		dest.x += this->offset.x;
		dest.y += this->offset.y;
		return this->base->move(dest, speed * this->speedScale, sync);
	}

	ErrorCode CoordPlaneMap::arc(motor_point_t dest, motor_point_t center,
	                             int splitter, float speed, bool clockwise,
	                             float scale) {
		dest.x *= this->scale.x;
		dest.y *= this->scale.y;
		dest.x += this->offset.x;
		dest.y += this->offset.y;

		center.x *= this->scale.x;
		center.y *= this->scale.y;
		center.x += this->offset.x;
		center.y += this->offset.y;

		scale *= (this->scale.x + this->scale.y) / 2;
		return this->base->arc(dest, center, splitter, speed * this->speedScale,
		                       clockwise, scale);
	}

	motor_point_t CoordPlaneMap::getPosition() const {
		motor_point_t pnt = this->base->getPosition();
		pnt.x -= this->offset.x;
		pnt.y -= this->offset.y;
		pnt.x /= this->scale.x;
		pnt.y /= this->scale.y;
		return pnt;
	}

	motor_rect_t CoordPlaneMap::getSize() const {
		motor_rect_t sz = this->base->getSize();
		sz.x -= this->offset.x;
		sz.y -= this->offset.y;
		sz.x /= this->scale.x;
		sz.y /= this->scale.y;
		sz.w /= this->scale.x;
		sz.h /= this->scale.y;
		return sz;
	}

	std::ostream &CoordPlaneMap::operator<<(std::ostream &os) const {
		os << "map(offset=" << this->offset.x << "x" << this->offset.y
		   << "; scale=" << this->scale.x << "x" << this->scale.y
		   << "; speed=" << this->speedScale << ")";
		return os;
	}

	std::unique_ptr<CoordPlane> CoordPlaneMap::clone(
	    std::shared_ptr<CoordPlane> base) {
		return std::make_unique<CoordPlaneMap>(this->offset, this->scale,
		                                       this->speedScale, base);
	}
}  // namespace CalX
