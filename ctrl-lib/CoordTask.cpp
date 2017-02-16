#include "CoordTask.h"
#include <cinttypes>
#include <iostream>
#include <math.h>

namespace _8SMC1 {

	ProgrammedCoordTask::ProgrammedCoordTask() :
		CoordTask::CoordTask(CoordTaskType::ProgrammedTask){

	}

	ProgrammedCoordTask::~ProgrammedCoordTask() {
		for (size_t i = 0; i < this->list.size(); i++) {
			delete this->list.at(i);
		}
	}

	void ProgrammedCoordTask::perform(CoordController *ctrl, TaskParameters &prms) {
		for (size_t i = 0; i < this->list.size(); i++) {
			this->list.at(i)->perform(ctrl, prms);
		}
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

	void MoveTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {
		if (this->rel) {
			ctrl->relativeMove(this->pos, this->speed_coef * prms.speed, 8, true);
		} else {
			ctrl->move(this->pos, this->speed_coef * prms.speed, 8, true);
		}
	}

	JumpTaskStep::JumpTaskStep(motor_point_t pos, float spc, bool rel) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
	}

	JumpTaskStep::~JumpTaskStep() {

	}

	void JumpTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {
		if (this->rel) {
			ctrl->relativeMove(this->pos, this->speed_coef * prms.speed, 8, false);
		} else {
			ctrl->move(this->pos, this->speed_coef * prms.speed, 8, false);
		}
	}

	CalibrateTaskStep::CalibrateTaskStep(int side) : TaskStep::TaskStep() {
		this->side = side;
	}

	CalibrateTaskStep::~CalibrateTaskStep() {

	}

	void CalibrateTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {
		ctrl->calibrate(this->side);
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

	void ArcTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {		
		if (this->rel) {
			ctrl->relativeArc(dest, center, splitter, this->speed * prms.speed, 8, this->clockwise);
		} else {
			ctrl->arc(dest, center, splitter, this->speed * prms.speed, 8, this->clockwise);
		}
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

	void RelArcTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {	
		motor_point_t cen = ctrl->getPosition();
		cen.x += center.x;
		cen.y += center.y;
		if (this->rel) {
			ctrl->relativeArc(dest, cen, splitter, this->speed * prms.speed, 8, this->clockwise);
		} else {
			ctrl->arc(dest, cen, splitter, this->speed * prms.speed, 8, this->clockwise);
		}
	}

	void RelArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}
}
