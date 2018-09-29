#ifndef CALX_CTRL_LIB_PLACE_COORD_PLANE_SET_H_
#define CALX_CTRL_LIB_PLACE_COORD_PLANE_SET_H_

#include "ctrl-lib/plane/CoordHandle.h"
#include <vector>

namespace CalX {

	class CoordPlaneSet {
	 public:
		virtual ~CoordPlaneSet() = default;
		virtual std::size_t getCoordCount() const = 0;
		virtual std::weak_ptr<CoordHandle> getCoord(std::size_t) const = 0;
		virtual std::weak_ptr<CoordHandle> createCoord(std::shared_ptr<MotorController>, std::shared_ptr<MotorController>,
			std::shared_ptr<InstrumentController> = nullptr) = 0;
		virtual bool removeCoord(std::size_t) = 0;
	};
	
	
	class CoordPlaneSetListener {
	 public:
		virtual ~CoordPlaneSetListener() = default;
		virtual void planeAdded(std::shared_ptr<CoordHandle>) = 0;
		virtual void planeRemoved(std::size_t) = 0;
	};
	
	class VectorCoordPlaneSet : public CoordPlaneSet {
	 public:
		VectorCoordPlaneSet(ConfigManager &, CoordPlaneSetListener *);
		std::size_t getCoordCount() const override;
		std::weak_ptr<CoordHandle> getCoord(std::size_t) const override;
		std::weak_ptr<CoordHandle> createCoord(
			std::shared_ptr<MotorController>, std::shared_ptr<MotorController>,
			std::shared_ptr<InstrumentController> = nullptr) override;
		bool removeCoord(std::size_t) override;
	 private:
		ConfigManager &config;
		CoordPlaneSetListener *listener;
		std::vector<std::shared_ptr<CoordHandle>> planes;
	};
}

#endif