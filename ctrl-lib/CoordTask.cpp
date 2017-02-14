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

	MoveTaskStep::MoveTaskStep(motor_point_t pos, float spc) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
	}

	MoveTaskStep::~MoveTaskStep() {

	}

	void MoveTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {
		ctrl->move(this->pos, this->speed_coef * prms.speed, 8, true);
	}

	JumpTaskStep::JumpTaskStep(motor_point_t pos, float spc) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
	}

	JumpTaskStep::~JumpTaskStep() {

	}

	void JumpTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {
		ctrl->move(this->pos, this->speed_coef * prms.speed, 8, false);
	}

	CalibrateTaskStep::CalibrateTaskStep(int side) : TaskStep::TaskStep() {
		this->side = side;
	}

	CalibrateTaskStep::~CalibrateTaskStep() {

	}

	void CalibrateTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {
		ctrl->calibrate(this->side);
	}

	ArcTaskStep::ArcTaskStep(motor_point_t dest, motor_point_t center, int sp, float speed) : TaskStep::TaskStep() {
		this->dest = dest;
		this->center = center;
		this->splitter = sp;
		this->speed = speed;
	}

	ArcTaskStep::~ArcTaskStep() {

	}

	void ArcTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {
		ctrl->arc(dest, center, splitter, this->speed * prms.speed, 8, this->clockwise);
	}

	void ArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}
}
