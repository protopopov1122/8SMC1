/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
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
#include <iostream>
#include <string>

namespace CalX {

	enum CoordType {
		BasicCoord, LinearCoord, LogarithmicCoord, PolarCoord, ComplexCoord
	};

	class CoordTranslator {
		public:
			CoordTranslator(CoordType t) {
				this->type = t;
			}
			virtual ~CoordTranslator() {
			}
			CoordType getType() {return this->type;}
			virtual motor_point_t get(coord_point_t pnt) {return this->get(pnt.x, pnt.y);}
			virtual motor_point_t get(double, double) = 0;
			virtual coord_point_t get(motor_point_t) = 0;
			virtual coord_point_t floatGet(double, double) = 0;
			virtual coord_point_t floatGet(coord_point_t) = 0;
			virtual CoordTranslator *getBase() {return nullptr;}
			virtual void setBase(CoordTranslator *b) {}
			virtual CoordTranslator *clone(CoordTranslator*) = 0;
		private:
			CoordType type;
	};
}

#endif
