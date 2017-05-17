/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef CALX_CTRL_LIB_COORD_PLANE_H_
#define CALX_CTRL_LIB_COORD_PLANE_H_

#include <iostream>
#include <vector>
#include <string>
#include "ctrl-lib/ConfigManager.h"
#include "ctrl-lib/EventListener.h"
#include "ctrl-lib/MotorController.h"
#include "ctrl-lib/InstrumentController.h"

/* Coordinate plane is abstraction that represents real motors.
   However most of implementations are just filters that perform
   some actions on coordinates. Only CoordController represents real
   motor system. You should stack filters on top of this controller
   and use top filter(there available virtual coordinate plane,
   that is plane emulator but it is not tested). To simplify
   stacking you may use CoordPlaneStack, it also takes responsibility
   to free filter memory, otherwise you are responsible to free memroy. */

namespace CalX {
	
	// Main interface
	class CoordPlane {
		public:
			virtual ~CoordPlane() {}
			virtual ErrorCode move(motor_point_t, float, int, bool) = 0;
			virtual ErrorCode relativeMove(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false) = 0;
			virtual ErrorCode relativeArc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId) = 0;
			virtual ErrorCode measure(TrailerId) = 0;
			virtual motor_point_t getPosition() = 0;
			virtual motor_rect_t getSize() = 0;
			virtual bool isMeasured() = 0;
			virtual void use() = 0;
			virtual void unuse() = 0;
			virtual void stop() = 0;
			virtual void dump(std::ostream&) = 0;
			virtual CoordPlane *clone(CoordPlane*) = 0;
			virtual ErrorCode open_session() {return ErrorCode::NoError;}
			virtual ErrorCode close_session() {return ErrorCode::NoError;}
	};
		
	struct TaskState {
		CoordPlane *plane;
		bool work;
		
		void stop() {
			plane->stop();
			work = false;
		}
	};
	
	// Arc movement linearizator
	class CoordPlaneLinearizer : public CoordPlane {
		public:
			CoordPlaneLinearizer(CoordPlane*);
			virtual ~CoordPlaneLinearizer();
			CoordPlane *getBase();
			
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual ErrorCode measure(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual bool isMeasured();
			virtual void use();
			virtual void unuse();
			virtual void stop();
			virtual void dump(std::ostream&);
			virtual CoordPlane *clone(CoordPlane*);
			virtual ErrorCode open_session();
			virtual ErrorCode close_session();
		private:
			CoordPlane *base;
			bool work;
			bool defWork;
	};
	
	// Coordinate plane commuticating directly with hardware
	class CoordController : public CoordPlane {
		public:
			CoordController(MotorController*, MotorController*, ConfigManager*, InstrumentController* = nullptr);
			virtual ~CoordController();
			MotorController *getXAxis();
			MotorController *getYAxis();
			InstrumentController *getInstrument();
			
                        virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual ErrorCode measure(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual bool isMeasured();
			virtual void stop();
			void kill();
			virtual void dump(std::ostream&);
			virtual CoordPlane *clone(CoordPlane*);
			virtual ErrorCode open_session();
			virtual ErrorCode close_session();
			
			void addEventListener(CoordEventListener*);
			void removeEventListener(CoordEventListener*);
			virtual void use();
			virtual void unuse();
		private:		
			MotorController *xAxis;
			MotorController *yAxis;
			InstrumentController *instr;
			ConfigManager *config;
			bool work;
			bool defWork;
			
			motor_rect_t size;
			bool measured;
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
			virtual ErrorCode measure(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual bool isMeasured();
			virtual void use();
			virtual void unuse();
			virtual void stop();
			virtual void dump(std::ostream&);
			virtual CoordPlane *clone(CoordPlane*);
			virtual ErrorCode open_session();
			virtual ErrorCode close_session();
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
			CoordPlaneMap(motor_point_t, motor_scale_t, float, CoordPlane*);
			virtual ~CoordPlaneMap();
			CoordPlane *getBase();
			motor_point_t getOffset();
			motor_scale_t getScale();
			float getSpeedScale();
			void setOffset(motor_point_t);
			void setScale(motor_scale_t);
			void setSpeedScale(float);
						
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual ErrorCode measure(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual bool isMeasured();
			virtual void use();
			virtual void unuse();
			virtual void stop();
			virtual void dump(std::ostream&);
			virtual CoordPlane *clone(CoordPlane*);
			virtual ErrorCode open_session();
			virtual ErrorCode close_session();
		private:
			CoordPlane* plane;
			motor_point_t offset;
			motor_scale_t scale;
			float speedScale;
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
			virtual ErrorCode measure(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual bool isMeasured();
			virtual void use();
			virtual void unuse();
			virtual void stop();
			virtual void dump(std::ostream&);
			virtual CoordPlane *clone(CoordPlane*);
			virtual ErrorCode open_session();
			virtual ErrorCode close_session();
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
			CoordPlane *getBase();
			
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual ErrorCode measure(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual bool isMeasured();
			virtual void use();
			virtual void unuse();
			virtual void stop();
			virtual void dump(std::ostream&);
			virtual CoordPlaneStack *clone(CoordPlane*);
			virtual ErrorCode open_session();
			virtual ErrorCode close_session();
		private:
			std::vector<CoordPlane*> stack;
	};
	
	// Virtual coordinate plane emulator
	class VirtualCoordPlane : public CoordPlane {
		public:
			VirtualCoordPlane(motor_point_t, motor_rect_t);
			virtual ~VirtualCoordPlane();
			
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(TrailerId);
			virtual ErrorCode measure(TrailerId);
			virtual motor_point_t getPosition();
			virtual motor_rect_t getSize();
			virtual bool isMeasured();
			virtual CoordPlane *clone(CoordPlane*) = 0;
			virtual ErrorCode open_session();
			virtual ErrorCode close_session();
			
			virtual void use();
			virtual void unuse();
			virtual void stop();
			virtual void dump(std::ostream&);
		protected:
			virtual void jump(motor_point_t, bool) = 0;
		private:
			motor_point_t position;
			motor_rect_t size;
	};
}

#endif
