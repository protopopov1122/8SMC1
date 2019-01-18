/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
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
#include <iostream>

namespace CalX {

	CoordPlaneStack::CoordPlaneStack(std::shared_ptr<CoordPlane> root) {
		this->stack.push_back(root);
	}

	bool CoordPlaneStack::popPlane() {
		if (this->stack.size() > 1) {
			this->stack.pop_back();
			return true;
		} else {
			return false;
		}
	}

	void CoordPlaneStack::pushPlane(std::shared_ptr<CoordPlane> plane) {
		this->stack.push_back(plane);
	}

	std::shared_ptr<CoordPlane> CoordPlaneStack::peekPlane() const {
		return this->stack.at(this->stack.size() - 1);
	}

	std::shared_ptr<CoordPlane> CoordPlaneStack::getBase() const {
		return this->stack.at(0);
	}

	ErrorCode CoordPlaneStack::move(motor_point_t dest, float speed, bool sync) {
		return this->peekPlane()->move(dest, speed, sync);
	}

	ErrorCode CoordPlaneStack::arc(motor_point_t dest, motor_point_t center,
	                               int splitter, float speed, bool clockwise,
	                               float scale) {
		return this->peekPlane()->arc(dest, center, splitter, speed, clockwise,
		                              scale);
	}

	ErrorCode CoordPlaneStack::calibrate(TrailerId tr) {
		return this->peekPlane()->calibrate(tr);
	}

	ErrorCode CoordPlaneStack::measure(TrailerId tr) {
		return this->peekPlane()->measure(tr);
	}

	motor_point_t CoordPlaneStack::getPosition() const {
		return this->peekPlane()->getPosition();
	}

	motor_rect_t CoordPlaneStack::getSize() const {
		return this->peekPlane()->getSize();
	}

	bool CoordPlaneStack::isMeasured() const {
		return this->peekPlane()->isMeasured();
	}

	std::ostream &CoordPlaneStack::operator<<(std::ostream &os) const {
		for (size_t i = this->stack.size() - 1; i < this->stack.size(); i--) {
			std::shared_ptr<CoordPlane> plane = this->stack.at(i);
			os << "\t" << plane << std::endl;
		}
		return os;
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

	std::unique_ptr<CoordPlane> CoordPlaneStack::clone(
	    std::shared_ptr<CoordPlane> plane) {
		std::unique_ptr<CoordPlaneStack> stack =
		    std::make_unique<CoordPlaneStack>(plane);
		for (size_t i = 1; i < this->stack.size(); i++) {
			stack->pushPlane(this->stack.at(i)->clone(stack->peekPlane()));
		}
		return stack;
	}

	CoordPlaneStatus CoordPlaneStack::getStatus() const {
		return this->peekPlane()->getStatus();
	}

	ErrorCode CoordPlaneStack::open_session() {
		return this->peekPlane()->open_session();
	}

	ErrorCode CoordPlaneStack::close_session() {
		return this->peekPlane()->close_session();
	}

	bool CoordPlaneStack::isSessionOpened() const {
		return this->peekPlane()->isSessionOpened();
	}
}  // namespace CalX
