#ifndef CALX_UI_CALX_COORD_GRAPH_CTRL_H_
#define CALX_UI_CALX_COORD_GRAPH_CTRL_H_

#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "calx.h"
#include "CalxActionQueue.h"
#include "CalxFrame.h"
#include "CalxCoordTranslator.h"

using namespace CalX;

namespace CalXUI {
	
	class CalxCoordCtrl; // Forward reference
	
	class CalxCoordGraphCtrl : public wxPanel {
		public:
			CalxCoordGraphCtrl(CalxCoordCtrl *ctrl, wxWindow *win, wxWindowID id)
				: wxPanel::wxPanel(win, id) {
				this->ctrl = ctrl;
				init();
			}
			std::string getExpression() {return expr->GetValue().ToStdString();}
			int getXMin() {return xmin->GetValue();}
			int getXMax() {return xmax->GetValue();}
			int getYMin() {return ymin->GetValue();}
			int getYMax() {return ymax->GetValue();}
			double getStep() {
				double val;
				step->GetValue().ToDouble(&val);
				return val;
			}
			CoordTranslator *getCoordTranslator() {
				return this->translator->getTranslator();
			}
			int getSpeed() {return speed->GetValue();}
		private:
			void init();
			CalxCoordCtrl *ctrl;
			wxTextCtrl *expr;
			wxSpinCtrl *xmin;
			wxSpinCtrl *xmax;
			wxSpinCtrl *ymin;
			wxSpinCtrl *ymax;
			wxTextCtrl *step;
			CalxCoordTranslator *translator;
			wxSpinCtrl *speed;
	};
	
}
	
#endif