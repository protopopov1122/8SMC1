/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "ctrl-lib/misc/CircleGenerator.h"
#include <cmath>

namespace CalX {

	Circle::Circle(motor_point_t center, int64_t radius, bool clockwise,
	               float scale) {
		this->center = center;
		this->radius = radius;
		this->scale = scale;
		this->offset_x = radius;
		this->offset_y = 0;
		this->criterion = 0;
		this->sector = 0;
		this->clockwise = clockwise;
	}

	motor_point_t Circle::getCenter() const {
		return this->center;
	}

	int64_t Circle::getRadius() const {
		return this->radius;
	}

	motor_point_t Circle::next() {
		if (!this->clockwise) {
			return this->getNextElement();
		} else {
			return this->getPrevElement();
		}
	}

	bool Circle::skip(motor_point_t pnt, bool *work) {
		if (pnt.x == offset_x && pnt.y == offset_y) {
			return false;
		}
		int64_t r1 = (pnt.x - center.x) * (pnt.x - center.x) +
		             (pnt.y - center.y) * (pnt.y - center.y);
		if ((int64_t) sqrt(r1) != radius) {
			return false;
		}
		motor_point_t start = { offset_x, offset_y };
		motor_point_t cur = start;

		do {
			if (work != nullptr && !*work) {
				return true;
			}
			cur = this->next();
			if (cur.x == start.x && cur.y == start.y) {
				break;
			}
		} while (abs(cur.x - pnt.x) / this->scale > COMPARISON_RADIUS ||
		         abs(cur.y - pnt.y) / this->scale > COMPARISON_RADIUS);
		if (cur.x == start.x && cur.y == start.y) {
			return false;
		}
		return true;
	}

	motor_point_t Circle::getNextElement() {
		if (this->criterion <= 0) {
			this->offset_y += 1;
			this->criterion += 2 * offset_y + 1;
		} else {
			this->offset_x -= 1;
			this->criterion -= 2 * offset_x + 1;
		}
		motor_point_t point = center;
		switch (this->sector) {
			case 0:
				point.x += this->offset_x;
				point.y += this->offset_y;
				break;
			case 1:
				point.x -= this->offset_y;
				point.y += this->offset_x;
				break;
			case 2:
				point.x -= this->offset_x;
				point.y -= this->offset_y;
				break;
			case 3:
				point.x += this->offset_y;
				point.y -= this->offset_x;
				break;
		}
		if (this->offset_x == 0 || this->offset_y == 0) {
			this->sector++;
			this->sector %= 4;
			this->offset_x = radius;
			this->offset_y = 0;
			this->criterion = 0;
		}
		return point;
	}

	motor_point_t Circle::getPrevElement() {
		if (criterion <= 0) {
			this->offset_y += 1;
			this->criterion += 2 * offset_y + 1;
		} else {
			this->offset_x -= 1;
			this->criterion -= 2 * offset_x + 1;
		}
		motor_point_t point = center;
		switch (this->sector) {
			case 0:
				point.x += this->offset_x;
				point.y -= this->offset_y;
				break;
			case 1:
				point.x -= this->offset_y;
				point.y -= this->offset_x;
				break;
			case 2:
				point.x -= this->offset_x;
				point.y += this->offset_y;
				break;
			case 3:
				point.x += this->offset_y;
				point.y += this->offset_x;
				break;
		}
		if (this->offset_x == 0 || this->offset_y == 0) {
			this->sector++;
			this->sector %= 4;
			this->offset_x = radius;
			this->offset_y = 0;
			this->criterion = 0;
		}
		return point;
	}
}  // namespace CalX
