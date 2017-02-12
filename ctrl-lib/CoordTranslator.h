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
			CoordTranslator();
			virtual ~CoordTranslator();
		private:
			motor_point_t offset;
			motor_point_t size;
	};

} /* namespace _8SMC1 */

#endif /* CTRL_LIB_COORDTRANSLATOR_H_ */
