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
	
	CoordPlaneLog::CoordPlaneLog(CoordPlane *cplane, std::ostream *os, std::string prefix, bool log_act, bool log_err) {
		this->plane = cplane;
		this->out = os;
		this->prefix = prefix;
		this->log_actions = log_act;
		this->log_errors = log_err;
	}
	
	CoordPlaneLog::~CoordPlaneLog() {
		
	}
	
	CoordPlane *CoordPlaneLog::getBase() {
		return this->plane;
	}
	
	std::string CoordPlaneLog::getPrefix() {
		return this->prefix;
	}
	
	bool CoordPlaneLog::isLoggingActions() {
		return this->log_actions;
	}
	
	bool CoordPlaneLog::isLoggingErrors() {
		return this->log_errors;
	}
	
	void CoordPlaneLog::setPrefix(std::string p) {
		this->prefix = p;
	}
	
	void CoordPlaneLog::setLoggingActions(bool l) {
		this->log_actions = l;
	}
	
	void CoordPlaneLog::setLoggingErrors(bool l) {
		this->log_errors = l;
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
			*out << this->prefix << "Calibrating to trailer #" << static_cast<int>(tr) << std::endl;
		}
		ErrorCode err = this->plane->calibrate(tr);
		if (this->log_errors && err != ErrorCode::NoError) {
			*out << this->prefix << "Error occured(" << err << ")" << std::endl;
		}
		return err;
	}
	
	ErrorCode CoordPlaneLog::measure(TrailerId tr) {
		if (this->log_actions) {
			*out << this->prefix << "Measuring to trailer #" << static_cast<int>(tr) << std::endl;
		}
		ErrorCode err = this->plane->measure(tr);
		if (this->log_errors && err != ErrorCode::NoError) {
			*out << this->prefix << "Error occured(" << err << ")" << std::endl;
		}
		return err;
	}
	
	motor_point_t CoordPlaneLog::getPosition() {
			return this->plane->getPosition();
	}
	
	motor_rect_t CoordPlaneLog::getSize() {
		return this->plane->getSize();
	}

	void CoordPlaneLog::dump(std::ostream &os) {
		os << "log(prefix=\"" << this->prefix << "\"; log_actions="
			<< this->log_actions << "; log_errors=" << this->log_errors << ")";
	}
	
	void CoordPlaneLog::use() {
		this->plane->use();
	}
	
	void CoordPlaneLog::unuse() {
		this->plane->unuse();
	}
	
	void CoordPlaneLog::stop() {
		this->plane->stop();
	}
}