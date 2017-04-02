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


#include "CoordPlane.h"

namespace CalX {
	
	CoordPlaneMap::CoordPlaneMap(motor_point_t offset, motor_scale_t scale, CoordPlane *plane) {
		this->offset = offset;
		this->scale = scale;
		this->plane = plane;
		INIT_LOG("CoordPlaneMap");
	}
	
	CoordPlaneMap::~CoordPlaneMap() {
		DESTROY_LOG("CoordPlaneMap");
	}
	
	CoordPlane *CoordPlaneMap::getBase() {
		return this->plane;
	}
	
	motor_point_t CoordPlaneMap::getOffset() {
		return this->offset;
	}
	
	motor_scale_t CoordPlaneMap::getScale() {
		return this->scale;
	}
	
	void CoordPlaneMap::setOffset(motor_point_t p) {
		this->offset = p;
	}
	
	void CoordPlaneMap::setScale(motor_scale_t s) {
		this->scale = s;
	}
	
	ErrorCode CoordPlaneMap::move(motor_point_t dest, float speed, int div, bool sync) {
		dest.x *= this->scale.x;
		dest.y *= this->scale.y;
		dest.x += this->offset.x;
		dest.y += this->offset.y;
		return this->plane->move(dest, speed, div, sync);
	}
	
	ErrorCode CoordPlaneMap::arc(motor_point_t dest, motor_point_t center, int splitter, float speed, int div, bool clockwise, bool strict) {
		dest.x *= this->scale.x;
		dest.y *= this->scale.y;
		dest.x += this->offset.x;
		dest.y += this->offset.y;
		
		center.x *= this->scale.x;
		center.y *= this->scale.y;
		center.x += this->offset.x;
		center.y += this->offset.y;
		return this->plane->arc(dest, center, splitter, speed, div, clockwise, strict);
	}
	
	ErrorCode CoordPlaneMap::calibrate(TrailerId tr) {
		return this->plane->calibrate(tr);
	}
	
	ErrorCode CoordPlaneMap::measure(TrailerId tr) {
		return this->plane->measure(tr);
	}
	
	motor_point_t CoordPlaneMap::getPosition() {
		motor_point_t pnt = this->plane->getPosition();
		pnt.x -= this->offset.x;
		pnt.y -= this->offset.y;
		pnt.x /= this->scale.x;
		pnt.y /= this->scale.y;
		return pnt;
	}
	
	motor_rect_t CoordPlaneMap::getSize() {
		motor_rect_t sz = this->plane->getSize();
		sz.x -= this->offset.x;
		sz.y -= this->offset.y;
		sz.x /= this->scale.x;
		sz.y /= this->scale.y;
		sz.w /= this->scale.x;
		sz.h /= this->scale.y;
		return sz;
	}

	void CoordPlaneMap::dump(std::ostream &os) {
		os << "map(offset=" << this->offset.x << "x" << this->offset.y
			<< "; scale=" << this->scale.x << "x" << this->scale.y << ")";
	}
	
	void CoordPlaneMap::use() {
		this->plane->use();
	}
	
	void CoordPlaneMap::unuse() {
		this->plane->unuse();
	}
	
	void CoordPlaneMap::stop() {
		this->plane->stop();
	}
	
	CoordPlane *CoordPlaneMap::clone(CoordPlane *base) {
		return new CoordPlaneMap(this->offset, this->scale, base);
	}
}