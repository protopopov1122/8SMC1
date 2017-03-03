#ifndef _8SMC1_UI_CALX_COORD_CTRL_H_
#define _8SMC1_UI_CALX_COORD_CTRL_H_

#include "calx.h"
#include "CalxActionQueue.h"
#include "ctrl-lib/SystemManager.h"
#include <wx/stattext.h>

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
	
	class CalxCoordCtrl : public wxPanel {
		public:
			friend class CalxCoordEventListener;
			CalxCoordCtrl(wxWindow*, wxWindowID, CoordHandle*);
			
			void updateUI();
		private:
			void OnExit(wxCloseEvent&);
			void OnQueueUpdate(wxThreadEvent&);
			
			CoordHandle *ctrl;
			CoordEventListener *listener;
			CalxActionQueue *queue;
			
			// Static text
			wxStaticText *generalInfoText;
			
			
	};
}

#endif