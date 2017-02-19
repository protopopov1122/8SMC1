/*
 * CoordTranslator.h
 *
 *      Author: Eugene Protopopov <protopopov1122@yandex.ru>
 */

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
			CoordTranslator(motor_point_t, motor_size_t);
			virtual ~CoordTranslator();
			motor_point_t getCenter();
			motor_size_t getScale();
			motor_point_t get(long double, long double);
			motor_point_t get(decimal_number, decimal_number);
			motor_point_t get(int64_t, int64_t);
		private:
			motor_point_t center;
			motor_size_t scale;
	};

} /* namespace _8SMC1 */

#endif /* CTRL_LIB_COORDTRANSLATOR_H_ */
