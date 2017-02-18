#include "CoordPlane.h"

namespace _8SMC1 {
	
	CoordPlaneLog::CoordPlaneLog(CoordPlane *cplane, std::ostream &os) {
		this->plane = cplane;
		this->out = &os;
	}
	
	CoordPlaneLog::~CoordPlaneLog() {
		
	}
	
	ErrorCode CoordPlaneLog::move(motor_point_t dest, float speed, int div, bool sync) {
		return this->plane->move(dest, speed, div, sync);
	}
	
	ErrorCode CoordPlaneLog::arc(motor_point_t dest, motor_point_t center, int splitter, float speed, int div, bool clockwise, bool strict) {
		return this->plane->arc(dest, center, splitter, speed, div, clockwise, strict);
	}
	
	ErrorCode CoordPlaneLog::calibrate(TrailerId tr) {
		return this->plane->calibrate(tr);
	}
	
	motor_point_t CoordPlaneLog::getPosition() {
			return this->plane->getPosition();
	}
}