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

#ifndef CALX_CTRL_LIB_CIRCLE_GENERATOR_H_
#define CALX_CTRL_LIB_CIRCLE_GENERATOR_H_

#include "ctrl-lib/ctrl/MotorController.h"
#include <cinttypes>
#include <vector>

/* This file contains Bresenham algorithm implementation for circles.
   It is incremental, that means it stores current position and parameters
   and can calculate next or previouse basing on these values.
   This algorithm is used only by arc build so you won't use normally.*/

namespace CalX {

#define COMPARISON_RADIUS 10

	class Circle {
	 public:
		Circle(motor_point_t, int64_t, bool, float);
		int64_t getRadius();
		motor_point_t getCenter();
		motor_point_t getNextElement();
		motor_point_t getPrevElement();
		bool skip(motor_point_t);

	 private:
		motor_point_t center;
		int64_t radius;
		float scale;

		int64_t curx;
		int64_t cury;
		int64_t curerr;

		int stage;
		bool cw;
	};
}

#endif
