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

#include "ctrl-lib/misc/CircleGenerator.h"
#include <iostream>
#include <math.h>
#include <stdio.h>

namespace CalX {

	Circle::Circle(motor_point_t cen, int64_t rad, bool cw, float scale) {
		this->center = cen;
		this->radius = rad;
		this->scale = scale;
		this->curx = rad;
		this->cury = 0;
		this->curerr = 0;
		this->stage = 0;
		this->cw = cw;
	}

	motor_point_t Circle::getCenter() {
		return this->center;
	}

	int64_t Circle::getRadius() {
		return this->radius;
	}

	motor_point_t Circle::getNextElement() {
		motor_point_t point;
		if (stage % 4 == 0) {
			if (curerr <= 0) {
				cury += 1;
				curerr += 2 * cury + 1;
			} else {
				curx -= 1;
				curerr -= 2 * curx + 1;
			}
		} else if (stage % 4 == 1) {
			if (curerr <= 0) {
				cury -= 1;
				curerr += 2 * cury - 1;
			} else {
				curx -= 1;
				curerr += 2 * curx - 1;
			}
		} else if (stage % 4 == 2) {
			if (curerr <= 0) {
				cury -= 1;
				curerr -= 2 * cury - 1;
			} else {
				curx += 1;
				curerr += 2 * curx + 1;
			}
		} else if (stage % 4 == 3) {  // TODO Fix coefs
			if (curerr <= 0) {
				cury += 1;
				curerr -= 2 * cury - 1;
			} else {
				curx += 1;
				curerr -= 2 * curx + 1;
			}
		}
		point.x = curx + center.x;
		point.y = cury + center.y;
		if (curx == 0 || cury == 0) {
			stage++;
			stage %= 4;
		}
		return point;
	}

	motor_point_t Circle::getPrevElement() {
		motor_point_t point;
		// TODO Rewrite code
		if (stage % 4 == 0) {
			if (curerr <= 0) {
				cury += 1;
				curerr += 2 * cury + 1;
			} else {
				curx -= 1;
				curerr -= 2 * curx + 1;
			}
		} else if (stage % 4 == 1) {
			if (curerr <= 0) {
				cury -= 1;
				curerr += 2 * cury - 1;
			} else {
				curx -= 1;
				curerr += 2 * curx - 1;
			}
		} else if (stage % 4 == 2) {
			if (curerr <= 0) {
				cury -= 1;
				curerr -= 2 * cury - 1;
			} else {
				curx += 1;
				curerr += 2 * curx + 1;
			}
		} else if (stage % 4 == 3) {  // TODO Fix coefs
			if (curerr <= 0) {
				cury += 1;
				curerr -= 2 * cury - 1;
			} else {
				curx += 1;
				curerr -= 2 * curx + 1;
			}
		}
		point.x = curx + center.x;
		point.y = -cury + center.y;
		if (curx == 0 || cury == 0) {
			stage++;
			stage %= 4;
		}
		return point;
	}

	bool Circle::skip(motor_point_t pnt, bool *work) {
		if (pnt.x == curx && pnt.y == cury) {
			return false;
		}
		int64_t r1 = (pnt.x - center.x) * (pnt.x - center.x) +
		             (pnt.y - center.y) * (pnt.y - center.y);
		if ((int64_t) sqrt(r1) != radius) {
			return false;
		}
		motor_point_t start = { curx, cury };
		motor_point_t cur = start;

		do {
			if (work != nullptr && !*work) {
				return true;
			}
			if (!cw) {
				cur = this->getNextElement();
			} else {
				cur = this->getPrevElement();
			}
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
}  // namespace CalX
