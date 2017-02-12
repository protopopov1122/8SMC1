#include "CoordTask.h"
#include "CircleGenerator.h"
#include <cinttypes>
#include <iostream>
#include <math.h>

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
		motor_point_t src = ctrl->getPosition();
		int r1 = (src.x - center.x) * (src.x - center.x) +
			     (src.y - center.y) * (src.y - center.y);
		int r2 = (dest.x - center.x) * (dest.x - center.x) +
			     (dest.y - center.y) * (dest.y - center.y);
		if (r1 != r2) {
			std::cout << "Error: Can not build arc" << std::endl;
			return;
		}
		Circle cir(center, (int) sqrt(r1));
		int add = this->clockwise ? 1 : -1;
		int start = cir.getFullSize() - 1;
		while (cir.getElement(start).x != src.x ||
			cir.getElement(start).y != src.y) {
			start += add;
		}
		std::vector<motor_point_t> path;
		while (cir.getElement(start).x != dest.x ||
			cir.getElement(start).y != dest.y) {
			motor_point_t point = cir.getElement(start);
			path.push_back(point);
			start += add;
		}
		for (size_t i = 0; i < (unsigned  int) this->splitter; i++) {
			if (ctrl->move(path.at(path.size() / this->splitter * i), prms.speed * this->speed, 8, true) !=
				ErrorCode::NoError) {
				std::cout << "Error" << std::endl;
				return;
			}
		}
		ctrl->move(dest, prms.speed * this->speed, 8, true);
	}

	void ArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}
}
