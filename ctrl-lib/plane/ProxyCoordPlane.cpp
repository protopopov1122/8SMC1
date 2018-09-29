/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#include "ctrl-lib/plane/ProxyCoordPlane.h"

namespace CalX {

	ProxyCoordPlane::ProxyCoordPlane(std::shared_ptr<CoordPlane> base)
	    : base(base) {}

	std::shared_ptr<CoordPlane> ProxyCoordPlane::getBase() const {
		return this->base;
	}

	ErrorCode ProxyCoordPlane::move(motor_point_t pos, float speed, bool sync) {
		return this->base->move(pos, speed, sync);
	}

	ErrorCode ProxyCoordPlane::arc(motor_point_t pos, motor_point_t cen, int spl,
	                               float speed, bool clw, float mul) {
		return this->base->arc(pos, cen, spl, speed, clw, mul);
	}

	ErrorCode ProxyCoordPlane::calibrate(TrailerId id) {
		return this->base->calibrate(id);
	}

	ErrorCode ProxyCoordPlane::measure(TrailerId id) {
		return this->base->measure(id);
	}

	motor_point_t ProxyCoordPlane::getPosition() const {
		return this->base->getPosition();
	}

	motor_rect_t ProxyCoordPlane::getSize() const {
		return this->base->getSize();
	}

	bool ProxyCoordPlane::isMeasured() const {
		return this->base->isMeasured();
	}

	bool ProxyCoordPlane::isSessionOpened() const {
		return this->base->isSessionOpened();
	}

	void ProxyCoordPlane::stop() {
		this->base->stop();
	}

	std::ostream &ProxyCoordPlane::operator<<(std::ostream &os) const {
		os << this->base;
		return os;
	}

	std::unique_ptr<CoordPlane> ProxyCoordPlane::clone(
	    std::shared_ptr<CoordPlane> base) {
		return std::make_unique<ProxyCoordPlane>(this->base->clone(base));
	}

	CoordPlaneStatus ProxyCoordPlane::getStatus() const {
		return this->base->getStatus();
	}

	ErrorCode ProxyCoordPlane::open_session() {
		return this->base->open_session();
	}

	ErrorCode ProxyCoordPlane::close_session() {
		return this->base->close_session();
	}

	void ProxyCoordPlane::use() {
		this->base->use();
	}

	void ProxyCoordPlane::unuse() {
		this->base->unuse();
	}
}  // namespace CalX