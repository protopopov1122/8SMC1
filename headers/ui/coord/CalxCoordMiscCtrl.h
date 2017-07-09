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
#include "ui/coord/CalxVirtualPlane.h"

using namespace CalX;

namespace CalXUI {

  class CalxCoordPane;  // Forward reference

  class CalxCoordEventListener : public CoordEventListener {
   public:
	CalxCoordEventListener(CalxCoordPane *);
	virtual ~CalxCoordEventListener();
	virtual void use();
	virtual void unuse();

   private:
	CalxCoordPane *ctrl;
  };

  class CalxCoordMotorListener : public MotorEventListener {
   public:
	CalxCoordMotorListener(CalxCoordPane *);
	virtual ~CalxCoordMotorListener();
	virtual void use();
	virtual void unuse();

   private:
	CalxCoordPane *ctrl;
  };

  class CalxCoordInstrumentListener : public InstrumentEventListener {
   public:
	CalxCoordInstrumentListener(CalxCoordPane *);
	virtual ~CalxCoordInstrumentListener();
	virtual void use();
	virtual void unuse();

   private:
	CalxCoordPane *ctrl;
  };

  class CalxCoordTimer : public wxTimer {
   public:
	CalxCoordTimer() : wxTimer::wxTimer(), ctrl(nullptr) {}
	~CalxCoordTimer() {}
	void setCtrl(CalxCoordPane *c) {
	  this->ctrl = c;
	}
	virtual void Notify();

   private:
	CalxCoordPane *ctrl;
  };
}

#endif
