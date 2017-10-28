/*
        Copyright (c) 2017 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
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

#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/coord/CalxCoordComponent.h"
#include "ui/coord/CalxCoordController.h"
#include "ui/coord/CalxCoordPaneMisc.h"
#include "ui/coord/CalxWatcherPool.h"
#include "ui/coord/CalxPlaneList.h"
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/collpane.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

using namespace CalX;

namespace CalXUI {

	wxDECLARE_EVENT(wxEVT_COORD_CTRL_WATCHER, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_COORD_CTRL_ENABLE, wxThreadEvent);

	class CalxCoordPane : public wxScrolledWindow,
	                      public CalxCoordFilterListener,
	                      public CalxPlaneHandle {
	 public:
		CalxCoordPane(wxWindow *, wxWindowID, std::shared_ptr<CoordHandle>, size_t);

		void updateUI();
		void shutdown();
		std::shared_ptr<CoordHandle> getHandle();
		virtual CalxWatcherPool *getWatchers();
		virtual void newWatcher();
		virtual void stop();
		virtual CalxCoordController *getController();
		bool addComponent(std::string, CalxCoordComponentFactory *, size_t,
		                  bool = true);
		size_t getComponentPaneCount();

		bool isBusy();
		void use();
		void unuse();
		bool isUsed();
		virtual void updateOffset(motor_point_t);
		virtual void updateScale(motor_scale_t);
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
		std::shared_ptr<CoordHandle> ctrl;
		CalxWatcherPool *watchers;
		std::shared_ptr<CoordEventListener> listener;
		std::shared_ptr<CalxCoordMotorListener> xListener;
		std::shared_ptr<CalxCoordMotorListener> yListener;
		std::shared_ptr<CalxCoordInstrumentListener> instrListener;
		CalxActionQueue *queue;

		int used;

		// Components
		std::vector<CalxCoordComponent *> components;
		wxPanel *generalPanel;
		wxPanel *component_panel;
		std::vector<wxPanel *> component_panes;
		wxButton *adjusterButton;
		wxButton *stopButton;
		wxStaticText *generalInfoText;
		CalxCoordTimer timer;
	};
}  // namespace CalXUI

#endif
