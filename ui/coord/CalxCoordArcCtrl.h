#ifndef CALX_UI_CALX_COORD_ARC_CTRL_H_
#define CALX_UI_CALX_COORD_ARC_CTRL_H_

#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "calx.h"
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