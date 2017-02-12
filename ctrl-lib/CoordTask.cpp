#include "CoordTask.h"
#include <cinttypes>

namespace _8SMC1 {

	CoordTask::CoordTask() {

	}

	CoordTask::~CoordTask() {
		for (size_t i = 0; i < this->list.size(); i++) {
			delete this->list.at(i);
		}
	}

	void CoordTask::perform(CoordController *ctrl, TaskParameters &prms) {
		for (size_t i = 0; i < this->list.size(); i++) {
			this->list.at(i)->perform(ctrl, prms);
		}
	}

	void CoordTask::addStep(TaskStep *st) {
		this->list.push_back(st);
	}

	size_t CoordTask::getSubCount() {
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

	CalibrateTaskStep::CalibrateTaskStep(int side) : TaskStep::TaskStep() {
		this->side = side;
	}

	CalibrateTaskStep::~CalibrateTaskStep() {

	}

	void CalibrateTaskStep::perform(CoordController *ctrl, TaskParameters &prms) {
		ctrl->calibrate(this->side);
	}
}
