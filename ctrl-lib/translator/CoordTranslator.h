#ifndef CTRL_LIB_COORDTRANSLATOR_H_
#define CTRL_LIB_COORDTRANSLATOR_H_

#include <cinttypes>
#include "ctrl-lib/DeviceController.h"

namespace _8SMC1 {

	

	class CoordTranslator {
		public:
			virtual ~CoordTranslator() {};
			virtual motor_point_t get(long double, long double) = 0;
	};
	
	class BasicCoordTranslator : public CoordTranslator {
		public:
			BasicCoordTranslator(motor_point_t, motor_size_t);
			virtual ~BasicCoordTranslator();
			motor_point_t getCenter();
			motor_size_t getScale();
			virtual motor_point_t get(long double, long double);
		private:
			motor_point_t center;
			motor_size_t scale;
	};
	
	class LinearCoordTranslator : public CoordTranslator {
		public:
			LinearCoordTranslator(coord_point_t, coord_scale_t, CoordTranslator* = nullptr);
			virtual ~LinearCoordTranslator();
			CoordTranslator *getBase();
			coord_point_t getOffset();
			coord_scale_t getScale();
			virtual motor_point_t get(long double, long double);
		private:
			CoordTranslator *base;
			coord_point_t offset;
			coord_scale_t scale;
	};
	
	class LogarithmicCoordTranslator : public CoordTranslator {
		public:
			LogarithmicCoordTranslator(motor_scale_t, CoordTranslator* = nullptr);
			virtual ~LogarithmicCoordTranslator();
			CoordTranslator *getBaseCoord();
			motor_scale_t getScale();
			virtual motor_point_t get(long double, long double);
		private:
			CoordTranslator *base;
			motor_scale_t scale;
	};
	
	class PolarCoordTranslator : public CoordTranslator {
		public:
			PolarCoordTranslator(CoordTranslator* = nullptr);
			virtual ~PolarCoordTranslator();
			CoordTranslator *getBaseCoord();
			virtual motor_point_t get(long double, long double);
		private:
			CoordTranslator *base;
	};

}

#endif
