#include "CoordTask.h"
#include "ctrl-lib/SystemManager.h"
#include <cinttypes>
#include <iostream>
#include <math.h>
#include <limits>

namespace _8SMC1 {

	ProgrammedCoordTask::ProgrammedCoordTask() :
		CoordTask::CoordTask(CoordTaskType::ProgrammedTask){

	}

	ProgrammedCoordTask::~ProgrammedCoordTask() {
		for (size_t i = 0; i < this->list.size(); i++) {
			delete this->list.at(i);
		}
	}

	ErrorCode ProgrammedCoordTask::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		ErrorCode errcode;
		state->plane = ctrl;
		state->work = true;
		ctrl->use();
		TaskState subSt;
		for (size_t i = 0; i < this->list.size() && state->work; i++) {
			errcode = this->list.at(i)->perform(ctrl, prms, sysman, &subSt);
			if (errcode != ErrorCode::NoError) {
				state->work = false;
				ctrl->unuse();
				return errcode;
			}
		}
		state->work = false;
		ctrl->unuse();
		return ErrorCode::NoError;
	}

	void ProgrammedCoordTask::addStep(TaskStep *st) {
		this->list.push_back(st);
	}

	size_t ProgrammedCoordTask::getSubCount() {
		return this->list.size();
	}

	MoveTaskStep::MoveTaskStep(motor_point_t pos, float spc, bool rel) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
	}

	MoveTaskStep::~MoveTaskStep() {

	}

	ErrorCode MoveTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		ErrorCode code;
		state->work = true;
		state->plane = ctrl;
		if (this->rel) {
			code = ctrl->relativeMove(this->pos, this->speed_coef * prms.speed, 8, true);
		} else {
			code = ctrl->move(this->pos, this->speed_coef * prms.speed, 8, true);
		}
		state->work = false;
		return code;
	}

	JumpTaskStep::JumpTaskStep(motor_point_t pos, float spc, bool rel) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
	}

	JumpTaskStep::~JumpTaskStep() {

	}

	ErrorCode JumpTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		ErrorCode code;
		state->plane = ctrl;
		state->work = true;
		if (this->rel) {
			code = ctrl->relativeMove(this->pos, this->speed_coef * prms.speed, 8, false);
		} else {
			code = ctrl->move(this->pos, this->speed_coef * prms.speed, 8, false);
		}
		state->work = false;
		return code;
	}

	CalibrateTaskStep::CalibrateTaskStep(TrailerId side) : TaskStep::TaskStep() {
		this->side = side;
	}

	CalibrateTaskStep::~CalibrateTaskStep() {

	}

	ErrorCode CalibrateTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		state->plane = ctrl;
		state->work = true;
		ErrorCode code = ctrl->calibrate(this->side);
		state->work = false;
		return code;
	}

	ArcTaskStep::ArcTaskStep(motor_point_t dest, motor_point_t center, int sp, float speed, bool rel)
			: TaskStep::TaskStep() {
		this->dest = dest;
		this->center = center;
		this->splitter = sp;
		this->speed = speed;
		this->rel = rel;
	}

	ArcTaskStep::~ArcTaskStep() {

	}

	ErrorCode ArcTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman, TaskState *state) {		
		ErrorCode code;
		state->plane = ctrl;
		state->work = true;
		if (this->rel) {
			code = ctrl->relativeArc(dest, center, splitter, this->speed * prms.speed, 8, this->clockwise);
		} else {
			code = ctrl->arc(dest, center, splitter, this->speed * prms.speed, 8, this->clockwise);
		}
		state->work = false;
		return code;
	}

	void ArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}

	RelArcTaskStep::RelArcTaskStep(motor_point_t dest, motor_point_t center, int sp, float speed, bool rel)
			: TaskStep::TaskStep() {
		this->dest = dest;
		this->center = center;
		this->splitter = sp;
		this->speed = speed;
		this->rel = rel;
		
	}

	RelArcTaskStep::~RelArcTaskStep() {

	}

	ErrorCode RelArcTaskStep::perform(CoordPlane *ctrl, TaskParameters &prms, SystemManager *sysman, TaskState *state) {	
		motor_point_t cen = ctrl->getPosition();
		cen.x += center.x;
		cen.y += center.y;
		ErrorCode code;
		state->plane = ctrl;
		state->work = true;
		if (this->rel) {
			code = ctrl->relativeArc(dest, cen, splitter, this->speed * prms.speed, 8, this->clockwise);
		} else {
			code = ctrl->arc(dest, cen, splitter, this->speed * prms.speed, 8, this->clockwise);
		}
		state->work = false;
		return code;
	}

	void RelArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}
	
	GraphCoordTask::GraphCoordTask(GraphBuilder *graph, CoordTranslator *trans, float scale)
		: CoordTask::CoordTask(CoordTaskType::GraphTask) {
		this->graph = graph;
		this->trans = trans;
		this->scale = scale;
	}
	
	GraphCoordTask::~GraphCoordTask() {
		delete this->graph;
		delete this->trans;
	}
	
	ErrorCode GraphCoordTask::perform(CoordPlane *plane, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		return this->graph->build(sysman, plane, this->trans, this->scale * prms.speed, state);
	}
}
