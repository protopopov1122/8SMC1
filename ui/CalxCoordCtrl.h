#ifndef _8SMC1_UI_CALX_COORD_CTRL_H_
#define _8SMC1_UI_CALX_COORD_CTRL_H_

#include "calx.h"
#include "CalxActionQueue.h"
#include "ctrl-lib/SystemManager.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>

using namespace _8SMC1;

namespace CalX {
	
	class CalxCoordCtrl; // Forward reference
	
	class CalxCoordEventListener : public CoordEventListener {
		public:
			CalxCoordEventListener(CalxCoordCtrl*);
			virtual ~CalxCoordEventListener();
			virtual void moving(CoordMoveEvent&);
			virtual void moved(CoordMoveEvent&);
			virtual void stopped(CoordErrorEvent&);
			virtual void calibrating(CoordCalibrateEvent&);
			virtual void calibrated(CoordCalibrateEvent&);
		private:
			CalxCoordCtrl *ctrl;
	};
	
	
	
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
			int getXOffset() {return xoffset->GetValue();}
			int getYOffset() {return yoffset->GetValue();}
			int getXScale() {return xscale->GetValue();}
			int getYScale() {return yscale->GetValue();}
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
			wxSpinCtrl *xoffset;
			wxSpinCtrl *yoffset;
			wxSpinCtrl *xscale;
			wxSpinCtrl *yscale;
			wxSpinCtrl *speed;
	};
	
	class CalxCoordCtrl : public wxScrolledWindow {
		public:
			friend class CalxCoordEventListener;
			CalxCoordCtrl(wxWindow*, wxWindowID, CoordHandle*);
			
			void updateUI();
			
			void OnLinearMoveClick(wxCommandEvent&);
			void OnLinearJumpClick(wxCommandEvent&);
			void OnArcMoveClick(wxCommandEvent&);
			void OnGraphBuildClick(wxCommandEvent&);
		private:
			void OnExit(wxCloseEvent&);
			void OnQueueUpdate(wxThreadEvent&);
			
			CoordHandle *ctrl;
			CoordEventListener *listener;
			CalxActionQueue *queue;
			
			// Components
			wxStaticText *generalInfoText;
			CalxCoordLinearCtrl *linear;
			CalxCoordArcCtrl *arc;
			CalxCoordGraphCtrl *graphCtrl;
			
	};
}

#endif