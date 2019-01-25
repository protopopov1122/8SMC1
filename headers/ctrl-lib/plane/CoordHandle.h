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

#ifndef CALX_CTRL_LIB_PLANE_COORD_HANDLE_H_
#define CALX_CTRL_LIB_PLANE_COORD_HANDLE_H_

#include "ctrl-lib/plane/CoordPlane.h"
#include <cinttypes>

namespace CalX {

	class CoordHandle
	    : public CoordPlaneStack,
	      public AbstractEventSource<std::shared_ptr<CoordEventListener>> {
	 public:
		CoordHandle(std::size_t, std::shared_ptr<CoordController>);

		std::size_t getID() const;
		std::shared_ptr<CoordController> getController() const;
		std::shared_ptr<FloatCoordPlane> getFloatPlane() const;

		virtual bool popPlane();
		virtual void pushPlane(std::shared_ptr<CoordPlane>);

		void addEventListener(std::shared_ptr<CoordEventListener>) override;
		void removeEventListener(std::shared_ptr<CoordEventListener>) override;

	 private:
		std::size_t id;
		std::shared_ptr<CoordController> root;
		std::shared_ptr<FloatCoordPlane> floatPlane;
	};
}  // namespace CalX

#endif
