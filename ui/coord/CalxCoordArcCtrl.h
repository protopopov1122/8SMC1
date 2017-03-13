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


#ifndef CALX_UI_CALX_COORD_ARC_CTRL_H_
#define CALX_UI_CALX_COORD_ARC_CTRL_H_

#include "calx.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "CalxActionQueue.h"
#include "CalxFrame.h"

using namespace CalX;

namespace CalXUI {
	
	class CalxCoordCtrl; // Forward reference
	
	class CalxCoordArcCtrl : public wxPanel {
		public:
			CalxCoordArcCtrl(CalxCoordCtrl *ctrl, wxWindow *win, wxWindowID id)
				: wxPanel::wxPanel(win, id) {
				this->ctrl = ctrl;
				init();
			}
			int getCoordX() {return xCoord->GetValue();}
			int getCoordY() {return yCoord->GetValue();}
			int getCenterCoordX() {return cxCoord->GetValue();}
			int getCenterCoordY() {return cyCoord->GetValue();}
			int getSpeed() {return speed->GetValue();}
			int getDivisor() {return 1 << divisor->GetSelection();}
			int getSplitter() {return splitter->GetValue();}
			bool isClockwise() {return clockwise->GetValue();}
			bool isRelative() {return relative->GetValue();}
		private:
			void init();
			CalxCoordCtrl *ctrl;
			wxSpinCtrl *xCoord;
			wxSpinCtrl *yCoord;
			wxSpinCtrl *cxCoord;
			wxSpinCtrl *cyCoord;
			wxSpinCtrl *speed;
			wxChoice *divisor;
			wxSpinCtrl *splitter;
			wxCheckBox *clockwise;
			wxCheckBox *relative;
	};
	
}
	
#endif