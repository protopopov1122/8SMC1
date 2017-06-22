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


#ifndef CALX_UI_CALX_COORD_LINEAR_CTRL_H_
#define CALX_UI_CALX_COORD_LINEAR_CTRL_H_

#include "ui/calx.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "ui/CalxActionQueue.h"
#include "ui/CalxFrame.h"

using namespace CalX;

namespace CalXUI {

	class CalxCoordCtrl; // Forward reference

	class CalxCoordLinearCtrl : public wxPanel {
		public:
			CalxCoordLinearCtrl(wxWindow *win, wxWindowID id)
				: wxPanel::wxPanel(win, id) {
				init();
			}
			double getCoordX() {return xCoord->GetValue();}
			double getCoordY() {return yCoord->GetValue();}
			double getSpeed() {return speed->GetValue();}
			int getDivisor() {return 1 << divisor->GetSelection();}
			bool isRelative() {return relative->GetValue();}
			wxButton *getMoveButton() {return this->moveButton;}
			wxButton *getJumpButton() {return this->jumpButton;}
		private:
			void init();
			wxButton *moveButton;
			wxButton *jumpButton;
			wxSpinCtrlDouble *xCoord;
			wxSpinCtrlDouble *yCoord;
			wxSpinCtrlDouble *speed;
			wxChoice *divisor;
			wxCheckBox *relative;
	};

}

#endif
