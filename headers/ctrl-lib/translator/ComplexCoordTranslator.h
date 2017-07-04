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


#ifndef CALX_CTRL_LIB_TRANSLATOR_COMPLEX_COORD_TRANSLATOR_H_
#define CALX_CTRL_LIB_TRANSLATOR_COMPLEX_COORD_TRANSLATOR_H_

#include "ctrl-lib/translator/AbstractCoordTranslator.h"
#include <vector>

namespace CalX {

	class ComplexCoordTranslator : public CoordTranslator {
		public:
			ComplexCoordTranslator(CoordTranslator*);
			virtual ~ComplexCoordTranslator();
			virtual motor_point_t get(double, double);
			virtual coord_point_t get(motor_point_t);
			virtual coord_point_t floatGet(double, double);
			virtual coord_point_t floatGet(coord_point_t);
			size_t getSize();
                        CoordTranslator* getTranslator(size_t);
			void add(CoordTranslator*);
			bool remove(size_t);
			bool insert(size_t, CoordTranslator*);
			virtual CoordTranslator *getBase();
			virtual void setBase(CoordTranslator*);
			virtual ComplexCoordTranslator *clone(CoordTranslator*);
		private:
			std::vector<CoordTranslator*> list;
	};
}

#endif