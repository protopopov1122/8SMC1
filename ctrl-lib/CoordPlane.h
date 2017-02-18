#ifndef _8SMC1_CTRL_LIB_COORD_PLANE_H_
#define _8SMC1_CTRL_LIB_COORD_PLANE_H_

#include <iostream>
#include "DeviceController.h"

/* This header contains coordinate plane interface definitions and some implementations.*/

namespace _8SMC1 {
		
	// Main interface
	class CoordPlane {
		public:
			virtual ~CoordPlane() {};
			virtual ErrorCode move(motor_point_t, float, int, bool) = 0;
			virtual ErrorCode relativeMove(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false) = 0;
			virtual ErrorCode relativeArc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId) = 0;
			virtual motor_point_t getPosition() = 0;
	};
	
	// Coordinate plane commuticating directly with hardware
	class CoordController : public CoordPlane {
		public:
			CoordController(DeviceController*, DeviceController*);
			virtual ~CoordController();
			DeviceController *getXAxis();
			DeviceController *getYAxis();
			
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
		private:
			DeviceController *xAxis;
			DeviceController *yAxis;
			
	};
	
	// Logging coordinate plane
	class CoordPlaneLog : public CoordPlane {
		public:
			CoordPlaneLog(CoordPlane*, std::ostream&);
			~CoordPlaneLog();
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
		private:
			CoordPlane *plane;
			std::ostream *out;
	};
}

#endif