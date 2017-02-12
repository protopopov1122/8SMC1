#ifndef _8SMC1_CTRL_LIB_CIRCLE_GENERATOR_H_
#define _8SMC1_CTRL_LIB_CIRCLE_GENERATOR_H_

#include <vector>
#include <cinttypes>
#include "DeviceController.h"

/* This file contains Bresenham algorithm implementation for circles */

namespace _8SMC1 {

	class Circle {
		public:
			Circle(motor_point_t, int);
			virtual ~Circle();
			int getRadius();
			motor_point_t getCenter();
			size_t getOctantSize();
			size_t getFullSize();
			motor_point_t getOctantElement(size_t);
			motor_point_t getElement(size_t);
		private:
			void calculate();	// Fill first octant

			motor_point_t center;
			int radius;
			std::vector<motor_point_t> octant;
	};
}

#endif
