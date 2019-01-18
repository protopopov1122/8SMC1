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

#ifndef CALX_CTRL_LIB_CTRL_CORE_H_
#define CALX_CTRL_LIB_CTRL_CORE_H_

#include "ctrl-lib/logger.h"
#include "platform.h"
#include "ctrl-lib/ConfigKeys.h"
#include <cinttypes>
#include <iosfwd>

/* This file contains basic structures, enums and constants that commonly used
 * by other library parts. */

namespace CalX {
	typedef int64_t motor_coord_t;

	/* Ctrl-lib possible errors. It doesn't include all Device API low-level
	 * errors.*/
	enum class ErrorCode {
		NoError = 0,
		PowerOff = 1,
		LowLevelError = 2,
		UnknownResource = 3,
		OperationUnavailable = 4,
		Interrupted = 5,
		InternalInterrupted = 6,
		WrongParameter = 7,
		Trailer1Pressed = 8,
		Trailer2Pressed = 9,
		MotorRunning = 10,
		MotorStopped = 11,
		ArcError = 12,
		InvalidCoordinates = 13,
		InvalidSpeed = 14,
		MathExprError = 15,
		MathRuntimeNoVar = 16,
		MathRuntimeNoFunction = 17,
		MathRuntimeWrongPrms = 18
	};

	std::ostream &operator<<(std::ostream &, const ErrorCode &);

	/* Used in device controller. Describes current move type and state.*/
	enum class MoveType {
		Stop = 0,
		MoveDown = -1,
		MoveUp = 1,
		RollDown = -2,
		RollUp = 2
	};

	/* Different structures that describe motor positions, position changes and
	 * state.*/
	struct motor_point_t {
		motor_point_t(motor_coord_t x, motor_coord_t y) : x(x), y(y) {}
		motor_point_t(const motor_point_t &p) : x(p.x), y(p.y) {}
		motor_point_t() : x(0), y(0) {}
		motor_point_t &operator=(const motor_point_t &p) {
			this->x = p.x;
			this->y = p.y;
			return *this;
		}
		motor_coord_t x;
		motor_coord_t y;
	};

	struct motor_size_t {
		motor_size_t(motor_coord_t w, motor_coord_t h) : w(w), h(h) {}
		motor_size_t(const motor_size_t &p) : w(p.w), h(p.h) {}
		motor_size_t() : w(0), h(0) {}
		motor_size_t &operator=(const motor_size_t &s) {
			this->w = s.w;
			this->h = s.h;
			return *this;
		}
		motor_coord_t w;
		motor_coord_t h;
	};

	struct motor_rect_t : public motor_point_t, public motor_size_t {
		motor_rect_t(motor_coord_t x, motor_coord_t y, motor_coord_t w,
		             motor_coord_t h)
		    : motor_point_t(x, y), motor_size_t(w, h) {}
		motor_rect_t(motor_point_t p, motor_size_t s)
		    : motor_point_t(p), motor_size_t(s) {}
		motor_rect_t(const motor_rect_t &r)
		    : motor_point_t(r.x, r.y), motor_size_t(r.w, r.h) {}
		motor_rect_t() : motor_point_t(), motor_size_t() {}
		motor_rect_t &operator=(const motor_rect_t &r) {
			this->x = r.x;
			this->y = r.y;
			this->w = r.w;
			this->h = r.h;
			return *this;
		}
	};

	enum class TrailerId { Trailer1 = 1, Trailer2 = 2 };

	/* Different structures that describe coordinate plane points and scales.*/
	struct coord_point_t {
		coord_point_t(double x, double y) : x(x), y(y) {}
		coord_point_t(const coord_point_t &p) : x(p.x), y(p.y) {}
		coord_point_t() : x(0.0), y(0.0) {}
		coord_point_t &operator=(const coord_point_t &c) {
			this->x = c.x;
			this->y = c.y;
			return *this;
		}
		double x;
		double y;
	};

	typedef coord_point_t motor_scale_t;
	typedef coord_point_t coord_scale_t;

	struct coord_size_t {
		coord_size_t(double w, double h) : w(w), h(h) {}
		coord_size_t(const coord_size_t &s) : w(s.w), h(s.h) {}
		coord_size_t() : w(0.0), h(0.0) {}
		coord_size_t &operator=(const coord_size_t &s) {
			this->w = s.w;
			this->h = s.h;
			return *this;
		}
		double w;
		double h;
	};

	struct coord_rect_t : public coord_point_t, public coord_size_t {
		coord_rect_t(double x, double y, double w, double h)
		    : coord_point_t(x, y), coord_size_t(w, h) {}
		coord_rect_t(coord_point_t p, coord_size_t s)
		    : coord_point_t(p), coord_size_t(s) {}
		coord_rect_t(const coord_rect_t &r)
		    : coord_point_t(r.x, r.y), coord_size_t(r.w, r.h) {}
		coord_rect_t() : coord_point_t(), coord_size_t() {}
		coord_rect_t &operator=(const coord_rect_t &r) {
			this->x = r.x;
			this->y = r.y;
			this->w = r.w;
			this->h = r.h;
			return *this;
		}
	};
}  // namespace CalX

#endif