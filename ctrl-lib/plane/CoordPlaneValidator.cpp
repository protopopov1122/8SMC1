#include "CoordPlane.h"

namespace CalX {
	
	CoordPlaneValidator::CoordPlaneValidator(motor_point_t min, motor_point_t max, float maxspeed, CoordPlane *cplane) {
		this->plane = cplane;
		this->min = min;
		this->max = max;
		this->max_speed = maxspeed;
	}
	
	CoordPlaneValidator::~CoordPlaneValidator() {
		
	}
	
	CoordPlane *CoordPlaneValidator::getBase() {
		return this->plane;
	}
	
	motor_point_t CoordPlaneValidator::getMinimum() {
		return this->min;
	}
	
	motor_point_t CoordPlaneValidator::getMaximum() {
		return this->max;
	}
	
	float CoordPlaneValidator::getMaxSpeed() {
		return this->max_speed;
	}
	
	void CoordPlaneValidator::setMinimum(motor_point_t p) {
		this->min = p;
	}
	
	void CoordPlaneValidator::setMaximum(motor_point_t p) {
		this->max = p;
	}
	
	void CoordPlaneValidator::setMaxSpeed(float s) {
		this->max_speed = s;
	}
	
	ErrorCode CoordPlaneValidator::move(motor_point_t dest, float speed, int div, bool sync) {
		if (speed < 0 || speed > this->max_speed) {
			return ErrorCode::InvalidSpeed;
		}
		if (dest.x < this->min.x ||
			dest.y < this->min.y ||
			dest.x >= this->max.x ||
			dest.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;	
		}
		return this->plane->move(dest, speed, div, sync);
	}
	
	ErrorCode CoordPlaneValidator::arc(motor_point_t dest, motor_point_t center, int splitter, float speed, int div, bool clockwise, bool strict) {
		if (speed < 0 || speed > this->max_speed) {
			return ErrorCode::InvalidSpeed;
		}
		if (dest.x < this->min.x ||
			dest.y < this->min.y ||
			dest.x >= this->max.x ||
			dest.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;	
		}
		if (center.x < this->min.x ||
			center.y < this->min.y ||
			center.x >= this->max.x ||
			center.y >= this->max.y) {
			return ErrorCode::InvalidCoordinates;	
		}
		return this->plane->arc(dest, center, splitter, speed, div, clockwise, strict);
	}
	
	ErrorCode CoordPlaneValidator::calibrate(TrailerId tr) {
		return this->plane->calibrate(tr);
	}
	
	motor_point_t CoordPlaneValidator::getPosition() {
			return this->plane->getPosition();
	}
	
	motor_rect_t CoordPlaneValidator::getSize() {
		return this->plane->getSize();
	}

	void CoordPlaneValidator::dump(std::ostream &os) {
		os << "validator(min=" << this->min.x << "x" << this->min.y
			<< "; max=" << this->max.x << "x" << this->max.y
			<< "; speed=" << this->max_speed << ")";
	}
	
	void CoordPlaneValidator::use() {
		this->plane->use();
	}
	
	void CoordPlaneValidator::unuse() {
		this->plane->unuse();
	}
	
	void CoordPlaneValidator::stop() {
		this->plane->stop();
	}
}