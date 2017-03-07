#ifndef _8SMC1_UI_CALX_COORD_LINEAR_CTRL_H_
#define _8SMC1_UI_CALX_COORD_LINEAR_CTRL_H_

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
	
	class CalxCoordLinearCtrl : public wxPanel {
		public:
			CalxCoordLinearCtrl(CalxCoordCtrl *ctrl, wxWindow *win, wxWindowID id)
				: wxPanel::wxPanel(win, id) {
				this->ctrl = ctrl;
				init();
			}
			int getCoordX() {return xCoord->GetValue();}
			int getCoordY() {return yCoord->GetValue();}
			int getSpeed() {return speed->GetValue();}
			int getDivisor() {return 1 << divisor->GetSelection();}
			bool isRelative() {return relative->GetValue();}
		private:
			void init();
			CalxCoordCtrl *ctrl;
			wxSpinCtrl *xCoord;
			wxSpinCtrl *yCoord;
			wxSpinCtrl *speed;
			wxChoice *divisor;
			wxCheckBox *relative;
	};
	
}
	
#endif