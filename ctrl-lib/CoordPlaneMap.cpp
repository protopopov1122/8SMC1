#include "CoordPlane.h"

namespace _8SMC1 {
	
	CoordPlaneMap::CoordPlaneMap(motor_point_t offset, motor_scale_t scale, CoordPlane *plane) {
		this->offset = offset;
		this->scale = scale;
		this->plane = plane;
	}
	
	CoordPlaneMap::~CoordPlaneMap() {
		
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
	
	motor_point_t CoordPlaneMap::getPosition() {
		motor_point_t pnt = this->plane->getPosition();
		pnt.x -= this->offset.x;
		pnt.y -= this->offset.y;
		pnt.x /= this->scale.x;
		pnt.y /= this->scale.y;
		return pnt;
	}

	void CoordPlaneMap::dump(std::ostream &os) {
		os << "map(offset=" << this->offset.x << "x" << this->offset.y
			<< "; scale=" << this->scale.x << "x" << this->scale.y << ")";
	}
}