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