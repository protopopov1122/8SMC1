/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/task/CoordTask.h"
#include "calx/ctrl-lib/SystemManager.h"
#include <cinttypes>
#include <limits>
#include <cmath>

namespace CalX {

	ProgrammedCoordTask::ProgrammedCoordTask()
	    : CoordTask::CoordTask(CoordTaskType::ProgrammedTask) {}

	ErrorCode ProgrammedCoordTask::perform(std::shared_ptr<CoordPlane> ctrl,
	                                       TaskParameters &prms,
	                                       SystemManager &sysman,
	                                       std::shared_ptr<TaskState> state) {
		ErrorCode errcode;
		state->plane = ctrl;
		state->work = true;
		ctrl->use();
		std::shared_ptr<TaskState> subSt = std::make_shared<TaskState>();
		for (std::size_t i = 0; i < this->list.size() && state->work; i++) {
			errcode = this->list.at(i)->perform(ctrl, prms, sysman, subSt);
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

	void ProgrammedCoordTask::addStep(std::shared_ptr<TaskStep> st) {
		this->list.push_back(st);
	}

	std::size_t ProgrammedCoordTask::getSubCount() const {
		return this->list.size();
	}

	bool ProgrammedCoordTask::removeStep(std::size_t i) {
		if (i >= this->list.size()) {
			return false;
		}
		this->list.erase(this->list.begin() + (std::ptrdiff_t) i);
		return true;
	}

	std::shared_ptr<TaskStep> ProgrammedCoordTask::pollStep(std::size_t i) {
		if (i >= this->list.size()) {
			return nullptr;
		}
		std::shared_ptr<TaskStep> step = this->list.at(i);
		this->list.erase(this->list.begin() + (std::ptrdiff_t) i);
		return step;
	}

	bool ProgrammedCoordTask::insertStep(std::size_t i,
	                                     std::shared_ptr<TaskStep> step) {
		if (i > this->list.size()) {
			return false;
		}
		if (i == this->list.size()) {
			this->list.push_back(step);
		} else {
			this->list.insert(this->list.begin() + (std::ptrdiff_t) i,
			                  std::move(step));
		}
		return true;
	}

	MoveTaskStep::MoveTaskStep(motor_point_t pos, float spc, bool rel)
	    : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
	}

	ErrorCode MoveTaskStep::perform(std::shared_ptr<CoordPlane> ctrl,
	                                TaskParameters &prms, SystemManager &sysman,
	                                std::shared_ptr<TaskState> state) {
		ErrorCode code;
		state->work = true;
		state->plane = ctrl;
		if (this->rel) {
			code = ctrl->relativeMove(this->pos, this->speed_coef * prms.speed, true);
		} else {
			code = ctrl->move(this->pos, this->speed_coef * prms.speed, true);
		}
		state->work = false;
		return code;
	}

	motor_point_t MoveTaskStep::getPosition() const {
		return this->pos;
	}

	float MoveTaskStep::getSpeed() const {
		return this->speed_coef;
	}

	bool MoveTaskStep::isRelative() const {
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

	JumpTaskStep::JumpTaskStep(motor_point_t pos, float spc, bool rel)
	    : TaskStep::TaskStep() {
		this->pos = pos;
		this->speed_coef = spc;
		this->rel = rel;
	}

	ErrorCode JumpTaskStep::perform(std::shared_ptr<CoordPlane> ctrl,
	                                TaskParameters &prms, SystemManager &sysman,
	                                std::shared_ptr<TaskState> state) {
		ErrorCode code;
		state->plane = ctrl;
		state->work = true;
		if (this->rel) {
			code =
			    ctrl->relativeMove(this->pos, this->speed_coef * prms.speed, false);
		} else {
			code = ctrl->move(this->pos, this->speed_coef * prms.speed, false);
		}
		state->work = false;
		return code;
	}

	motor_point_t JumpTaskStep::getPosition() const {
		return this->pos;
	}

	float JumpTaskStep::getSpeed() const {
		return this->speed_coef;
	}

	bool JumpTaskStep::isRelative() const {
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
	}

	ErrorCode CalibrateTaskStep::perform(std::shared_ptr<CoordPlane> ctrl,
	                                     TaskParameters &prms,
	                                     SystemManager &sysman,
	                                     std::shared_ptr<TaskState> state) {
		state->plane = ctrl;
		state->work = true;
		ErrorCode code = ctrl->calibrate(this->side);
		state->work = false;
		return code;
	}

	ArcTaskStep::ArcTaskStep(motor_point_t dest, motor_point_t center, int sp,
	                         float speed, bool rel)
	    : TaskStep::TaskStep() {
		this->dest = dest;
		this->center = center;
		this->splitter = sp;
		this->speed = speed;
		this->rel = rel;
		this->clockwise = false;
	}

	ErrorCode ArcTaskStep::perform(std::shared_ptr<CoordPlane> ctrl,
	                               TaskParameters &prms, SystemManager &sysman,
	                               std::shared_ptr<TaskState> state) {
		ErrorCode code;
		state->plane = ctrl;
		state->work = true;
		if (this->rel) {
			code = ctrl->relativeArc(dest, center, splitter, this->speed * prms.speed,
			                         this->clockwise);
		} else {
			code = ctrl->arc(dest, center, splitter, this->speed * prms.speed,
			                 this->clockwise);
		}
		state->work = false;
		return code;
	}

	motor_point_t ArcTaskStep::getDestination() const {
		return this->dest;
	}

	motor_point_t ArcTaskStep::getCenter() const {
		return this->center;
	}

	int ArcTaskStep::getSplitter() const {
		return this->splitter;
	}

	float ArcTaskStep::getSpeed() const {
		return this->speed;
	}

	bool ArcTaskStep::isClockwise() const {
		return this->clockwise;
	}

	bool ArcTaskStep::isRelative() const {
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

	RelArcTaskStep::RelArcTaskStep(motor_point_t dest, motor_point_t center,
	                               int sp, float speed, bool rel)
	    : TaskStep::TaskStep() {
		this->dest = dest;
		this->center = center;
		this->splitter = sp;
		this->speed = speed;
		this->rel = rel;
		this->clockwise = false;
	}

	ErrorCode RelArcTaskStep::perform(std::shared_ptr<CoordPlane> ctrl,
	                                  TaskParameters &prms, SystemManager &sysman,
	                                  std::shared_ptr<TaskState> state) {
		motor_point_t cen = ctrl->getPosition();
		cen.x += center.x;
		cen.y += center.y;
		ErrorCode code;
		state->plane = ctrl;
		state->work = true;
		if (this->rel) {
			code = ctrl->relativeArc(dest, cen, splitter, this->speed * prms.speed,
			                         this->clockwise);
		} else {
			code = ctrl->arc(dest, cen, splitter, this->speed * prms.speed,
			                 this->clockwise);
		}
		state->work = false;
		return code;
	}

	void RelArcTaskStep::setClockwise(bool c) {
		this->clockwise = c;
	}

	GraphCoordTask::GraphCoordTask(std::unique_ptr<GraphBuilder> graph,
	                               std::shared_ptr<CoordTranslator> trans,
	                               float scale)
	    : CoordTask::CoordTask(CoordTaskType::GraphTask),
	      graph(std::move(graph)),
	      trans(trans),
	      scale(scale) {}

	ErrorCode GraphCoordTask::perform(std::shared_ptr<CoordPlane> plane,
	                                  TaskParameters &prms, SystemManager &sysman,
	                                  std::shared_ptr<TaskState> state) {
		return this->graph->build(sysman, plane, *this->trans,
		                          this->scale * prms.speed, *state);
	}
}  // namespace CalX
