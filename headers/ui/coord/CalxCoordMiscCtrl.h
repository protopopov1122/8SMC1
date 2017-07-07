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

#ifndef CALX_UI_CALX_COORD_MISC_CTRL_H_
#define CALX_UI_CALX_COORD_MISC_CTRL_H_

#include "ui/calx.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "ui/CalxActionQueue.h"
#include "ui/CalxFrame.h"
#include "ui/coord/CalxCoordLinearCtrl.h"
#include "ui/coord/CalxCoordArcCtrl.h"
#include "ui/coord/CalxCoordGraphCtrl.h"
#include "ui/coord/CalxCoordOtherCtrl.h"
#include "ui/coord/CalxVirtualPlane.h"

using namespace CalX;

namespace CalXUI {

  class CalxCoordCtrl;  // Forward reference

  class CalxCoordEventListener : public CoordEventListener {
   public:
	CalxCoordEventListener(CalxCoordCtrl *);
	virtual ~CalxCoordEventListener();
	virtual void use();
	virtual void unuse();

   private:
	CalxCoordCtrl *ctrl;
  };

  class CalxCoordMotorListener : public MotorEventListener {
   public:
	CalxCoordMotorListener(CalxCoordCtrl *);
	virtual ~CalxCoordMotorListener();
	virtual void use();
	virtual void unuse();

   private:
	CalxCoordCtrl *ctrl;
  };

  class CalxCoordInstrumentListener : public InstrumentEventListener {
   public:
	CalxCoordInstrumentListener(CalxCoordCtrl *);
	virtual ~CalxCoordInstrumentListener();
	virtual void use();
	virtual void unuse();

   private:
	CalxCoordCtrl *ctrl;
  };

  class CalxCoordTimer : public wxTimer {
   public:
	CalxCoordTimer() : wxTimer::wxTimer(), ctrl(nullptr) {}
	~CalxCoordTimer() {}
	void setCtrl(CalxCoordCtrl *c) {
	  this->ctrl = c;
	}
	virtual void Notify();

   private:
	CalxCoordCtrl *ctrl;
  };

  class CalxCoordPreviewAction : public CalxAction {
   public:
	CalxCoordPreviewAction(CalxCoordCtrl *, CalxVirtualPlaneDialog *,
						   CoordTranslator *, GraphBuilder *, float,
						   bool = false);
	virtual ~CalxCoordPreviewAction();

	virtual void perform(SystemManager *);
	virtual void stop();

   private:
	CalxCoordCtrl *ctrl;
	CalxVirtualPlaneDialog *dialog;
	CoordTranslator *translator;
	GraphBuilder *builder;
	float speed;
	TaskState state;
	bool use_float;
  };
}

#endif
