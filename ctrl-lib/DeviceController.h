#ifndef _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_
#define _8SMC1_CTRL_LIB_DEVICE_CONTROLLER_H_

#include <cinttypes>
#include <pthread.h>
#include "DeviceManager.h"

/* This file contains definitions of high-level controller library.
   Library purpose is controlling device state, speed, complex
   moves and synchronization. */

typedef int64_t motor_coord_t;
namespace _8SMC1 {

	enum ErrorCode {
		NoError = 0, WrongParameter = 1,
		Trailer1Pressed = 2, Trailer2Pressed = 3,
		DeviceRunning = 4, DeviceStopped = 5,
		ArcError = 6
	};

	enum MoveType {
		Stop = 0, MoveDown = -1, MoveUp = 1,
		RollDown = -2, RollUp = 2
	};

	struct motor_point_t {
		motor_coord_t x;
		motor_coord_t y;
	};	

	struct motor_size_t {
		motor_coord_t w;
		motor_coord_t h;
	};

#define ROLL_STEP 30000
#define ROLL_SPEED 4000.0f
#define ROLL_DIV 8
#define TRAILER_COMEBACK 800

	class CoordController;

	class DeviceController {
		public:
			friend class CoordController;

			DeviceController(Device*);
			virtual ~DeviceController();
			Device *getDevice();

			ErrorCode checkTrailers();
			ErrorCode waitWhileRunning();
			ErrorCode moveToTrailer(int, int);
			ErrorCode resetPosition();
			ErrorCode startMove(motor_coord_t, float, int, bool = false);
			ErrorCode startRelativeMove(motor_coord_t, float, int, bool = false);
			void stop();
			unsigned int getLength();

			motor_coord_t getPosition();

			friend void *device_control_thread(void*);
		private:
			// Calibration
			void calculate_length();
			// Threading related
			pthread_t dev_thread;
			bool exists;
			bool wait_for_thread;

			Device *dev;
			MoveType dest;
			unsigned int length;
	};

	class CoordPlane {
		public:
			virtual ~CoordPlane() {};
			virtual ErrorCode move(motor_point_t, float, int, bool) = 0;
			virtual ErrorCode relativeMove(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false) = 0;
			virtual ErrorCode relativeArc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(int) = 0;
			virtual motor_point_t getPosition() = 0;
	};
	
	class CoordController : public CoordPlane {
		public:
			CoordController(DeviceController*, DeviceController*);
			virtual ~CoordController();
			DeviceController *getXAxis();
			DeviceController *getYAxis();
			virtual ErrorCode move(motor_point_t, float, int, bool);
			virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, int, bool, bool = false);
			virtual ErrorCode calibrate(int);
			virtual motor_point_t getPosition();
		private:
			DeviceController *xAxis;
			DeviceController *yAxis;
			
			motor_size_t scale;
			
	};
}

#endif
