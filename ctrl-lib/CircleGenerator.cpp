#include "CircleGenerator.h"
#include <iostream>

namespace _8SMC1 {

	Circle::Circle(motor_point_t cen, int rad) {
		this->center = cen;
		this->radius = rad;
		this->calculate();
	}

	Circle::~Circle() {

	}

	motor_point_t Circle::getCenter() {
		return this->center;
	}

	int Circle::getRadius() {
		return this->radius;
	}

	size_t Circle::getOctantSize() {
		return this->octant.size();
	}

	size_t Circle::getFullSize() {
		return 8 * this->octant.size();
	}

	motor_point_t Circle::getOctantElement(size_t i) {
		if (i >= this->octant.size()) {
			motor_point_t point = {0, 0};
			return point;
		}
		return this->octant.at(i);
	}
	
	motor_point_t Circle::getElement(size_t i) {
		if (i >= this->octant.size() * 8) {
			i %= this->octant.size() * 8;
		}
		motor_point_t point = {0, 0};
		size_t index = i % this->octant.size();
		switch (i / this->octant.size()) {
			case 0:
				point = this->octant.at(index);
				break;
			case 1: {
				motor_point_t pnt = this->octant.at(this->octant.size() - index - 1);
				point = {pnt.y, pnt.x};
			}
				break;
			case 2: {
				motor_point_t pnt = this->octant.at(index);
				point = {-pnt.y, pnt.x};
			}
				break;
			case 3: {
				motor_point_t pnt = this->octant.at(this->octant.size() - index - 1);
				point = {-pnt.x, pnt.y};
			}
				break;
			case 4: {
				motor_point_t pnt = this->octant.at(index);
				point = {-pnt.x, -pnt.y};
			}
				break;
			case 5: {
				motor_point_t pnt = this->octant.at(this->octant.size() - index - 1);
				point = {-pnt.y, -pnt.x};
			}
				break;
			case 6: {
				motor_point_t pnt = this->octant.at(index);
				point = {pnt.y, -pnt.x};
			}
				break;
			case 7: {
				motor_point_t pnt = this->octant.at(this->octant.size() - index - 1);
				point = {pnt.x, -pnt.y};
			}
				break;
			default:
				break;
		}

		point.x += center.x;
		point.y += center.y;

		return point;
	}
	
	void Circle::calculate() {
		int x = this->radius;
		int y = 0;
		int err = 0;
		while (x >= y) {
			motor_point_t point = {x, y};
			this->octant.push_back(point);
			if (err <= 0)
		        {
		            y += 1;
		            err += 2*y + 1;
		        }
		        if (err > 0)
		        {
		            x -= 1;
		            err -= 2*x + 1;
        		}
		}
	}
}
