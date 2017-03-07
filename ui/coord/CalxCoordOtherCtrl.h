#ifndef _8SMC1_UI_CALX_COORD_OTHER_CTRL_H_
#define _8SMC1_UI_CALX_COORD_OTHER_CTRL_H_

#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "calx.h"
#include "CalxActionQueue.h"
#include "CalxFrame.h"

using namespace _8SMC1;

namespace CalX {
	
	class CalxCoordCtrl; // Forward reference
	
	class CalxCoordOtherCtrl : public wxPanel {
		public:
			CalxCoordOtherCtrl(CalxCoordCtrl *ctrl, wxWindow *win, wxWindowID id)
				: wxPanel::wxPanel(win, id) {
				this->ctrl = ctrl;
				init();
			}
			TrailerId getTrailer() {return trailer->GetSelection()==0 ? TrailerId::Trailer1 : TrailerId::Trailer2;}
			bool isLoggingActions() {return logActions->GetValue();}
			bool isLoggingErrors() {return logActions->GetValue();}
			int getXOffset() {return xOffset->GetValue();}
			int getYOffset() {return yOffset->GetValue();}
			double getXScale() {
				double val;
				xScale->GetValue().ToDouble(&val);
				return val;
			}
			double getYScale() {
				double val;
				yScale->GetValue().ToDouble(&val);
				return val;
			}
			int getMinX() {return minx->GetValue();}
			int getMinY() {return miny->GetValue();}
			int getMaxX() {return maxx->GetValue();}
			int getMaxY() {return maxy->GetValue();}
			int getSpeed() {return speed->GetValue();}
		private:
			void init();
			CalxCoordCtrl *ctrl;
			wxChoice *trailer;
			wxCheckBox *logActions;
			wxCheckBox *logErrors;
			wxSpinCtrl *xOffset;
			wxSpinCtrl *yOffset;
			wxTextCtrl *xScale;
			wxTextCtrl *yScale;
			wxSpinCtrl *minx;
			wxSpinCtrl *miny;
			wxSpinCtrl *maxx;
			wxSpinCtrl *maxy;
			wxSpinCtrl *speed;
	};
	
}
	
#endif