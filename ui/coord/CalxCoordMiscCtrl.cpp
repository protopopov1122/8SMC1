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

#include "ui/CalxApp.h"
#include "ui/CalxErrorHandler.h"
#include "ui/coord/CalxCoordCtrl.h"

namespace CalXUI {

  void CalxCoordTimer::Notify() {
	ctrl->updateUI();
  }

  CalxCoordEventListener::CalxCoordEventListener(CalxCoordCtrl *ctrl) {
	this->ctrl = ctrl;
  }

  CalxCoordEventListener::~CalxCoordEventListener() {}

  void CalxCoordEventListener::use() {
	ctrl->use();
  }

  void CalxCoordEventListener::unuse() {
	ctrl->unuse();
  }

  CalxCoordMotorListener::CalxCoordMotorListener(CalxCoordCtrl *ctrl) {
	this->ctrl = ctrl;
  }

  CalxCoordMotorListener::~CalxCoordMotorListener() {}

  void CalxCoordMotorListener::use() {
	ctrl->use();
  }

  void CalxCoordMotorListener::unuse() {
	ctrl->unuse();
  }

  CalxCoordInstrumentListener::CalxCoordInstrumentListener(
	  CalxCoordCtrl *ctrl) {
	this->ctrl = ctrl;
  }

  CalxCoordInstrumentListener::~CalxCoordInstrumentListener() {}

  void CalxCoordInstrumentListener::use() {
	ctrl->use();
  }

  void CalxCoordInstrumentListener::unuse() {
	ctrl->unuse();
  }

  CalxCoordPreviewAction::CalxCoordPreviewAction(CalxCoordCtrl *ctrl,
												 CalxVirtualPlaneDialog *dialog,
												 CoordTranslator *trans,
												 GraphBuilder *builder,
												 float speed, bool use_float) {
	this->ctrl = ctrl;
	this->dialog = dialog;
	this->translator = trans;
	this->builder = builder;
	this->speed = speed;
	this->use_float = use_float;
	this->state.plane = nullptr;
	this->state.work = false;
  }

  CalxCoordPreviewAction::~CalxCoordPreviewAction() {
	delete this->builder;
  }

  void CalxCoordPreviewAction::perform(SystemManager *sysman) {
	ctrl->setMaster(true);
	dialog->Enable(false);
	if (this->use_float) {
	  wxGetApp().getErrorHandler()->handle(builder->floatBuild(
		  sysman, dialog->getFloatPlane(), translator, speed, &state));
	} else {
	  wxGetApp().getErrorHandler()->handle(builder->build(
		  sysman, dialog->getPlane(), translator, speed, &state));
	}
	dialog->Refresh();
	dialog->Enable(true);
	ctrl->setMaster(false);
  }

  void CalxCoordPreviewAction::stop() {
	state.stop();
  }
}
