#include "CoordPlane.h"

namespace _8SMC1 {
	
	CoordPlaneLog::CoordPlaneLog(CoordPlane *cplane, std::ostream &os, std::string prefix, bool log_act, bool log_err) {
		this->plane = cplane;
		this->out = &os;
		this->prefix = prefix;
		this->log_actions = log_act;
		this->log_errors = log_err;
	}
	
	CoordPlaneLog::~CoordPlaneLog() {
		
	}
	
	ErrorCode CoordPlaneLog::move(motor_point_t dest, float speed, int div, bool sync) {
		if (this->log_actions) {
			*out << this->prefix << "Linear moving to " << dest.x << "x" << dest.y << " with base speed " << speed
				<< "; step divisor " << div << "; synchrone movement is " << (sync ? "on." : "off.") << std::endl;
		}
		ErrorCode err = this->plane->move(dest, speed, div, sync);
		if (this->log_errors && err != ErrorCode::NoError) {
			*out << this->prefix << "Error occured(" << err << ")" << std::endl;
		}
		return err;
	}
	
	ErrorCode CoordPlaneLog::arc(motor_point_t dest, motor_point_t center, int splitter, float speed, int div, bool clockwise, bool strict) {
		if (this->log_actions) {
			*out << this->prefix << (clockwise ? "Clockwise" : "Counter-clockwise" )
				<< " arc moving to " << dest.x << "x" << dest.y << "; center " << center.x << "x" << center.y << " with base speed " << speed
				<< "; splitter " << splitter << "; step divisor " << div << "." << std::endl;
		}
		ErrorCode err = this->plane->arc(dest, center, splitter, speed, div, clockwise, strict);
		if (this->log_errors && err != ErrorCode::NoError) {
			*out << this->prefix << "Error occured(" << err << ")" << std::endl;
		}
		return err;
	}
	
	ErrorCode CoordPlaneLog::calibrate(TrailerId tr) {
		if (this->log_actions) {
			*out << this->prefix << "Calibrating to trailer#" << tr << std::endl;
		}
		ErrorCode err = this->plane->calibrate(tr);
		if (this->log_errors && err != ErrorCode::NoError) {
			*out << this->prefix << "Error occured(" << err << ")" << std::endl;
		}
		return err;
	}
	
	motor_point_t CoordPlaneLog::getPosition() {
			return this->plane->getPosition();
	}

	void CoordPlaneLog::dump(std::ostream &os) {
		os << "log(prefix=\"" << this->prefix << "\"; log_actions="
			<< this->log_actions << "; log_errors=" << this->log_errors << ")";
	}
}