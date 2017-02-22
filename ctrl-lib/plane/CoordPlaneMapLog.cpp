#include <math.h>
#include "CoordPlane.h"

namespace _8SMC1 {

	CoordPlaneMapLog::CoordPlaneMapLog(motor_scale_t logscale, motor_scale_t scale, CoordPlane *plane) {
		this->logscale = logscale;
		this->scale = scale;
		this->plane = plane;
	}
	
	CoordPlaneMapLog::~CoordPlaneMapLog() {
		
	}
	
	ErrorCode CoordPlaneMapLog::move(motor_point_t dest, float speed, int div, bool sync) {
		if (this->logscale.x > 0) {
			float res = log(((long double) dest.x) * this->scale.x) / log(this->logscale.x);
			dest.x = res;
		}
		if (this->logscale.y > 0) {
			float res = log(((long double) dest.y) * this->scale.y) / log(this->logscale.y);
			dest.y = res;
		}
		return this->plane->move(dest, speed, div, sync);
	}
	
	ErrorCode CoordPlaneMapLog::arc(motor_point_t dest, motor_point_t center, int splitter, float speed, int 		div, bool clockwise, bool strict) {
		if (this->logscale.x > 0) {
			dest.x = log(dest.x) / log(this->logscale.x);
			center.x = log(center.x) / log(this->logscale.x);
		}
		if (this->logscale.y > 0) {
			dest.y = log(dest.y) / log(this->logscale.y);
			center.y = log(center.y) / log(this->logscale.y);
		}
		return this->plane->arc(dest, center, splitter, speed, div, clockwise, strict);
	}
	
	ErrorCode CoordPlaneMapLog::calibrate(TrailerId tr) {
		return this->plane->calibrate(tr);
	}
	
	motor_point_t CoordPlaneMapLog::getPosition() {
		motor_point_t pnt = this->plane->getPosition();
		pnt.x = this->logscale.x > 0 ? pow(this->logscale.x, pnt.x) : pnt.x;
		pnt.y = this->logscale.y > 0 ? pow(this->logscale.y, pnt.y) : pnt.y;
		return pnt;
	}

	void CoordPlaneMapLog::dump(std::ostream &os) {
		os << "logarithmic_map(scale=" << this->logscale.x << "x" << this->logscale.y << ")";
	}
}