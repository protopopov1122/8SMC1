/*
 * CoordTranslator.h
 *
 *      Author: Eugene Protopopov <protopopov1122@yandex.ru>
 */

#ifndef CTRL_LIB_COORDTRANSLATOR_H_
#define CTRL_LIB_COORDTRANSLATOR_H_

#include "DeviceController.h"

namespace _8SMC1 {

	class CoordTranslator {
		public:
			CoordTranslator(motor_point_t, motor_size_t, motor_size_t);
			virtual ~CoordTranslator();
			motor_point_t getCenter();
			motor_size_t getSize();
			motor_size_t getScale();
			motor_point_t get(long double, long double);
		private:
			motor_point_t center;
			motor_size_t size;
			motor_size_t scale;
	};

} /* namespace _8SMC1 */

#endif /* CTRL_LIB_COORDTRANSLATOR_H_ */
