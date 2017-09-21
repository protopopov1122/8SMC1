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

#ifndef CALX_CTRL_LIB_COORD_PLANE_H_
#define CALX_CTRL_LIB_COORD_PLANE_H_

/* Coordinate plane is abstraction that represents real motors.
   However most of implementations are just filters that perform
   some actions on coordinates. Only CoordController represents real
   motor system. You should stack filters on top of this controller
   and use top filter(there available virtual coordinate plane,
   that is plane emulator but it is not tested). To simplify
   stacking you may use CoordPlaneStack, it also takes responsibility
   to free filter memory, otherwise you are responsible to free memroy. */

#include "ctrl-lib/plane/AbstractCoordPlane.h"
#include "ctrl-lib/plane/CoordController.h"
#include "ctrl-lib/plane/CoordPlaneLinearizer.h"
#include "ctrl-lib/plane/CoordPlaneLog.h"
#include "ctrl-lib/plane/CoordPlaneMap.h"
#include "ctrl-lib/plane/CoordPlaneStack.h"
#include "ctrl-lib/plane/CoordPlaneValidator.h"
#include "ctrl-lib/plane/FloatCoordPlane.h"
#include "ctrl-lib/plane/VirtualCoordPlane.h"

#endif
