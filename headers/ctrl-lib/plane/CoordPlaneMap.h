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

#ifndef CALX_CTRL_LIB_PLANE_COORD_PLANE_MAP_H_
#define CALX_CTRL_LIB_PLANE_COORD_PLANE_MAP_H_

#include "ctrl-lib/plane/ProxyCoordPlane.h"

namespace CalX {

	class CoordPlaneMap : public ProxyCoordPlane {
	 public:
		CoordPlaneMap(motor_point_t, motor_scale_t, float,
		              std::shared_ptr<CoordPlane>);
		motor_point_t getOffset() const;
		motor_scale_t getScale() const;
		float getSpeedScale() const;
		void setOffset(motor_point_t);
		void setScale(motor_scale_t);
		void setSpeedScale(float);

		virtual ErrorCode move(motor_point_t, float, bool);
		virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		                      float = 1.0f);
		virtual motor_point_t getPosition() const;
		virtual motor_rect_t getSize() const;
		virtual std::ostream &operator<<(std::ostream &) const;
		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>);

	 private:
		motor_point_t offset;
		motor_scale_t scale;
		float speedScale;
	};
}  // namespace CalX

#endif
