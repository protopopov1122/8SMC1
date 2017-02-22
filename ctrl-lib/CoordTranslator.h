#ifndef CTRL_LIB_COORDTRANSLATOR_H_
#define CTRL_LIB_COORDTRANSLATOR_H_

#include <cinttypes>
#include "DeviceController.h"

namespace _8SMC1 {

	struct decimal_number {
		uint32_t i;
		uint32_t j;
		int8_t s;
	};

	class CoordTranslator {
		public:
			virtual ~CoordTranslator() {};
			virtual motor_point_t get(long double, long double) = 0;
			virtual motor_point_t get(int64_t, int64_t) = 0;
			virtual motor_point_t get(decimal_number, decimal_number) = 0;
	};
	
	class LinearCoordTranslator : public CoordTranslator {
		public:
			LinearCoordTranslator(motor_point_t, motor_size_t);
			virtual ~LinearCoordTranslator();
			motor_point_t getCenter();
			motor_size_t getScale();
			virtual motor_point_t get(long double, long double);
			virtual motor_point_t get(decimal_number, decimal_number);
			virtual motor_point_t get(int64_t, int64_t);
		private:
			motor_point_t center;
			motor_size_t scale;
	};

}

#endif
