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
	
	CoordPlaneStack::CoordPlaneStack(CoordPlane *root) {
		this->stack.push_back(root);
	}
	
	CoordPlaneStack::~CoordPlaneStack() {
		for (const auto& plane : this->stack) {
			delete plane;
		}
	}
	
	bool CoordPlaneStack::popPlane() {
		if (this->stack.size() > 1) {
			CoordPlane *plane = this->peekPlane();
			this->stack.pop_back();
			delete plane;
			return true;
		} else {
			return false;
		}
	}
	
	void CoordPlaneStack::pushPlane(CoordPlane *plane) {
		this->stack.push_back(plane);
	}
	
	CoordPlane *CoordPlaneStack::peekPlane() {
		return this->stack.at(this->stack.size() - 1);
	}
	
	ErrorCode CoordPlaneStack::move(motor_point_t dest, float speed, int div, bool sync) {
		return this->peekPlane()->move(dest, speed, div, sync);
	}
	
	ErrorCode CoordPlaneStack::arc(motor_point_t dest, motor_point_t center, int splitter, float speed, int div, bool clockwise, bool strict) {
		return this->peekPlane()->arc(dest, center, splitter, speed, div, clockwise, strict);
	}
	
	ErrorCode CoordPlaneStack::calibrate(TrailerId tr) {
		return this->peekPlane()->calibrate(tr);
	}
	
	ErrorCode CoordPlaneStack::measure(TrailerId tr) {
		return this->peekPlane()->measure(tr);
	}
	
	motor_point_t CoordPlaneStack::getPosition() {
			return this->peekPlane()->getPosition();
	}
	
	motor_rect_t CoordPlaneStack::getSize() {
		return this->peekPlane()->getSize();
	}
	
	void CoordPlaneStack::dump(std::ostream &os) {
		for (size_t i = this->stack.size() - 1; i < this->stack.size(); i--) {
			CoordPlane *plane = this->stack.at(i);
			os << "\t";
			plane->dump(os);
			os << std::endl;
		}
	}
	
	void CoordPlaneStack::use() {
		this->peekPlane()->use();
	}
	
	void CoordPlaneStack::unuse() {
		this->peekPlane()->unuse();
	}
	
	void CoordPlaneStack::stop() {
		this->peekPlane()->stop();
	}
}