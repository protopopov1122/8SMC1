#ifndef _8SMC1_UI_CALX_COORD_CTRL_H_
#define _8SMC1_UI_CALX_COORD_CTRL_H_

#include "calx.h"
#include "CalxActionQueue.h"
#include "CalxFrame.h"
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
	
	class CalxCoordCtrl : public wxScrolledWindow {
		public:
			friend class CalxCoordEventListener;
			CalxCoordCtrl(wxWindow*, wxWindowID, CoordHandle*);
			
			void updateUI();
			CoordHandle *getHandle() {return this->ctrl;}
			CoordPlaneLog *getPlaneLog();
			CoordPlaneMap *getPlaneMap();
			CoordPlaneValidator *getPlaneValidator();
			
			void OnLinearMoveClick(wxCommandEvent&);
			void OnLinearJumpClick(wxCommandEvent&);
			void OnArcMoveClick(wxCommandEvent&);
			void OnGraphBuildClick(wxCommandEvent&);
			void OnCalibrateClick(wxCommandEvent&);
			void OnUpdateFiltersClick(wxCommandEvent&);
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
			CalxCoordOtherCtrl *otherCtrl;
			
			// Filters
			CoordPlaneLog *log;
			CoordPlaneMap *map;
			CoordPlaneValidator *validator;
			
	};
}

#endif