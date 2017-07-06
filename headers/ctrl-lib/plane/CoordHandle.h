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

#ifndef CALX_CTRL_LIB_PLANE_COORD_HANDLE_H_
#define CALX_CTRL_LIB_PLANE_COORD_HANDLE_H_

#include <cinttypes>
#include "ctrl-lib/plane/CoordPlane.h"

namespace CalX {

  class CoordHandle : public CoordPlaneStack {
   public:
	CoordHandle(size_t, CoordController *);
	virtual ~CoordHandle();

	size_t getID();
	CoordController *getController();
	FloatCoordPlane *getFloatPlane();

	virtual bool popPlane();
	virtual void pushPlane(CoordPlane *);

	void addEventListener(CoordEventListener *);
	void removeEventListener(CoordEventListener *);

   private:
	size_t id;
	CoordController *root;
	FloatCoordPlane *floatPlane;
  };
}

#endif
