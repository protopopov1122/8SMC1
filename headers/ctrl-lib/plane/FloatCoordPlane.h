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

#ifndef CALX_CTRL_LIB_PLANE_FLOAT_COORD_PLANE_H_
#define CALX_CTRL_LIB_PLANE_FLOAT_COORD_PLANE_H_

#include "ctrl-lib/plane/ProxyCoordPlane.h"

/* Adds floating point coordinate support to coordinate plane by
   implementing according methods for all elementar coordinate actions */

namespace CalX {

	class FloatCoordPlane : public ProxyCoordPlane {
	 public:
		FloatCoordPlane(coord_point_t, coord_scale_t, double,
		                std::shared_ptr<CoordPlane>);

		coord_point_t getOffset();
		coord_scale_t getScale();
		double getSpeedScale();
		void setBase(std::shared_ptr<CoordPlane>);
		void setOffset(coord_point_t);
		void setScale(coord_scale_t);
		void setSpeedScale(double);

		// Proxy methods
		virtual ErrorCode move(motor_point_t, float, bool);
		virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		                      float = 1.0f);
		virtual motor_point_t getPosition();
		virtual motor_rect_t getSize();
		virtual void dump(std::ostream &);
		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>);
		// Floating-point methods
		virtual ErrorCode move(coord_point_t, double, bool);
		virtual ErrorCode arc(coord_point_t, coord_point_t, int, double, bool,
		                      float = 1.0f);
		virtual ErrorCode relativeMove(coord_point_t, float, bool);
		virtual ErrorCode relativeArc(coord_point_t, coord_point_t, int, float,
		                              bool, float = 1.0f);
		virtual coord_point_t getFloatPosition();
		virtual coord_rect_t getFloatSize();

	 private:
		coord_point_t offset;
		coord_scale_t scale;
		double speed;
	};
}  // namespace CalX

#endif
