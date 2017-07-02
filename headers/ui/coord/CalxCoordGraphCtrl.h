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

using namespace CalX;

namespace CalXUI {

	class CalxCoordCtrl; // Forward reference

	class CalxCoordGraphCtrl : public wxPanel {
		public:
			CalxCoordGraphCtrl(wxWindow *win, wxWindowID id)
				: wxPanel::wxPanel(win, id) {
				init();
			}
			std::string getExpression() {return expr->GetValue().ToStdString();}
			double getXMin() {return xmin->GetValue();}
			double getXMax() {return xmax->GetValue();}
			double getYMin() {return ymin->GetValue();}
			double getYMax() {return ymax->GetValue();}
			double getStep() {return step->GetValue();}
			CoordTranslator *getCoordTranslator() {
				return this->translator->getTranslator();
			}
			double getSpeed() {return speed->GetValue();}
			wxButton *getBuildButton() {return this->buildButton;}
			wxButton *getPreviewButton() {return this->previewButton;}
			void OnClose(wxCloseEvent&);
		private:
			void init();
			wxTextCtrl *expr;
			wxSpinCtrlDouble *xmin;
			wxSpinCtrlDouble *xmax;
			wxSpinCtrlDouble *ymin;
			wxSpinCtrlDouble *ymax;
			wxSpinCtrlDouble *step;
			CalxCoordFilter *translator;
			ComplexCoordTranslator *trans;
			wxSpinCtrlDouble *speed;
			wxButton *buildButton;
			wxButton *previewButton;
	};

}

#endif
