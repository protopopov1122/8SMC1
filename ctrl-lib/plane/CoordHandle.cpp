/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/plane/CoordHandle.h"

namespace CalX {

	CoordHandle::CoordHandle(std::size_t id,
	                         std::shared_ptr<CoordController> root)
	    : CoordPlaneStack::CoordPlaneStack(root) {
		this->id = id;
		this->root = root;
		coord_point_t offset = { 0.0, 0.0 };
		coord_scale_t scale = { 1.0, 1.0 };
		this->floatPlane =
		    std::make_shared<FloatCoordPlane>(offset, scale, 1.0, root);
	}

	std::size_t CoordHandle::getID() const {
		return this->id;
	}

	std::shared_ptr<CoordController> CoordHandle::getController() const {
		return this->root;
	}

	std::shared_ptr<FloatCoordPlane> CoordHandle::getFloatPlane() const {
		return this->floatPlane;
	}

	bool CoordHandle::popPlane() {
		if (CoordPlaneStack::popPlane()) {
			this->floatPlane->setBase(this->peekPlane());
			return true;
		} else {
			return false;
		}
	}

	void CoordHandle::pushPlane(std::shared_ptr<CoordPlane> pl) {
		CoordPlaneStack::pushPlane(pl);
		this->floatPlane->setBase(this->peekPlane());
	}

	void CoordHandle::addEventListener(std::shared_ptr<CoordEventListener> l) {
		this->root->addEventListener(l);
	}

	void CoordHandle::removeEventListener(std::shared_ptr<CoordEventListener> l) {
		this->root->removeEventListener(l);
	}
}  // namespace CalX
