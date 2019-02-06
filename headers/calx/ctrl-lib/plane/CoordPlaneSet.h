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

#ifndef CALX_CTRL_LIB_PLACE_COORD_PLANE_SET_H_
#define CALX_CTRL_LIB_PLACE_COORD_PLANE_SET_H_

#include "calx/ctrl-lib/plane/CoordHandle.h"
#include <vector>

namespace CalX {

	class CoordPlaneSet {
	 public:
		virtual ~CoordPlaneSet() = default;
		virtual std::size_t getCoordCount() const = 0;
		virtual std::weak_ptr<CoordHandle> getCoord(std::size_t) const = 0;
		virtual std::weak_ptr<CoordHandle> createCoord(
		    std::shared_ptr<MotorController>, std::shared_ptr<MotorController>,
		    std::shared_ptr<InstrumentController> = nullptr) = 0;
		virtual bool removeCoord(std::size_t) = 0;
	};

	class CoordPlaneSetListener {
	 public:
		virtual ~CoordPlaneSetListener() = default;
		virtual void onPlaneAdded(std::shared_ptr<CoordHandle>) = 0;
		virtual void onPlaneRemoving(std::size_t) = 0;
	};

	class VectorCoordPlaneSet : public CoordPlaneSet {
	 public:
		VectorCoordPlaneSet(ConfigurationCatalogue &,
		                    CoordPlaneSetListener * = nullptr);
		std::size_t getCoordCount() const override;
		std::weak_ptr<CoordHandle> getCoord(std::size_t) const override;
		std::weak_ptr<CoordHandle> createCoord(
		    std::shared_ptr<MotorController>, std::shared_ptr<MotorController>,
		    std::shared_ptr<InstrumentController> = nullptr) override;
		bool removeCoord(std::size_t) override;

	 private:
		ConfigurationCatalogue &config;
		CoordPlaneSetListener *listener;
		std::vector<std::shared_ptr<CoordHandle>> planes;
	};
}  // namespace CalX

#endif