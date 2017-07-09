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

#include "ui/CalxApp.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/collpane.h>
#include "ui/CalxActionQueue.h"
#include "ui/coord/CalxCoordLinearCtrl.h"
#include "ui/coord/CalxCoordArcCtrl.h"
#include "ui/coord/CalxCoordGraphCtrl.h"
#include "ui/coord/CalxCoordOtherCtrl.h"
#include "ui/coord/CalxCoordMiscCtrl.h"
#include "ui/coord/CalxCoordController.h"
#include "ui/coord/CalxWatcherPool.h"

using namespace CalX;

namespace CalXUI {

  wxDECLARE_EVENT(wxEVT_COORD_CTRL_WATCHER, wxThreadEvent);
  wxDECLARE_EVENT(wxEVT_COORD_CTRL_ENABLE, wxThreadEvent);

  class CalxCoordCtrl : public wxScrolledWindow, public CalxFilterController {
   public:
	CalxCoordCtrl(wxWindow *, wxWindowID, CoordHandle *);

	void updateUI();
	void shutdown();
	CoordHandle *getHandle();
	CalxWatcherPool *getWatchers();
	CalxCoordController *getController();

	bool isBusy();
	void use();
	void unuse();
	bool isUsed();
	virtual void setOffset(motor_point_t);
	virtual void setScale(motor_scale_t);
	void setEnabled(bool);

   private:
	void OnExit(wxCloseEvent &);
	void OnStopClick(wxCommandEvent &);
	void OnAdjusterClick(wxCommandEvent &);
	void OnWatcherClick(wxCommandEvent &);
	void OnWatcherRequest(wxThreadEvent &);
	void OnEnableEvent(wxThreadEvent &);
	void OnInterfaceUpdate(wxCollapsiblePaneEvent &);

	CalxCoordController *controller;
	CoordHandle *ctrl;
	CalxWatcherPool *watchers;
	CoordEventListener *listener;
	CalxCoordMotorListener *xListener;
	CalxCoordMotorListener *yListener;
	CalxCoordInstrumentListener *instrListener;
	CalxActionQueue *queue;

	int used;

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
  };
}

#endif
