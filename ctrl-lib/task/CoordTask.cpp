/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "ctrl-lib/task/CoordTask.h"
#include "ctrl-lib/SystemManager.h"
#include <cinttypes>
#include <iostream>
#include <math.h>
#include <limits>

namespace CalX {

	ProgrammedCoordTask::ProgrammedCoordTask() :
		CoordTask::CoordTask(CoordTaskType::ProgrammedTask){
		INIT_LOG("ProgrammedCoordTask");
	}

	ProgrammedCoordTask::~ProgrammedCoordTask() {
		for (size_t i = 0; i < this->list.size(); i++) {
			delete this->list.at(i);
		}
		DESTROY_LOG("ProgrammedCoordTask");
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

	bool ProgrammedCoordTask::removeStep(size_t i) {
		if (i >= this->list.size()) {
			return false;
		}
		delete this->list.at(i);
        this->list.erase(this->list.begin() + (std::ptrdiff_t) i);
		return true;
	}

	TaskStep *ProgrammedCoordTask::pollStep(size_t i) {
		if (i >= this->list.size()) {
			return nullptr;
		}
		TaskStep *step = this->list.at(i);
        this->list.erase(this->list.begin() + (std::ptrdiff_t) i);
		return step;
	}

	bool ProgrammedCoordTask::insertStep(size_t i, TaskStep *step) {
		if (i > this->list.size()) {
			return false;
		}
		if (i == this->list.size()) {
			this->list.push_back(step);
		} else {
            this->list.insert(this->list.begin() + (std::ptrdiff_t) i, step);
		}
		return true;
	}

	MoveTaskStep::MoveTaskStep(motor_point_t pos, float spc, bool rel) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
		INIT_LOG("MoveTaskStep");
	}

	MoveTaskStep::~MoveTaskStep() {
		DESTROY_LOG("MoveTaskStep");
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

	motor_point_t MoveTaskStep::getPosition() {
		return this->pos;
	}

	float MoveTaskStep::getSpeed() {
		return this->speed_coef;
	}

	bool MoveTaskStep::isRelative() {
		return this->rel;
	}

	void MoveTaskStep::setPosition(motor_point_t dest) {
		this->pos = dest;
	}

	void MoveTaskStep::setSpeed(float sp) {
		this->speed_coef = sp;
	}

	void MoveTaskStep::setRelative(bool r) {
		this->rel = r;
	}

	JumpTaskStep::JumpTaskStep(motor_point_t pos, float spc, bool rel) : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
		INIT_LOG("JumpTaskStep");
	}

	JumpTaskStep::~JumpTaskStep() {
		DESTROY_LOG("JumpTaskStep");
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

	motor_point_t JumpTaskStep::getPosition() {
		return this->pos;
	}

	float JumpTaskStep::getSpeed() {
		return this->speed_coef;
	}

	bool JumpTaskStep::isRelative() {
		return this->rel;
	}

	void JumpTaskStep::setPosition(motor_point_t dest) {
		this->pos = dest;
	}

	void JumpTaskStep::setSpeed(float sp) {
		this->speed_coef = sp;
	}

	void JumpTaskStep::setRelative(bool r) {
		this->rel = r;
	}

	CalibrateTaskStep::CalibrateTaskStep(TrailerId side) : TaskStep::TaskStep() {
		this->side = side;
		INIT_LOG("CalibrateTaskStep");
	}

	CalibrateTaskStep::~CalibrateTaskStep() {
		DESTROY_LOG("CalibrateTaskStep");
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
		INIT_LOG("ArcTaskStep");
	}

	ArcTaskStep::~ArcTaskStep() {
		DESTROY_LOG("ArcTaskStep");
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

	motor_point_t ArcTaskStep::getDestination() {
		return this->dest;
	}

	motor_point_t ArcTaskStep::getCenter() {
		return this->center;
	}

	int ArcTaskStep::getSplitter() {
		return this->splitter;
	}

	float ArcTaskStep::getSpeed() {
		return this->speed;
	}

	bool ArcTaskStep::isClockwise() {
		return this->clockwise;
	}

	bool ArcTaskStep::isRelative() {
		return this->rel;
	}

	void ArcTaskStep::setDestination(motor_point_t p) {
		this->dest = p;
	}

	void ArcTaskStep::setCenter(motor_point_t p) {
		this->center = p;
	}

	void ArcTaskStep::setSplitter(int s) {
		this->splitter = s;
	}

	void ArcTaskStep::setSpeed(float s) {
		this->speed = s;
	}

	void ArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}

	void ArcTaskStep::setRelative(bool r) {
		this->rel = r;
	}

	RelArcTaskStep::RelArcTaskStep(motor_point_t dest, motor_point_t center, int sp, float speed, bool rel)
			: TaskStep::TaskStep() {
		this->dest = dest;
		this->center = center;
		this->splitter = sp;
		this->speed = speed;
		this->rel = rel;
		INIT_LOG("RelArcTaskStep");
	}

	RelArcTaskStep::~RelArcTaskStep() {
		DESTROY_LOG("RelArcTaskStep");
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
		INIT_LOG("GraphCoordTask");
	}

	GraphCoordTask::~GraphCoordTask() {
		delete this->graph;
		delete this->trans;
		DESTROY_LOG("GraphCoordTask");
	}

	ErrorCode GraphCoordTask::perform(CoordPlane *plane, TaskParameters &prms, SystemManager *sysman, TaskState *state) {
		return this->graph->build(sysman, plane, this->trans, this->scale * prms.speed, state);
	}
}
