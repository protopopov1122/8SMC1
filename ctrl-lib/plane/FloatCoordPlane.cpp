/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "ctrl-lib/plane/CoordPlane.h"
#include <cmath>

namespace CalX {

	FloatCoordPlane::FloatCoordPlane(coord_point_t offs, coord_scale_t scl, double sp, CoordPlane *bs)
		: offset(offs), scale(scl), speed(sp), plane(bs) {
		INIT_LOG("FloatCoordPlane");
	}

	FloatCoordPlane::~FloatCoordPlane() {
		DESTROY_LOG("FloatCoordPlane");
	}

	CoordPlane *FloatCoordPlane::getBase() {
		return this->plane;
	}

	coord_point_t FloatCoordPlane::getOffset() {
		return this->offset;
	}

	coord_scale_t FloatCoordPlane::getScale() {
		return this->scale;
	}

	double FloatCoordPlane::getSpeedScale() {
		return this->speed;
	}

	void FloatCoordPlane::setBase(CoordPlane *pl) {
		this->plane = pl;
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

	ErrorCode FloatCoordPlane::move(motor_point_t dest, float speed, int div, bool sync) {
		dest.x *= this->scale.x;
		dest.y *= this->scale.y;
		dest.x += this->offset.x;
		dest.y += this->offset.y;
		return this->plane->move(dest, speed * this->speed, div, sync);
	}

	ErrorCode FloatCoordPlane::arc(motor_point_t dest, motor_point_t center, int splitter, float speed, int div, bool clockwise, float scale) {
		dest.x *= this->scale.x;
		dest.y *= this->scale.y;
		dest.x += this->offset.x;
		dest.y += this->offset.y;

		center.x *= this->scale.x;
		center.y *= this->scale.y;
		center.x += this->offset.x;
		center.y += this->offset.y;

		scale *= (this->scale.x + this->scale.y) / 2;
		return this->plane->arc(dest, center, splitter, speed * this->speed, div, clockwise, scale);
	}

	ErrorCode FloatCoordPlane::calibrate(TrailerId tr) {
		return this->plane->calibrate(tr);
	}

	ErrorCode FloatCoordPlane::measure(TrailerId tr) {
		return this->plane->measure(tr);
	}

	motor_point_t FloatCoordPlane::getPosition() {
		motor_point_t pnt = this->plane->getPosition();
		pnt.x -= this->offset.x;
		pnt.y -= this->offset.y;
		pnt.x /= this->scale.x;
		pnt.y /= this->scale.y;
		return pnt;
	}

	motor_rect_t FloatCoordPlane::getSize() {
		motor_rect_t sz = this->plane->getSize();
		sz.x -= this->offset.x;
		sz.y -= this->offset.y;
		sz.x /= this->scale.x;
		sz.y /= this->scale.y;
		sz.w /= this->scale.x;
		sz.h /= this->scale.y;
		return sz;
	}

	bool FloatCoordPlane::isMeasured() {
		return this->plane->isMeasured();
	}

	void FloatCoordPlane::dump(std::ostream &os) {
		os << "float(offset=" << this->offset.x << "x" << this->offset.y
			<< "; scale=" << this->scale.x << "x" << this->scale.y
			<< "; speed=" << this->speed << ")";
	}

	void FloatCoordPlane::use() {
		this->plane->use();
	}

	void FloatCoordPlane::unuse() {
		this->plane->unuse();
	}

	void FloatCoordPlane::stop() {
		this->plane->stop();
	}

	CoordPlane *FloatCoordPlane::clone(CoordPlane *base) {
		return new FloatCoordPlane(this->offset, this->scale, this->speed, base);
	}

	CoordPlaneStatus FloatCoordPlane::getStatus() {
		return this->plane->getStatus();
	}

	ErrorCode FloatCoordPlane::open_session() {
		return this->plane->open_session();
	}

	ErrorCode FloatCoordPlane::close_session() {
		return this->plane->close_session();
	}

	ErrorCode FloatCoordPlane::move(coord_point_t dest, double speed, int div, bool sync) {
		motor_point_t pdest = {
			static_cast<motor_coord_t>(round(dest.x * this->scale.x + this->offset.x)),
			static_cast<motor_coord_t>(round(dest.y * this->scale.y + this->offset.y))
		};
		return this->plane->move(pdest, static_cast<float>(speed * this->speed), div, sync);
	}

	ErrorCode FloatCoordPlane::arc(coord_point_t dest, coord_point_t center, int splitter, double speed, int div, bool clockwise, float scale) {

		motor_point_t pdest = {
			static_cast<motor_coord_t>(round(dest.x * this->scale.x + this->offset.x)),
			static_cast<motor_coord_t>(round(dest.y * this->scale.y + this->offset.y))
		};

		motor_point_t pcen = {
			static_cast<motor_coord_t>(round(center.x * this->scale.x + this->offset.x)),
			static_cast<motor_coord_t>(round(center.y * this->scale.y + this->offset.y))
		};

		return this->plane->arc(pdest, pcen, splitter, static_cast<float>(speed * this->speed), div, clockwise, scale);
	}

	coord_point_t FloatCoordPlane::getFloatPosition() {
		motor_point_t pnt = this->plane->getPosition();
		coord_point_t out = {
			(static_cast<double>(pnt.x) - this->offset.x) / this->scale.x,
			(static_cast<double>(pnt.y) - this->offset.y) / this->scale.y
		};
		return out;
	}

	coord_rect_t FloatCoordPlane::getFloatSize() {
		motor_rect_t sz = this->plane->getSize();
		coord_rect_t out = {
			(static_cast<double>(sz.x) - this->offset.x) / this->scale.x,
			(static_cast<double>(sz.y) - this->offset.y) / this->scale.y,
			static_cast<double>(sz.w) / this->scale.x,
			static_cast<double>(sz.h) / this->scale.y
		};
		return out;
	}

	ErrorCode FloatCoordPlane::relativeMove(coord_point_t relpoint, float speed, int div,
			bool sync) {
		coord_point_t point = getFloatPosition();
		point.x += relpoint.x;
		point.y += relpoint.y;
		return move(point, speed, div, sync);
	}

	ErrorCode FloatCoordPlane::relativeArc(coord_point_t reldest, coord_point_t relcenter, int splitter,
				float speed, int div, bool clockwise, float scale) {
		coord_point_t dest = getFloatPosition();
		coord_point_t center = getFloatPosition();
		dest.x += reldest.x;
		dest.y += reldest.y;
		center.x += relcenter.x;
		center.y += relcenter.y;
		return arc(dest, center, splitter, speed, div, clockwise, scale);
	}
}
