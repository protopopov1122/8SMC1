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

#ifndef CALX_CTRL_LIB_PLANE_FLOAT_COORD_PLANE_H_
#define CALX_CTRL_LIB_PLANE_FLOAT_COORD_PLANE_H_

#include "calx/ctrl-lib/plane/ProxyCoordPlane.h"

/* Adds floating point coordinate support to coordinate plane by
   implementing according methods for all elementar coordinate actions */

namespace CalX {

	class FloatingCoordPlane : public virtual CoordPlane {
	 public:
		virtual ErrorCode move(coord_point_t, double, bool) = 0;
		virtual ErrorCode arc(coord_point_t, coord_point_t, int, double, bool,
		                      float = 1.0f) = 0;
		virtual ErrorCode relativeMove(coord_point_t, float, bool) = 0;
		virtual ErrorCode relativeArc(coord_point_t, coord_point_t, int, float,
		                              bool, float = 1.0f) = 0;
		virtual coord_point_t getFloatPosition() const = 0;
		virtual coord_rect_t getFloatSize() const = 0;
	};

	class FloatCoordPlane : public ProxyCoordPlane, public FloatingCoordPlane {
	 public:
		FloatCoordPlane(coord_point_t, coord_scale_t, double,
		                std::shared_ptr<CoordPlane>);

		coord_point_t getOffset() const;
		coord_scale_t getScale() const;
		double getSpeedScale() const;
		void setBase(std::shared_ptr<CoordPlane>);
		void setOffset(coord_point_t);
		void setScale(coord_scale_t);
		void setSpeedScale(double);

		// Proxy methods
		ErrorCode move(motor_point_t, float, bool) override;
		ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		              float = 1.0f) override;
		motor_point_t getPosition() const override;
		motor_rect_t getSize() const override;
		std::ostream &operator<<(std::ostream &) const override;
		std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>) override;
		// Floating-point methods
		ErrorCode move(coord_point_t, double, bool) override;
		ErrorCode arc(coord_point_t, coord_point_t, int, double, bool,
		              float = 1.0f) override;
		ErrorCode relativeMove(coord_point_t, float, bool) override;
		ErrorCode relativeArc(coord_point_t, coord_point_t, int, float, bool,
		                      float = 1.0f) override;
		coord_point_t getFloatPosition() const override;
		coord_rect_t getFloatSize() const override;

	 private:
		coord_point_t offset;
		coord_scale_t scale;
		double speed;
	};
}  // namespace CalX

#endif
