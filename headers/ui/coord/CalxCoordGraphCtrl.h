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

#ifndef CALX_UI_CALX_COORD_GRAPH_CTRL_H_
#define CALX_UI_CALX_COORD_GRAPH_CTRL_H_

#include "ui/calx.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "ui/CalxActionQueue.h"
#include "ui/CalxFrame.h"
#include "ui/coord/CalxCoordFilter.h"
#include "ui/coord/CalxCoordController.h"

using namespace CalX;

namespace CalXUI {

  class CalxCoordCtrl;  // Forward reference

  class CalxCoordGraphCtrl : public wxPanel {
   public:
	CalxCoordGraphCtrl(wxWindow*, wxWindowID, CalxCoordController*);

   private:
	void OnClose(wxCloseEvent &);
	void OnBuildClick(wxCommandEvent&);
	void OnPreviewClick(wxCommandEvent&);
	
	wxTextCtrl *expr;
	wxSpinCtrlDouble *xmin;
	wxSpinCtrlDouble *xmax;
	wxSpinCtrlDouble *ymin;
	wxSpinCtrlDouble *ymax;
	wxSpinCtrlDouble *step;
	CalxCoordFilter *translator;
	ComplexCoordTranslator *trans;
	wxSpinCtrlDouble *speed;
	
	CalxCoordController *controller;
  };
}

#endif
