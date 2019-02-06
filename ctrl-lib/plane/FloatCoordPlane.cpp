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

#include "calx/ctrl-lib/plane/CoordPlane.h"
#include <iostream>
#include <cmath>

namespace CalX {

	FloatCoordPlane::FloatCoordPlane(coord_point_t offs, coord_scale_t scl,
	                                 double sp, std::shared_ptr<CoordPlane> bs)
	    : ProxyCoordPlane::ProxyCoordPlane(bs),
	      offset(offs),
	      scale(scl),
	      speed(sp) {}

	coord_point_t FloatCoordPlane::getOffset() const {
		return this->offset;
	}

	coord_scale_t FloatCoordPlane::getScale() const {
		return this->scale;
	}

	double FloatCoordPlane::getSpeedScale() const {
		return this->speed;
	}

	void FloatCoordPlane::setBase(std::shared_ptr<CoordPlane> pl) {
		this->base = pl;
	}

	void FloatCoordPlane::setOffset(coord_point_t offset) {
		this->offset = offset;
	}

	void FloatCoordPlane::setScale(coord_scale_t scale) {
		this->scale = scale;
	}

	void FloatCoordPlane::setSpeedScale(double s) {
		this->speed = s;
	}

	ErrorCode FloatCoordPlane::move(motor_point_t dest, float speed, bool sync) {
		dest.x *= this->scale.x;
		dest.y *= this->scale.y;
		dest.x += this->offset.x;
		dest.y += this->offset.y;
		return this->base->move(dest, speed * this->speed, sync);
	}

	ErrorCode FloatCoordPlane::arc(motor_point_t dest, motor_point_t center,
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
		return this->base->arc(dest, center, splitter, speed * this->speed,
		                       clockwise, scale);
	}

	motor_point_t FloatCoordPlane::getPosition() const {
		motor_point_t pnt = this->base->getPosition();
		pnt.x -= this->offset.x;
		pnt.y -= this->offset.y;
		pnt.x /= this->scale.x;
		pnt.y /= this->scale.y;
		return pnt;
	}

	motor_rect_t FloatCoordPlane::getSize() const {
		motor_rect_t sz = this->base->getSize();
		sz.x -= this->offset.x;
		sz.y -= this->offset.y;
		sz.x /= this->scale.x;
		sz.y /= this->scale.y;
		sz.w /= this->scale.x;
		sz.h /= this->scale.y;
		return sz;
	}

	std::ostream &FloatCoordPlane::operator<<(std::ostream &os) const {
		os << "float(offset=" << this->offset.x << "x" << this->offset.y
		   << "; scale=" << this->scale.x << "x" << this->scale.y
		   << "; speed=" << this->speed << ")";
		return os;
	}

	std::unique_ptr<CoordPlane> FloatCoordPlane::clone(
	    std::shared_ptr<CoordPlane> base) {
		return std::make_unique<FloatCoordPlane>(this->offset, this->scale,
		                                         this->speed, base);
	}

	ErrorCode FloatCoordPlane::move(coord_point_t dest, double speed, bool sync) {
		motor_point_t pdest = { static_cast<motor_coord_t>(
			                          round(dest.x * this->scale.x + this->offset.x)),
			                      static_cast<motor_coord_t>(round(
			                          dest.y * this->scale.y + this->offset.y)) };
		return this->base->move(pdest, static_cast<float>(speed * this->speed),
		                        sync);
	}

	ErrorCode FloatCoordPlane::arc(coord_point_t dest, coord_point_t center,
	                               int splitter, double speed, bool clockwise,
	                               float scale) {
		motor_point_t pdest = { static_cast<motor_coord_t>(
			                          round(dest.x * this->scale.x + this->offset.x)),
			                      static_cast<motor_coord_t>(round(
			                          dest.y * this->scale.y + this->offset.y)) };

		motor_point_t pcen = { static_cast<motor_coord_t>(round(
			                         center.x * this->scale.x + this->offset.x)),
			                     static_cast<motor_coord_t>(round(
			                         center.y * this->scale.y + this->offset.y)) };

		return this->base->arc(pdest, pcen, splitter,
		                       static_cast<float>(speed * this->speed), clockwise,
		                       scale);
	}

	coord_point_t FloatCoordPlane::getFloatPosition() const {
		motor_point_t pnt = this->base->getPosition();
		coord_point_t out = {
			(static_cast<double>(pnt.x) - this->offset.x) / this->scale.x,
			(static_cast<double>(pnt.y) - this->offset.y) / this->scale.y
		};
		return out;
	}

	coord_rect_t FloatCoordPlane::getFloatSize() const {
		motor_rect_t sz = this->base->getSize();
		coord_rect_t out = {
			(static_cast<double>(sz.x) - this->offset.x) / this->scale.x,
			(static_cast<double>(sz.y) - this->offset.y) / this->scale.y,
			static_cast<double>(sz.w) / this->scale.x,
			static_cast<double>(sz.h) / this->scale.y
		};
		return out;
	}

	ErrorCode FloatCoordPlane::relativeMove(coord_point_t relpoint, float speed,
	                                        bool sync) {
		coord_point_t point = getFloatPosition();
		point.x += relpoint.x;
		point.y += relpoint.y;
		return move(point, speed, sync);
	}

	ErrorCode FloatCoordPlane::relativeArc(coord_point_t reldest,
	                                       coord_point_t relcenter, int splitter,
	                                       float speed, bool clockwise,
	                                       float scale) {
		coord_point_t dest = getFloatPosition();
		coord_point_t center = getFloatPosition();
		dest.x += reldest.x;
		dest.y += reldest.y;
		center.x += relcenter.x;
		center.y += relcenter.y;
		return arc(dest, center, splitter, speed, clockwise, scale);
	}
}  // namespace CalX
