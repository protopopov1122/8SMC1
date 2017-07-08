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

#ifndef CALX_UI_CALX_INSTRUMENT_CTRL_H_
#define CALX_UI_CALX_INSTRUMENT_CTRL_H_

#include "ui/CalxApp.h"
#include <wx/stattext.h>
#include <wx/timer.h>
#include <wx/choice.h>
#include "ui/CalxActionQueue.h"
#include "ui/dev/CalxDeviceHandle.h"
#include <iostream>

namespace CalXUI {

  wxDECLARE_EVENT(wxEVT_INSTRUMENT_CTRL_ENABLE, wxThreadEvent);

  class CalxInstrumentCtrl : public CalxDeviceHandle {
   public:
	CalxInstrumentCtrl(wxWindow *, wxWindowID, InstrumentController *);
	virtual void stop();
	virtual void updateUI();
	virtual bool isBusy();

   private:
	void OnExit(wxCloseEvent &);
	void OnEnableButton(wxCommandEvent &);
	void OnSessionSwitch(wxCommandEvent &);
	void OnModeClick(wxCommandEvent &);
	void OnConfClick(wxCommandEvent &);
	void OnEnableEvent(wxThreadEvent &);

	wxStaticText *infoText;
	wxStaticText *instrInfoText;
	wxButton *sessionButton;
	wxButton *enabledButton;
	wxChoice *modeChoice;

	CalxActionQueue *queue;
	InstrumentController *ctrl;
	InstrumentEventListener *listener;
	wxTimer *timer;
  };
}

#endif
