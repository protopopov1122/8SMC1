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

#ifndef CALX_CTRL_LIB_PLANE_VIRTUAL_COORD_PLANE_H_
#define CALX_CTRL_LIB_PLANE_VIRTUAL_COORD_PLANE_H_

#include "ctrl-lib/plane/AbstractCoordPlane.h"

namespace CalX {

	class VirtualCoordPlane : public CoordPlane {
	 public:
		VirtualCoordPlane(motor_point_t, motor_rect_t);

		virtual ErrorCode move(motor_point_t, float, bool);
		virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		                      float = 1.0f);
		virtual ErrorCode calibrate(TrailerId);
		virtual ErrorCode measure(TrailerId);
		virtual motor_point_t getPosition();
		virtual motor_rect_t getSize();
		virtual bool isMeasured();
		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>) = 0;
		virtual CoordPlaneStatus getStatus();
		virtual ErrorCode open_session();
		virtual ErrorCode close_session();
		virtual bool isSessionOpened();

		virtual void use();
		virtual void unuse();
		virtual void stop();
		virtual std::ostream &operator<<(std::ostream &);

	 protected:
		virtual bool jump(motor_point_t, bool) = 0;

	 private:
		motor_point_t position;
		motor_rect_t size;
	};
}  // namespace CalX

#endif
