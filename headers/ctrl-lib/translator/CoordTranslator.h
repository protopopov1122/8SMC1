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


#ifndef CTRL_LIB_COORDTRANSLATOR_H_
#define CTRL_LIB_COORDTRANSLATOR_H_


/* Coordinate translator represents different type coornidate systems.
   Each system is mapped to another or to real coordinates.
   These systems are implemented:
	* Decart - named linear
	* Logarithmic
	* Polar
   To use one of them you should pass BasicCoordTranslator to it.
   NOTE: You can also stack them, however you are responsible
   to free memory after them. */

#include "ctrl-lib/translator/AbstractCoordTranslator.h"
#include "ctrl-lib/translator/BasicCoordTranslator.h"
#include "ctrl-lib/translator/LinearCoordTranslator.h"
#include "ctrl-lib/translator/LogarithmicCoordTranslator.h"
#include "ctrl-lib/translator/PolarCoordTranslator.h"
#include "ctrl-lib/translator/ComplexCoordTranslator.h"

#endif
