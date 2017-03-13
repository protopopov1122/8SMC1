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

#include "calx.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "CalxActionQueue.h"
#include "CalxFrame.h"
#include "CalxCoordLinearCtrl.h"
#include "CalxCoordArcCtrl.h"
#include "CalxCoordGraphCtrl.h"
#include "CalxCoordOtherCtrl.h"

using namespace CalX;

namespace CalXUI {
	
	class CalxCoordCtrl; // Forward reference
	
	
	class CalxCoordEventListener : public CoordEventListener {
		public:
			CalxCoordEventListener(CalxCoordCtrl*);
			virtual ~CalxCoordEventListener();
			virtual void use();
			virtual void unuse();
		private:
			CalxCoordCtrl *ctrl;
	};
	
	class CalxCoordDeviceListener : public MotorEventListener {
		public:
			CalxCoordDeviceListener(CalxCoordCtrl*);
			virtual ~CalxCoordDeviceListener();
			virtual void use();
			virtual void unuse();
		private:
			CalxCoordCtrl *ctrl;
	};
	
	
	class CalxCoordTimer : public wxTimer {
		public:
			CalxCoordTimer() : wxTimer::wxTimer() {
			}
			~CalxCoordTimer() {}
			void setCtrl(CalxCoordCtrl *c) {this->ctrl = c;}
			virtual void Notify();
		private:
			CalxCoordCtrl *ctrl;
	};
			
	
	class CalxCoordMoveAction : public CalxAction {
		public:
			CalxCoordMoveAction(CalxCoordCtrl*, CoordHandle*, bool, bool, motor_point_t, float, int);
			virtual ~CalxCoordMoveAction();
			
			virtual void perform(SystemManager*);
			virtual void stop();
		private:
			CalxCoordCtrl *ctrl;
			CoordHandle *handle;
			bool jump;
			bool relative;
			motor_point_t dest;
			float speed;
			int div;
	};
			
	class CalxCoordArcAction : public CalxAction {
		public:
			CalxCoordArcAction(CalxCoordCtrl*, CoordHandle*, bool, motor_point_t, motor_point_t, int, float, int, bool);
			virtual ~CalxCoordArcAction();
			
			virtual void perform(SystemManager*);
			virtual void stop();
		private:
			CalxCoordCtrl *ctrl;
			CoordHandle *handle;
			bool relative;
			motor_point_t dest;
			motor_point_t cen;
			int splitter;
			float speed;
			int div;
			bool clockwise;
	};
			
	class CalxCoordGraphAction : public CalxAction {
		public:
			CalxCoordGraphAction(CalxCoordCtrl*, CoordHandle*, CoordTranslator*, GraphBuilder*, float);
			virtual ~CalxCoordGraphAction();
			
			virtual void perform(SystemManager*);
			virtual void stop();
		private:
			CalxCoordCtrl *ctrl;
			CoordHandle *handle;
			CoordTranslator *translator;
			GraphBuilder *builder;
			float speed;
			TaskState state;
	};
			
	class CalxCoordCalibrateAction : public CalxAction {
		public:
			CalxCoordCalibrateAction(CalxCoordCtrl*, CoordHandle*, TrailerId);
			virtual ~CalxCoordCalibrateAction();
			
			virtual void perform(SystemManager*);
			virtual void stop();
		private:
			CalxCoordCtrl *ctrl;
			CoordHandle *handle;
			TrailerId trailer;
	};
}

#endif