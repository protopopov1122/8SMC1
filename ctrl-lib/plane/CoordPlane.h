#ifndef _8SMC1_CTRL_LIB_COORD_PLANE_H_
#define _8SMC1_CTRL_LIB_COORD_PLANE_H_

#include <iostream>
#include <vector>
#include <string>
#include "ctrl-lib/EventListener.h"
#include "ctrl-lib/DeviceController.h"

/* Coordinate plane is abstraction that represents real motors.
   However most of implementations are just filters that perform
   some actions on coordinates. Only CoordController represents real
   motor system. You should stack filters on top of this controller
   and use top filter(there available virtual coordinate plane,
   that is plane emulator but it is not tested). To simplify
   stacking you may use CoordPlaneStack, it also takes responsibility
   to free filter memory, otherwise you are responsible to free memroy. */

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
			virtual void use() = 0;
			virtual void unuse() = 0;
			virtual void dump(std::ostream&) = 0;
	};
	
	// Coordinate plane commuticating directly with hardware
	class CoordController : public CoordPlane {
		public:
			CoordController(DeviceController*, DeviceController*);
			virtual ~CoordController();
			DeviceController *getXAxis();
			DeviceController *getYAxis();
			virtual void measure(TrailerId);
			
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void dump(std::ostream&);
			
			void addEventListener(CoordEventListener*);
			void removeEventListener(CoordEventListener*);
			virtual void use();
			virtual void unuse();
		private:		
			DeviceController *xAxis;
			DeviceController *yAxis;
			
			motor_rect_t size;
			std::vector<CoordEventListener*> listeners;
			
			void sendMovingEvent(CoordMoveEvent&);
			void sendMovedEvent(CoordMoveEvent&);
			void sendStoppedEvent(CoordErrorEvent&);
			void sendCalibratingEvent(CoordCalibrateEvent&);
			void sendCalibratedEvent(CoordCalibrateEvent&);
			
	};
	
	// Logging coordinate plane
	class CoordPlaneLog : public CoordPlane {
		public:
			CoordPlaneLog(CoordPlane*, std::ostream*, std::string, bool = true, bool = true);
			virtual ~CoordPlaneLog();
			CoordPlane *getBase();
			std::string getPrefix();
			bool isLoggingActions();
			bool isLoggingErrors();
			void setPrefix(std::string);
			void setLoggingActions(bool);
			void setLoggingErrors(bool);
			
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void use();
			virtual void unuse();
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
			CoordPlane *getBase();
			motor_point_t getOffset();
			motor_scale_t getScale();
			void setOffset(motor_point_t);
			void setScale(motor_scale_t);
						
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void use();
			virtual void unuse();
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
			CoordPlane *getBase();
			motor_point_t getMinimum();
			motor_point_t getMaximum();
			float getMaxSpeed();
			void setMinimum(motor_point_t);
			void setMaximum(motor_point_t);
			void setMaxSpeed(float);
						
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void use();
			virtual void unuse();
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
			virtual void use();
			virtual void unuse();
			virtual void dump(std::ostream&);
		private:
			std::vector<CoordPlane*> stack;
	};
	
	// Virtual device emulator
	class VirtualDevice {
		public:
			virtual ~VirtualDevice() {};
			virtual motor_coord_t getPosition() = 0;
			virtual ErrorCode start(motor_coord_t, float, int) = 0;
			virtual bool isTrailerPressed(TrailerId) = 0;
	};
	
	// Virtual coordinate plane emulator
	class VirtualCoordPlane : public CoordPlane {
		public:
			VirtualCoordPlane(VirtualDevice*, VirtualDevice*);
			virtual ~VirtualCoordPlane();
			VirtualDevice *getXAxis();
			VirtualDevice *getYAxis();
			virtual void measure(TrailerId);
			
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual void use();
			virtual void unuse();
			virtual void dump(std::ostream&);
			
		private:
			VirtualDevice *xDev;
			VirtualDevice *yDev;
			motor_rect_t size;
	};
}

#endif