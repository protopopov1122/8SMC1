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

#include "ctrl-lib/plane/CoordPlane.h"

namespace CalX {

	ErrorCode CoordPlane::relativeMove(motor_point_t relpoint, float speed,
	                                   bool sync) {
		motor_point_t point = getPosition();
		point.x += relpoint.x;
		point.y += relpoint.y;
		return move(point, speed, sync);
	}

	ErrorCode CoordPlane::relativeArc(motor_point_t reldest,
	                                  motor_point_t relcenter, int splitter,
	                                  float speed, bool clockwise, float scale) {
		motor_point_t dest = getPosition();
		motor_point_t center = getPosition();
		dest.x += reldest.x;
		dest.y += reldest.y;
		center.x += relcenter.x;
		center.y += relcenter.y;
		return arc(dest, center, splitter, speed, clockwise, scale);
	}

	ErrorCode CoordPlane::open_session() {
		return ErrorCode::NoError;
	}

	ErrorCode CoordPlane::close_session() {
		return ErrorCode::NoError;
	}

	void TaskState::stop() {
		plane->stop();
		work = false;
	}
}  // namespace CalX
