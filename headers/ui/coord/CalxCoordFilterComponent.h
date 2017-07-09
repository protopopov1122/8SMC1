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

#ifndef CALX_UI_COORD_CALX_COORD_FILTER_CTRL_H_
#define CALX_UI_COORD_CALX_COORD_FILTER_CTRL_H_

#include <math.h>
#include "ui/CalxApp.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/collpane.h>
#include "ui/CalxActionQueue.h"
#include "ui/coord/CalxCoordController.h"
#include "ui/coord/CalxCoordComponent.h"

namespace CalXUI {

  class CalxCoordFilterComponentFactory : public CalxCoordComponentFactory {
   public:
	virtual CalxCoordComponent *newComponent(wxWindow *, CalxCoordController *);
  };

  class CalxCoordFilterComponent : public CalxCoordComponent,
								   public CalxCoordFilterListener {
   public:
	CalxCoordFilterComponent(wxWindow *, wxWindowID, CalxCoordController *);

	virtual void updateOffset(motor_point_t);
	virtual void updateScale(motor_scale_t);

   private:
	void OnFiltersUpdate(wxCommandEvent &);
	void OnExit(wxCloseEvent &);

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

	CalxCoordController *controller;
  };
}

#endif
