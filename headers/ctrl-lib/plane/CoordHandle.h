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

#ifndef CALX_CTRL_LIB_PLANE_COORD_HANDLE_H_
#define CALX_CTRL_LIB_PLANE_COORD_HANDLE_H_

#include "ctrl-lib/plane/CoordPlane.h"
#include <cinttypes>

namespace CalX {

	class CoordHandle : public CoordPlaneStack {
	 public:
		CoordHandle(size_t, std::shared_ptr<CoordController>);

		size_t getID();
		std::shared_ptr<CoordController> getController();
		std::shared_ptr<FloatCoordPlane> getFloatPlane();

		virtual bool popPlane();
		virtual void pushPlane(std::shared_ptr<CoordPlane>);

		void addEventListener(CoordEventListener *);
		void removeEventListener(CoordEventListener *);

	 private:
		size_t id;
		std::shared_ptr<CoordController> root;
		std::shared_ptr<FloatCoordPlane> floatPlane;
	};
}  // namespace CalX

#endif
