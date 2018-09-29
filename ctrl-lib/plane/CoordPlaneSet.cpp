/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#include "ctrl-lib/plane/CoordPlaneSet.h"
#include "ctrl-lib/plane/CoordPlane.h"

namespace CalX {
	
	VectorCoordPlaneSet::VectorCoordPlaneSet(ConfigManager &config, CoordPlaneSetListener *listener)
		: config(config), listener(listener) {}

	std::size_t VectorCoordPlaneSet::getCoordCount() const {
		return this->planes.size();
	}

	std::weak_ptr<CoordHandle> VectorCoordPlaneSet::getCoord(std::size_t index) const {
		if (index >= this->planes.size()) {
			return std::weak_ptr<CoordHandle>();
		}
		return this->planes.at(index);
	}

	std::weak_ptr<CoordHandle> VectorCoordPlaneSet::createCoord(
	                                                        std::shared_ptr<MotorController> motor1,
	                                                        std::shared_ptr<MotorController> motor2,
	                                                        std::shared_ptr<InstrumentController> instrument) {
		if (motor1 == nullptr || motor2 == nullptr) {
			return std::weak_ptr<CoordHandle>();
		}

		std::shared_ptr<CoordController> plane = std::make_shared<CoordController>(
		    this->config, motor1, motor2, instrument);
		std::shared_ptr<CoordHandle> handle =
		    std::make_shared<CoordHandle>(this->planes.size(), plane);
		if (this->config.getEntry("core")->getBool("auto_power_motors",
		                                                        false)) {
			plane->getXAxis()->enablePower(true);
			plane->getYAxis()->enablePower(true);
		}
		this->planes.push_back(handle);
		if (this->listener) {
			this->listener->onPlaneAdded(handle);
		}
		return handle;
	}

	bool VectorCoordPlaneSet::removeCoord(std::size_t index) {
		if (index < this->planes.size()) {
			if (this->listener) {
				this->listener->onPlaneRemoving(index);
			}
			this->planes.erase(this->planes.begin() + (std::ptrdiff_t) index);
			return true;
		} else {
			return false;
		}
	}
}