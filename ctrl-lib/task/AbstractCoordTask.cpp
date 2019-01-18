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

#include "ctrl-lib/task/AbstractCoordTask.h"
#include "ctrl-lib/plane/CoordPlane.h"

namespace CalX {

	class BarrierVirtualPlane : public VirtualCoordPlane {
	 public:
		BarrierVirtualPlane(motor_point_t offset, motor_rect_t size)
		    : VirtualCoordPlane::VirtualCoordPlane(offset, size) {
			this->actual = false;
			this->res = { 0, 0 };
		}

		virtual bool jump(motor_point_t point, bool sync) {
			if (sync) {
				this->res = point;
				this->actual = true;
				return false;
			} else {
				return true;
			}
		}

		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane> pl) {
			return std::make_unique<BarrierVirtualPlane>(pl->getPosition(),
			                                             pl->getSize());
		}

		std::pair<motor_point_t, bool> getResult() {
			return std::make_pair(this->res, this->actual);
		}

	 private:
		bool actual;
		motor_point_t res;
	};

	std::pair<motor_point_t, bool> CoordTask::getStartPoint(
	    motor_point_t offset, motor_rect_t size, SystemManager *sysman) {
		std::shared_ptr<BarrierVirtualPlane> plane =
		    std::make_shared<BarrierVirtualPlane>(offset, size);
		TaskParameters prms = { 1.0f };
		std::shared_ptr<TaskState> state = std::make_shared<TaskState>();
		state->work = false;
		state->plane = plane;
		this->perform(plane, prms, sysman, state);
		return plane->getResult();
	}
}  // namespace CalX