#ifndef _8SMC1_CTRL_LIB_TASK_H_
#define _8SMC1_CTRL_LIB_TASK_H_

#include <vector>
#include "CoordPlane.h"
#include "CoordTranslator.h"
#include "GraphBuilder.h"
#include "graph/FunctionEngine.h"

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
		ProgrammedTask, GraphTask, WrapperTask
	};
	
	class CoordTask : public TaskStep {
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
	
	class GraphCoordTask : public CoordTask {
		public:
			GraphCoordTask(GraphBuilder*, CoordTranslator*, float);
			virtual ~GraphCoordTask();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*);
		private:
			GraphBuilder *graph;
			CoordTranslator *trans;
			float scale;
	};
	
	class CoordTaskWrapper : public CoordTask {
		public:
			CoordTaskWrapper(CoordTask*);
			virtual ~CoordTaskWrapper();
			virtual ErrorCode perform(CoordPlane*, TaskParameters&, SystemManager*);
			
			void setCalibration(TrailerId);
			void setValidation(motor_point_t, motor_point_t, float);
			void setMap(motor_point_t, motor_scale_t);
		private:
			CoordTask *task;
			// Wrapper options
			bool cal_tr; // Calibrate trailers
			bool coord_map;	// Coordinate plane map
			bool coord_val;	// Coordinate plane validate
			// Wrapper values
			TrailerId tr;	// Calibration
			// Map
			motor_point_t coord_offset;
			motor_scale_t coord_scale;
			// Validate
			motor_point_t val_min;
			motor_point_t val_max;
			float val_speed;
	};
}

#endif
