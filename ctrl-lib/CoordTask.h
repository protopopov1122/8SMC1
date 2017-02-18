#ifndef _8SMC1_CTRL_LIB_TASK_H_
#define _8SMC1_CTRL_LIB_TASK_H_

#include <vector>
#include "CoordPlane.h"

namespace _8SMC1 {
	
	class SystemManager; // Forward referencing

	struct TaskParameters {
		float speed;
	};

	class TaskStep {
		public:
			virtual ~TaskStep() {};
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*) = 0;
	};

	class MoveTaskStep : public TaskStep {
		public:
			MoveTaskStep(motor_point_t, float, bool = false);
			virtual ~MoveTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*);
		private:
			motor_point_t pos;
			float speed_coef;
			bool rel;
	};

	class JumpTaskStep : public TaskStep {
		public:
			JumpTaskStep(motor_point_t, float, bool = false);
			virtual ~JumpTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*);
		private:
			motor_point_t pos;
			float speed_coef;
			bool rel;
	};

	class CalibrateTaskStep : public TaskStep {
		public:
			CalibrateTaskStep(TrailerId);
			virtual ~CalibrateTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*);
		private:
			TrailerId side;
	};

	class ArcTaskStep : public TaskStep {
		public:
			ArcTaskStep(motor_point_t, motor_point_t, int, float, bool = false);
			virtual ~ArcTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*);
			void setClockwise(bool);
		private:
			motor_point_t dest;
			motor_point_t center;
			int splitter;
			float speed;
			bool clockwise;
			bool rel;
	};

	// Calculates center position relatively to current position
	class RelArcTaskStep : public TaskStep {
		public:
			RelArcTaskStep(motor_point_t, motor_point_t, int, float, bool = false);
			virtual ~RelArcTaskStep();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*);
			void setClockwise(bool);
		private:
			motor_point_t dest;
			motor_point_t center;
			int splitter;
			float speed;
			bool clockwise;
			bool rel;
	};

	enum CoordTaskType {
		ProgrammedTask, FunctionTask
	};
	
	class CoordTask {
		public:
			CoordTask(CoordTaskType tp) {this->type = tp;}
			virtual ~CoordTask() {}
			CoordTaskType getType() {return this->type;}
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*) = 0;
		private:
			CoordTaskType type;
	};
	
	class ProgrammedCoordTask : public CoordTask {
		public:
			ProgrammedCoordTask();
			virtual ~ProgrammedCoordTask();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*);
			void addStep(TaskStep*);
			size_t getSubCount();
		private:
			std::vector<TaskStep*> list;
	};
}

#endif
