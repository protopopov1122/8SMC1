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

#ifndef CALX_UI_DEV_CALX_INSTRUMENT_COMPONENT_H_
#define CALX_UI_DEV_CALX_INSTRUMENT_COMPONENT_H_

#include "ui/CalxActionQueue.h"
#include "ui/calx.h"
#include "ui/dev/CalxDeviceHandle.h"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/timer.h>

namespace CalXUI {

	class CalxMotorComponent;  // Forward reference
	wxDECLARE_EVENT(wxEVT_MOTOR_CTRL_ENABLE, wxThreadEvent);

	class CalxMotorEventListener : public MotorEventListener {
	 public:
		CalxMotorEventListener(CalxMotorComponent *);
		virtual ~CalxMotorEventListener();

		virtual void use();
		virtual void unuse();

	 private:
		CalxMotorComponent *dev;
		int used;
	};

	class CalxMotorTimer : public wxTimer {
	 public:
		CalxMotorTimer() : wxTimer::wxTimer(), ctrl(nullptr) {}
		~CalxMotorTimer() {}
		void setCtrl(CalxMotorComponent *d) {
			this->ctrl = d;
		}
		virtual void Notify();

	 private:
		CalxMotorComponent *ctrl;
	};

	class CalxMotorComponent : public CalxDeviceHandle {
	 public:
		friend class CalxMotorEventListener;
		CalxMotorComponent(wxWindow *, wxWindowID, std::shared_ptr<CalX::MotorController>);
		virtual void updateUI();
		virtual void stop();
		virtual bool isBusy();
		void setEnabled(bool);

	 private:
		void switchPowerClick(wxCommandEvent &);
		void rollToTrailer1(wxCommandEvent &);
		void rollToTrailer2(wxCommandEvent &);
		void stopClick(wxCommandEvent &);
		void moveClick(wxCommandEvent &);
		void rmoveClick(wxCommandEvent &);
		void threadUpdate(wxThreadEvent &);
		void OnExit(wxCloseEvent &);
		void OnEnableEvent(wxThreadEvent &);
		void OnConfigEvent(wxCommandEvent &);

		std::shared_ptr<CalX::MotorController> dev;
		CalxActionQueue *queue;
		MotorEventListener *listener;
		CalxMotorTimer timer;

		wxPanel *infoPanel;
		wxPanel *movePanel;
		wxPanel *actionPanel;
		wxButton *stopButton;

		// Value windows
		wxSpinCtrl *moveSpin;
		wxSpinCtrl *moveSpeedSpin;

		// Info texts
		wxStaticText *position;
		wxStaticText *power;
		wxStaticText *state;
		wxStaticText *trailer1;
		wxStaticText *trailer2;
		wxStaticText *hardwareInfo;
		wxStaticText *runtimeInfo;
	};
}

#endif
