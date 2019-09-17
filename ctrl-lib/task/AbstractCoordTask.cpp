/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/task/AbstractCoordTask.h"
#include "calx/ctrl-lib/plane/CoordPlane.h"

namespace CalX {

	class BarrierVirtualPlane : public VirtualCoordPlane {
	 public:
		BarrierVirtualPlane(motor_point_t offset, motor_rect_t size)
		    : VirtualCoordPlane::VirtualCoordPlane(offset, size) {}

		bool jump(motor_point_t point, bool sync) override {
			if (sync) {
				return false;
			} else {
				this->res = point;
				return true;
			}
		}

		std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane> pl) override {
			return std::make_unique<BarrierVirtualPlane>(pl->getPosition(),
			                                             pl->getSize());
		}

		std::optional<motor_point_t> getResult() {
			return this->res;
		}

	 private:
		std::optional<motor_point_t> res;
	};

	std::optional<motor_point_t> CoordTask::getStartPoint(motor_point_t offset,
	                                                      motor_rect_t size,
	                                                      SystemManager &sysman) {
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