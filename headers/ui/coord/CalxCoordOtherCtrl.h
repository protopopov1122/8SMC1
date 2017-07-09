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

#ifndef CALX_UI_CALX_COORD_OTHER_CTRL_H_
#define CALX_UI_CALX_COORD_OTHER_CTRL_H_

#include <math.h>
#include "ui/calx.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/collpane.h>
#include "ui/CalxActionQueue.h"
#include "ui/CalxFrame.h"
#include "ui/coord/CalxCoordPositionCtrl.h"
#include "ui/coord/CalxCoordController.h"
#include "ui/coord/CalxFilterController.h"

using namespace CalX;

namespace CalXUI {

  class CalxCoordOtherCtrl : public wxPanel {
   public:
	CalxCoordOtherCtrl(wxWindow *, wxWindowID, CalxCoordController *,
					   CalxFilterController *);

	wxCollapsiblePane *getPositionPane();
	wxCollapsiblePane *getFiltersPane();

	void setOffset(motor_point_t);
	void setScale(motor_scale_t);

   private:
	void OnInstrumentEnableClick(wxCommandEvent &);
	void OnAdjustPositionClick(wxCommandEvent &);
	void OnCalibrateClick(wxCommandEvent &);
	void OnMeasureClick(wxCommandEvent &);
	void OnFiltersUpdate(wxCommandEvent &);

	wxCollapsiblePane *posCollPane;
	wxCollapsiblePane *filtersCollPane;
	wxCheckBox *enableInstrument;
	wxChoice *trailer;
	wxChoice *measureTrailer;
	wxCheckBox *logActions;
	wxCheckBox *logErrors;
	wxSpinCtrl *xOffset;
	wxSpinCtrl *yOffset;
	wxSpinCtrlDouble *xScale;
	wxSpinCtrlDouble *yScale;
	wxSpinCtrl *minx;
	wxSpinCtrl *miny;
	wxSpinCtrl *maxx;
	wxSpinCtrl *maxy;
	wxSpinCtrl *speed;

	CalxCoordPositionCtrl *posCtrl;
	CalxCoordController *controller;
	CalxFilterController *filters;
  };
}

#endif
