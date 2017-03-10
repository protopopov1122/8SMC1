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