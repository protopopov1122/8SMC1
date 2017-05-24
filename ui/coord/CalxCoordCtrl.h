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


#ifndef CALX_UI_CALX_COORD_CTRL_H_
#define CALX_UI_CALX_COORD_CTRL_H_

#include "calx.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/collpane.h>
#include "CalxActionQueue.h"
#include "CalxFrame.h"
#include "CalxCoordLinearCtrl.h"
#include "CalxCoordArcCtrl.h"
#include "CalxCoordGraphCtrl.h"
#include "CalxCoordOtherCtrl.h"
#include "CalxCoordMiscCtrl.h"

using namespace CalX;

namespace CalXUI {
	
	class CalxCoordPlaneWatcher; // Forward referencing
	
	wxDECLARE_EVENT(wxEVT_COORD_CTRL_WATCHER, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_COORD_CTRL_ENABLE, wxThreadEvent);
	
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
			void OnGraphPreviewClick(wxCommandEvent&);
			void OnCalibrateClick(wxCommandEvent&);
			void OnMeasureClick(wxCommandEvent&);
			void OnUpdateFiltersClick(wxCommandEvent&);
			void OnInstrumentEnableClick(wxCommandEvent&);
			void OnInterfaceUpdate(wxCollapsiblePaneEvent&);
			void OnPositionChangeClick(wxCommandEvent&);
			void OnConfigureClick(wxCommandEvent&);
			void use();
			void unuse();
			bool isUsed();
			void setMaster(bool);
			void setEnabled(bool);
			void bindWatcher(CalxCoordPlaneWatcher*);
			void unbindWatcher(CalxCoordPlaneWatcher*);
			bool hasWatchers();
			bool isBusy() {
				return !queue->isEmpty();
			}
			void setPlaneOffset(motor_point_t);
			
			// Request synchronizing with interface
			void requestMeasure(TrailerId);
			void requestPosition(double, double);
			void requestPositionAbs(motor_point_t);
			void requestCenter();
			void requestInvert();
			void requestWatcher();
		private:
			void OnExit(wxCloseEvent&);
			void OnQueueUpdate(wxThreadEvent&);
			void OnStopClick(wxCommandEvent&);
			void OnWatcherClick(wxCommandEvent&);
			void OnWatcherRequest(wxThreadEvent&);
			void OnEnableEvent(wxThreadEvent&);
			
			CoordHandle *ctrl;
			CoordEventListener *listener;
			CalxCoordMotorListener *xListener;
			CalxCoordMotorListener *yListener;
			CalxCoordInstrumentListener *instrListener;
			CalxActionQueue *queue;
			int used;
			bool master;
			std::vector<CalxCoordPlaneWatcher*> watchers;
			
			
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
			CoordPlaneMap *unit_map;
			
	};
}

#endif
