#ifndef _8SMC1_CTRL_LIB_TASK_H_
#define _8SMC1_CTRL_LIB_TASK_H_

#include <vector>
#include "DeviceController.h"

namespace Controller {

	class TaskStep {
		public:
			virtual ~TaskStep() {};
			virtual void perform(DeviceController*) = 0;
	};

	class JumpTaskStep : public TaskStep {
		public:
			JumpTaskStep(motor_point_t);
			virtual ~JumpTaskStep();
			virtual void perform(DeviceController*);
		private:
			motor_point_t pos;
	};

	class Task {
		public:
			Task();
			virtual ~Task();
			void perform(DeviceController*);
			void addStep(TaskStep*);
		private:
			std::vector<TaskStep*> list;
	};
}

#endif
