#ifndef CALX_UI_CALX_COORD_CTRL_H_
#define CALX_UI_CALX_COORD_CTRL_H_

#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include "calx.h"
#include "CalxActionQueue.h"
#include "CalxFrame.h"
#include "CalxCoordLinearCtrl.h"
#include "CalxCoordArcCtrl.h"
#include "CalxCoordGraphCtrl.h"
#include "CalxCoordOtherCtrl.h"
#include "CalxCoordMiscCtrl.h"

using namespace CalX;

namespace CalXUI {
	
	class CalxCoordCtrl : public wxScrolledWindow {
		public:
			friend class CalxCoordEventListener;
			CalxCoordCtrl(wxWindow*, wxWindowID, CoordHandle*);
			
			void updateUI();
			void stop();
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
			void use();
			void unuse();
			bool isUsed();
			void setMaster(bool);
			void setEnabled(bool);
		private:
			void OnExit(wxCloseEvent&);
			void OnQueueUpdate(wxThreadEvent&);
			void OnStopClick(wxCommandEvent&);
			
			CoordHandle *ctrl;
			CoordEventListener *listener;
			CalxCoordDeviceListener *xListener;
			CalxCoordDeviceListener *yListener;
			CalxActionQueue *queue;
			int used;
			bool master;
			
			
			// Components
			wxPanel *generalPanel;
			wxPanel *actionPanel;
			wxButton *stopButton;
			wxStaticText *generalInfoText;
			CalxCoordLinearCtrl *linear;
			CalxCoordArcCtrl *arc;
			CalxCoordGraphCtrl *graphCtrl;
			CalxCoordOtherCtrl *otherCtrl;
			CalxCoordTimer timer;
			
			// Filters
			CoordPlaneLog *log;
			CoordPlaneMap *map;
			CoordPlaneValidator *validator;
			
	};
}

#endif