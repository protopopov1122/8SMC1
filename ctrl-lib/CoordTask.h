#ifndef _8SMC1_CTRL_LIB_TASK_H_
#define _8SMC1_CTRL_LIB_TASK_H_

#include <vector>
#include "DeviceController.h"

namespace Controller {

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

	class CalibrateTaskStep : public TaskStep {
		public:
			CalibrateTaskStep(int);
			virtual ~CalibrateTaskStep();
			virtual void perform(CoordController*, TaskParameters&);
		private:
			int side;
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
