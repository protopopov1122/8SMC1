#ifndef _8SMC1_CTRL_LIB_COORD_PLANE_H_
#define _8SMC1_CTRL_LIB_COORD_PLANE_H_

#include <iostream>
#include <vector>
#include <string>
#include "ctrl-lib/DeviceController.h"

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
			virtual motor_rect_t getSize() = 0;
			virtual void dump(std::ostream&);
		private:
			friend std::ostream& operator<<(std::ostream&, CoordPlane&);
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
			virtual motor_rect_t getSize();
			virtual void measure(TrailerId);
			virtual void dump(std::ostream&);
		private:
			DeviceController *xAxis;
			DeviceController *yAxis;
			
			motor_rect_t size;
			
	};
	
	// Logging coordinate plane
	class CoordPlaneLog : public CoordPlane {
		public:
			CoordPlaneLog(CoordPlane*, std::ostream&, std::string, bool = true, bool = true);
			virtual ~CoordPlaneLog();
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void dump(std::ostream&);
		private:
			CoordPlane *plane;
			std::ostream *out;
			std::string prefix;
			bool log_actions;
			bool log_errors;
	};
	
	// Mapping coordinates
	class CoordPlaneMap : public CoordPlane {
		public:
			CoordPlaneMap(motor_point_t, motor_scale_t, CoordPlane*);
			virtual ~CoordPlaneMap();
						
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void dump(std::ostream&);
		private:
			CoordPlane* plane;
			motor_point_t offset;
			motor_scale_t scale;
	};
	
	// Coordinate and speed validation
	class CoordPlaneValidator : public CoordPlane {
		public:
			CoordPlaneValidator(motor_point_t, motor_point_t, float, CoordPlane*);
			virtual ~CoordPlaneValidator();
						
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void dump(std::ostream&);
		private:
			CoordPlane *plane;
			motor_point_t min;
			motor_point_t max;
			float max_speed;
	};
	
	// Coordinate plane abstraction stacking
	class CoordPlaneStack : public CoordPlane {
		public:
			CoordPlaneStack(CoordPlane*);
			virtual ~CoordPlaneStack();
			
			CoordPlane *peekPlane();
			bool popPlane();
			void pushPlane(CoordPlane*);
			
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void dump(std::ostream&);
		private:
			std::vector<CoordPlane*> stack;
	};
}

#endif