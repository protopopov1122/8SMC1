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


#ifndef CALX_UI_CALX_COORD_POSITION_CTRL_H_
#define CALX_UI_CALX_COORD_POSITION_CTRL_H_

#include "CalxApp.h"
#include <wx/spinctrl.h>

namespace CalXUI {

	class CalxCoordPositionCtrl : public wxPanel {
		public:
			CalxCoordPositionCtrl(wxWindow*, wxWindowID);
			double getXPosition() {return this->xPos->GetValue();}
			double getYPosition() {return this->yPos->GetValue();}
			wxButton *getMoveButton() {return this->moveButton;}
			wxButton *getConfigureButton() {return this->configureButton;}
			int getSpeed() {return speed->GetValue();}
			int getDivisor() {return 1 << divisor->GetSelection();}
		private:
			wxSpinCtrlDouble *xPos;
			wxSpinCtrlDouble *yPos;
			wxSpinCtrl *speed;
			wxChoice *divisor;
			wxButton *moveButton;
			wxButton *configureButton;
	};
}

#endif