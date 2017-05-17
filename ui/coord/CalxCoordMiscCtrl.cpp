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


#include "CalxApp.h"
#include "CalxErrorHandler.h"
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
	
	CalxCoordMotorListener::CalxCoordMotorListener(CalxCoordCtrl *ctrl) {
		this->ctrl = ctrl;
	}
	
	CalxCoordMotorListener::~CalxCoordMotorListener() {
	}
	
	void CalxCoordMotorListener::use() {
		ctrl->use();
	}
	
	void CalxCoordMotorListener::unuse() {
		ctrl->unuse();
	}
	
	CalxCoordInstrumentListener::CalxCoordInstrumentListener(CalxCoordCtrl *ctrl) {
		this->ctrl = ctrl;
	}
	
	CalxCoordInstrumentListener::~CalxCoordInstrumentListener() {
		
	}
	
	void CalxCoordInstrumentListener::use() {
		ctrl->use();
	}
	
	void CalxCoordInstrumentListener::unuse() {
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
		handle->open_session();
		this->ctrl->setMaster(true);
		if (relative) {
			wxGetApp().getErrorHandler()->handle(handle->relativeMove(dest, speed, div, jump));
		} else {
			wxGetApp().getErrorHandler()->handle(handle->move(dest, speed, div, jump));
		}
		this->ctrl->setMaster(false);
		handle->close_session();
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
		handle->open_session();
		ctrl->setMaster(true);
		if (relative) {
			wxGetApp().getErrorHandler()->handle(handle->relativeArc(dest, cen, splitter, speed, div, clockwise));
		} else {
			wxGetApp().getErrorHandler()->handle(handle->arc(dest, cen, splitter, speed, div, clockwise));
		}
		ctrl->setMaster(false);
		handle->close_session();
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
		handle->open_session();
		ctrl->setMaster(true);
		wxGetApp().getErrorHandler()->handle(builder->build(sysman, handle, translator, speed, &state));
		ctrl->setMaster(false);
		handle->close_session();
	}
			
	void CalxCoordGraphAction::stop() {
		state.stop();
	}
	
	CalxCoordPreviewAction::CalxCoordPreviewAction(CalxCoordCtrl *ctrl, CalxVirtualPlaneDialog *dialog, CoordTranslator *trans, GraphBuilder *builder, float speed) {
		this->ctrl = ctrl;
		this->dialog = dialog;
		this->translator = trans;
		this->builder = builder;
		this->speed = speed;
	}
	
	CalxCoordPreviewAction::~CalxCoordPreviewAction() {
		delete this->builder;	
	}
			
	void CalxCoordPreviewAction::perform(SystemManager *sysman) {
		ctrl->setMaster(true);
		wxGetApp().getErrorHandler()->handle(builder->build(sysman, dialog->getPlane(), translator, speed, &state));
		dialog->Refresh();
		dialog->Enable(true);
		ctrl->setMaster(false);
	}
			
	void CalxCoordPreviewAction::stop() {
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
		handle->open_session();
		this->ctrl->setMaster(true);
		wxGetApp().getErrorHandler()->handle(handle->calibrate(trailer));
		this->ctrl->setMaster(false);
		handle->close_session();
	}
			
	void CalxCoordCalibrateAction::stop() {
		handle->stop();
	}
	
	CalxCoordMeasureAction::CalxCoordMeasureAction(CalxCoordCtrl *ctrl, CoordHandle *handle, TrailerId tr) {
		this->ctrl = ctrl;
		this->handle = handle;
		this->trailer = tr;
	}
	
	CalxCoordMeasureAction::~CalxCoordMeasureAction() {
		
	}
			
	void CalxCoordMeasureAction::perform(SystemManager *sysman) {
		handle->open_session();
		this->ctrl->setMaster(true);
		wxGetApp().getErrorHandler()->handle(handle->getController()->measure(trailer));
		this->ctrl->setMaster(false);
		handle->close_session();
	}
			
	void CalxCoordMeasureAction::stop() {
		handle->stop();
	}
}