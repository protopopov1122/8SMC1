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

#include <cinttypes>
#include <vector>
#include "ctrl-lib/DeviceController.h"

/* Coordinate translator represents different type coornidate systems.
   Each system is mapped to another or to real coordinates.
   These systems are implemented:
	* Decart - named linear
	* Logarithmic
	* Polar 
   To use one of them you should pass BasicCoordTranslator to it.
   NOTE: You can also stack them, however you are responsible
   to free memory after them. */

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
			virtual CoordTranslator *getBase() {return nullptr;}
			virtual void setBase(CoordTranslator *b) {}
			virtual CoordTranslator *clone(CoordTranslator*) = 0;
		private:
			CoordType type;
	};
	
	class BasicCoordTranslator : public CoordTranslator {
		public:
			BasicCoordTranslator(motor_point_t, motor_size_t);
			virtual ~BasicCoordTranslator();
			motor_point_t getCenter();
			motor_size_t getScale();
			void setCenter(motor_point_t);
			void setScale(motor_size_t);
			virtual motor_point_t get(double, double);
			virtual coord_point_t get(motor_point_t);
			virtual CoordTranslator *clone(CoordTranslator*);
		private:
			motor_point_t center;
			motor_size_t scale;
	};
	
	class LinearCoordTranslator : public CoordTranslator {
		public:
			LinearCoordTranslator(coord_point_t, coord_scale_t, CoordTranslator* = nullptr);
			virtual ~LinearCoordTranslator();
			virtual CoordTranslator *getBase();
			virtual void setBase(CoordTranslator*);
			coord_point_t getOffset();
			coord_scale_t getScale();
			void setOffset(coord_point_t);
			void setScale(coord_scale_t);
			virtual motor_point_t get(double, double);
			virtual coord_point_t get(motor_point_t);
			virtual CoordTranslator *clone(CoordTranslator*);
		private:
			CoordTranslator *base;
			coord_point_t offset;
			coord_scale_t scale;
	};
	
	class LogarithmicCoordTranslator : public CoordTranslator {
		public:
			LogarithmicCoordTranslator(coord_scale_t, CoordTranslator* = nullptr);
			virtual ~LogarithmicCoordTranslator();
			virtual CoordTranslator *getBase();
			virtual void setBase(CoordTranslator*);
			coord_scale_t getScale();
			void setScale(coord_scale_t);
			virtual motor_point_t get(double, double);
			virtual coord_point_t get(motor_point_t);
			virtual CoordTranslator *clone(CoordTranslator*);
		private:
			CoordTranslator *base;
			coord_scale_t scale;
	};
	
	class PolarCoordTranslator : public CoordTranslator {
		public:
			PolarCoordTranslator(CoordTranslator* = nullptr);
			virtual ~PolarCoordTranslator();
			virtual CoordTranslator *getBase();
			virtual void setBase(CoordTranslator*);
			virtual motor_point_t get(double, double);
			virtual coord_point_t get(motor_point_t);
			virtual CoordTranslator *clone(CoordTranslator*);
		private:
			CoordTranslator *base;
	};
	
	class ComplexCoordTranslator : public CoordTranslator {
		public:
			ComplexCoordTranslator(CoordTranslator*);
			virtual ~ComplexCoordTranslator();
			virtual motor_point_t get(double, double);
			virtual coord_point_t get(motor_point_t);
			size_t getSize();
			CoordTranslator* get(size_t);
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
