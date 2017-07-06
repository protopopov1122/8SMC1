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

using namespace CalX;

namespace CalXUI {

	class CalxCoordCtrl; // Forward reference

	class CalxCoordOtherCtrl : public wxPanel {
		public:
			CalxCoordOtherCtrl(CalxCoordCtrl *ctrl, wxWindow *win, wxWindowID id)
				: wxPanel::wxPanel(win, id) {
				this->ctrl = ctrl;
				init();
			}
			bool isInstrumentEnabled() {return enableInstrument->GetValue();}
			TrailerId getTrailer() {return trailer->GetSelection()==0 ? TrailerId::Trailer1 : TrailerId::Trailer2;}
			TrailerId getMeasureTrailer() {return measureTrailer->GetSelection()==0 ? TrailerId::Trailer1 : TrailerId::Trailer2;}
			bool isLoggingActions() {return logActions->GetValue();}
			bool isLoggingErrors() {return logActions->GetValue();}
			int getXOffset() {return xOffset->GetValue();}
			int getYOffset() {return yOffset->GetValue();}
            void setXOffset(motor_coord_t x) {
                xOffset->SetValue((int) x);
			}
            void setYOffset(motor_coord_t y) {
                yOffset->SetValue((int) y);
			}
			double getXScale() {
				return xScale->GetValue();
			}
			double getYScale() {
				return yScale->GetValue();
			}
			void setXScale(double xs) {
				xScale->SetValue(xs);
			}
			void setYScale(double ys) {
				yScale->SetValue(ys);
			}
			int getMinX() {return minx->GetValue();}
			int getMinY() {return miny->GetValue();}
			int getMaxX() {return maxx->GetValue();}
			int getMaxY() {return maxy->GetValue();}
			int getSpeed() {return speed->GetValue();}
			CalxCoordPositionCtrl *getPositionController() {return this->posCtrl;}
		private:
			void init();
			CalxCoordCtrl *ctrl;
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
	};

}

#endif
