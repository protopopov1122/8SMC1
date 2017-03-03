#ifndef _8SMC1_CTRL_LIB_EVENT_LISTENER_H_
#define _8SMC1_CTRL_LIB_EVENT_LISTENER_H_

#include "CtrlCore.h"

namespace _8SMC1 {
	
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
			virtual void moving(CoordMoveEvent&) {}
			virtual void moved(CoordMoveEvent&) {}
			virtual void stopped(CoordErrorEvent&) {}
			virtual void calibrating(CoordCalibrateEvent&) {}
			virtual void calibrated(CoordCalibrateEvent&) {}
	};
}


#endif