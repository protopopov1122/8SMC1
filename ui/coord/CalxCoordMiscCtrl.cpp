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


#include "CalxCoordCtrl.h"

namespace CalXUI {
	
	void CalxCoordTimer::Notify() {
		ctrl->updateUI();
	}
	
	CalxCoordEventListener::CalxCoordEventListener(CalxCoordCtrl *ctrl) {
		this->ctrl = ctrl;
	}
	
	CalxCoordEventListener::~CalxCoordEventListener() {
	}
	
	void CalxCoordEventListener::use() {
		ctrl->use();
	}
	
	void CalxCoordEventListener::unuse() {
		ctrl->unuse();
	}
	
	CalxCoordDeviceListener::CalxCoordDeviceListener(CalxCoordCtrl *ctrl) {
		this->ctrl = ctrl;
	}
	
	CalxCoordDeviceListener::~CalxCoordDeviceListener() {
	}
	
	void CalxCoordDeviceListener::use() {
		ctrl->use();
	}
	
	void CalxCoordDeviceListener::unuse() {
		ctrl->unuse();
	}
		
	
	CalxCoordMoveAction::CalxCoordMoveAction(CalxCoordCtrl *ctrl, CoordHandle *handle, bool jump, bool relative, motor_point_t dest, float speed, int div) {
		this->ctrl = ctrl;
		this->handle = handle;
		this->jump = jump;
		this->relative = relative;
		this->dest = dest;
		this->speed = speed;
		this->div = div;
	}
	
	CalxCoordMoveAction::~CalxCoordMoveAction() {
		
	}
			
	void CalxCoordMoveAction::perform(SystemManager *sysman) {
		this->ctrl->setMaster(true);
		if (relative) {
			handle->relativeMove(dest, speed, div, jump);
		} else {
			handle->move(dest, speed, div, jump);
		}
		this->ctrl->setMaster(false);
	}
			
	void CalxCoordMoveAction::stop() {
		handle->stop();
	}
	
	
	
	CalxCoordArcAction::CalxCoordArcAction(CalxCoordCtrl *ctrl, CoordHandle *handle, bool relative, motor_point_t dest, motor_point_t cen, int splitter, float speed, int div, bool clockwise) {
		this->ctrl = ctrl;
		this->handle = handle;
		this->relative = relative;
		this->dest = dest;
		this->cen = cen;
		this->splitter = splitter;
		this->speed = speed;
		this->div = div;
		this->clockwise = clockwise;
	}
	
	CalxCoordArcAction::~CalxCoordArcAction() {
		
	}
			
	void CalxCoordArcAction::perform(SystemManager *sysman) {
		ctrl->setMaster(true);
		if (relative) {
			handle->relativeArc(dest, cen, splitter, speed, div, clockwise);
		} else {
			handle->arc(dest, cen, splitter, speed, div, clockwise);
		}
		ctrl->setMaster(false);
	}
			
	void CalxCoordArcAction::stop() {
		handle->stop();
	}
	
	CalxCoordGraphAction::CalxCoordGraphAction(CalxCoordCtrl *ctrl, CoordHandle *handle, CoordTranslator *trans, GraphBuilder *builder, float speed) {
		this->ctrl = ctrl;
		this->handle = handle;
		this->translator = trans;
		this->builder = builder;
		this->speed = speed;
	}
	
	CalxCoordGraphAction::~CalxCoordGraphAction() {
		delete this->builder;	
	}
			
	void CalxCoordGraphAction::perform(SystemManager *sysman) {
		ctrl->setMaster(true);
		builder->build(sysman, handle, translator, speed, &state);
		ctrl->setMaster(false);
	}
			
	void CalxCoordGraphAction::stop() {
		state.stop();
	}
	
	CalxCoordCalibrateAction::CalxCoordCalibrateAction(CalxCoordCtrl *ctrl, CoordHandle *handle, TrailerId tr) {
		this->ctrl = ctrl;
		this->handle = handle;
		this->trailer = tr;
	}
	
	CalxCoordCalibrateAction::~CalxCoordCalibrateAction() {
		
	}
			
	void CalxCoordCalibrateAction::perform(SystemManager *sysman) {
		this->ctrl->setMaster(true);
		handle->calibrate(trailer);
		this->ctrl->setMaster(false);
	}
			
	void CalxCoordCalibrateAction::stop() {
		handle->stop();
	}
}