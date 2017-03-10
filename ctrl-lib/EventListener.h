#ifndef CALX_CTRL_LIB_EVENT_LISTENER_H_
#define CALX_CTRL_LIB_EVENT_LISTENER_H_

#include "CtrlCore.h"

namespace CalX {
	
	struct MotorMoveEvent {
		motor_coord_t destination;
		float speed;
		int divisor;
	};
	
	struct MotorErrorEvent {
		ErrorCode errcode;
	};
	
	struct MotorRollEvent {
		int trailer;
	};
	
	class MotorEventListener {
		public:
			virtual ~MotorEventListener() {}
			virtual void use() {}
			virtual void unuse() {}
			virtual void moving(MotorMoveEvent&) {}
			virtual void moved(MotorMoveEvent&) {}
			virtual void stopped(MotorErrorEvent&) {}
			virtual void rolling(MotorRollEvent&) {}
			virtual void rolled(MotorRollEvent&) {}
	};
	
	struct CoordMoveEvent {
		motor_point_t destination;
		float speed;
		int divisor;
		bool synchrone;
	};
	
	struct CoordErrorEvent {
		ErrorCode errcode;
	};
	
	struct CoordCalibrateEvent {
		TrailerId trailer;
	};
	
	class CoordEventListener {
		public:
			virtual ~CoordEventListener() {}
			virtual void use() {}
			virtual void unuse() {}
			virtual void moving(CoordMoveEvent&) {}
			virtual void moved(CoordMoveEvent&) {}
			virtual void stopped(CoordErrorEvent&) {}
			virtual void calibrating(CoordCalibrateEvent&) {}
			virtual void calibrated(CoordCalibrateEvent&) {}
	};
}


#endif