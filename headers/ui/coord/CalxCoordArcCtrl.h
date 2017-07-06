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

#include "ui/calx.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "ui/CalxActionQueue.h"
#include "ui/CalxFrame.h"

using namespace CalX;

namespace CalXUI {

  class CalxCoordCtrl;  // Forward reference

  class CalxCoordArcCtrl : public wxPanel {
   public:
	CalxCoordArcCtrl(wxWindow *win, wxWindowID id) : wxPanel::wxPanel(win, id) {
	  init();
	}
	double getCoordX() {
	  return xCoord->GetValue();
	}
	double getCoordY() {
	  return yCoord->GetValue();
	}
	double getCenterCoordX() {
	  return cxCoord->GetValue();
	}
	double getCenterCoordY() {
	  return cyCoord->GetValue();
	}
	double getSpeed() {
	  return speed->GetValue();
	}
	int getSplitter() {
	  return splitter->GetValue();
	}
	bool isClockwise() {
	  return clockwise->GetValue();
	}
	bool isRelative() {
	  return relative->GetValue();
	}
	wxButton *getMoveButton() {
	  return this->moveButton;
	}

   private:
	void init();
	wxButton *moveButton;
	wxSpinCtrlDouble *xCoord;
	wxSpinCtrlDouble *yCoord;
	wxSpinCtrlDouble *cxCoord;
	wxSpinCtrlDouble *cyCoord;
	wxSpinCtrlDouble *speed;
	wxSpinCtrl *splitter;
	wxCheckBox *clockwise;
	wxCheckBox *relative;
  };
}

#endif
