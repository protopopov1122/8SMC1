#ifndef _8SMC1_CTRL_LIB_CIRCLE_GENERATOR_H_
#define _8SMC1_CTRL_LIB_CIRCLE_GENERATOR_H_

#include <vector>
#include <cinttypes>
#include "ctrl-lib/DeviceController.h"

/* This file contains Bresenham algorithm implementation for circles */

namespace _8SMC1 {

#define	COMPARISON_RADIUS 10

	class Circle {
		public:
			Circle(motor_point_t, int64_t, bool);
			virtual ~Circle();
			int getRadius();
			motor_point_t getCenter();
			motor_point_t getNextElement();
			motor_point_t getPrevElement();
			bool skip(motor_point_t);
		private:

			motor_point_t center;
			int64_t radius;

			int64_t curx;
			int64_t cury;
			int64_t curerr;

			int stage;
			bool cw;
	};
}

#endif
