#ifndef _8SMC1_CTRL_LIB_TASK_H_
#define _8SMC1_CTRL_LIB_TASK_H_

#include <vector>
#include "DeviceController.h"

namespace _8SMC1 {

	struct TaskParameters {
		float speed;
	};

	class TaskStep {
		public:
			virtual ~TaskStep() {};
			virtual void perform(CoordController*, TaskParameters&) = 0;
	};

	class MoveTaskStep : public TaskStep {
		public:
			MoveTaskStep(motor_point_t, float);
			virtual ~MoveTaskStep();
			virtual void perform(CoordController*, TaskParameters&);
		private:
			motor_point_t pos;
			float speed_coef;
	};

	class JumpTaskStep : public TaskStep {
		public:
			JumpTaskStep(motor_point_t, float);
			virtual ~JumpTaskStep();
			virtual void perform(CoordController*, TaskParameters&);
		private:
			motor_point_t pos;
			float speed_coef;
	};

	class CalibrateTaskStep : public TaskStep {
		public:
			CalibrateTaskStep(int);
			virtual ~CalibrateTaskStep();
			virtual void perform(CoordController*, TaskParameters&);
		private:
			int side;
	};

	class ArcTaskStep : public TaskStep {
		public:
			ArcTaskStep(motor_point_t, motor_point_t, int, float);
			virtual ~ArcTaskStep();
			virtual void perform(CoordController*, TaskParameters&);
			void setClockwise(bool);
		private:
			motor_point_t dest;
			motor_point_t center;
			int splitter;
			float speed;
			bool clockwise;
	};

	class CoordTask {
		public:
			CoordTask();
			virtual ~CoordTask();
			void perform(CoordController*, TaskParameters&);
			void addStep(TaskStep*);
			size_t getSubCount();
		private:
			std::vector<TaskStep*> list;
	};
}

#endif
