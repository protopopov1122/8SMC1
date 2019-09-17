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

#ifndef CALX_CTRL_LIB_CIRCLE_GENERATOR_H_
#define CALX_CTRL_LIB_CIRCLE_GENERATOR_H_

#include "calx/ctrl-lib/ctrl/MotorController.h"
#include <cinttypes>
#include <vector>

/* This file contains Bresenham algorithm implementation for circles.
   It is incremental, that means it stores current position and parameters
   and can calculate next or previouse basing on these values.
   This algorithm is used only by arc build so you won't use normally.*/

namespace CalX {

	static constexpr int COMPARISON_RADIUS = 10;

	class Circle {
	 public:
		Circle(motor_point_t, int64_t, bool, float);
		int64_t getRadius() const;
		motor_point_t getCenter() const;
		bool skip(motor_point_t, bool *);
		motor_point_t next();

	 private:
		motor_point_t getNextElement();
		motor_point_t getPrevElement();

		motor_point_t center;
		int64_t radius;
		float scale;

		int64_t offset_x;
		int64_t offset_y;
		int64_t criterion;

		int sector;
		bool clockwise;
	};
}  // namespace CalX

#endif
