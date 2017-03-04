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

namespace _8SMC1 {

	enum CoordType {
		BasicCoord, LinearCoord, LogarithmicCoord, PolarCoord, ComplexCoord
	};

	class CoordTranslator {
		public:
			CoordTranslator(CoordType t) {this->type = t;}
			virtual ~CoordTranslator() {};
			CoordType getType() {return this->type;}
			virtual motor_point_t get(coord_point_t pnt) {return this->get(pnt.x, pnt.y);}
			virtual motor_point_t get(long double, long double) = 0;
			virtual coord_point_t get(motor_point_t) = 0;
			virtual CoordTranslator *getBase() {return nullptr;}
			virtual void setBase(CoordTranslator *b) {}
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
			virtual motor_point_t get(long double, long double);
			virtual coord_point_t get(motor_point_t);
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
			virtual motor_point_t get(long double, long double);
			virtual coord_point_t get(motor_point_t);
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
			virtual motor_point_t get(long double, long double);
			virtual coord_point_t get(motor_point_t);
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
			virtual motor_point_t get(long double, long double);
			virtual coord_point_t get(motor_point_t);
		private:
			CoordTranslator *base;
	};
	
	class ComplexCoordTranslator : public CoordTranslator {
		public:
			ComplexCoordTranslator(CoordTranslator*);
			virtual ~ComplexCoordTranslator();
			virtual motor_point_t get(long double, long double);
			virtual coord_point_t get(motor_point_t);
			size_t getSize();
			CoordTranslator* get(size_t);
			void add(CoordTranslator*);
			bool remove(size_t);
			bool insert(size_t, CoordTranslator*);
			virtual CoordTranslator *getBase();
			virtual void setBase(CoordTranslator*);
		private:
			std::vector<CoordTranslator*> list;
	};

}

#endif
