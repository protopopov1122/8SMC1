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

#ifndef CALX_CTRL_LIB_TRANSLATOR_ABSTRACT_COORD_TRANSLATOR_H_
#define CALX_CTRL_LIB_TRANSLATOR_ABSTRACT_COORD_TRANSLATOR_H_

#include "ctrl-lib/CtrlCore.h"
#include <iosfwd>
#include <string>
#include <memory>

/* Coordinate translators are used to map
   floating point coordinates to integer ones(as well as floats).
   They may be stacked or used separately.
   Main purpose is representing coordinate plane
   with floating point coordinates.
*/

namespace CalX {

	enum class CoordType {
		BasicCoord,  // Basic coord translator. Similar to linear, but has no
		             // underlying translators

		LinearCoord,  // Linear translator picks given coordinates, multiples them
		              // by scale and adds offset.
		// Basically, it represents Decart coordinates. After translation
		// coordinates are passed
		// to underlying one(if exists)

		LogarithmicCoord,  // Creates logarithmic coordinates. Translates given
		                   // coordinates to logarithm with
		// given base(if base != 0 otherwise coordinate stay same)

		PolarCoord,  // Creates polar coordinate plane abstraction. Given
		             // coordinates are
		             // translated to angle and radius

		ComplexCoord  // Allows to easily stack coordinate translators and manages
		              // them
	};

	class CoordTranslator {
	 public:
		CoordTranslator(CoordType t) {
			this->type = t;
		}
		virtual ~CoordTranslator() = default;
		CoordType getType() const {
			return this->type;
		}
		virtual motor_point_t get(coord_point_t pnt) const {
			return this->get(pnt.x, pnt.y);
		}
		virtual motor_point_t get(double, double) const = 0;
		virtual coord_point_t get(motor_point_t) const = 0;
		virtual coord_point_t floatGet(double, double) const = 0;
		virtual coord_point_t floatGet(coord_point_t) const = 0;
		virtual std::shared_ptr<CoordTranslator> getBase() const {
			return nullptr;
		}
		virtual void setBase(std::shared_ptr<CoordTranslator>) {}
		virtual std::unique_ptr<CoordTranslator> clone(
		    std::shared_ptr<CoordTranslator>) = 0;

	 private:
		CoordType type;
	};
}  // namespace CalX

#endif
