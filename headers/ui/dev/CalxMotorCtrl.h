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


#ifndef CALX_UI_CALX_DEVICE_CTRL_H_
#define CALX_UI_CALX_DEVICE_CTRL_H_

#include "ui/calx.h"
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/timer.h>
#include "ui/CalxActionQueue.h"

namespace CalXUI {

	class CalxMotorCtrl; // Forward reference
	wxDECLARE_EVENT(wxEVT_MOTOR_CTRL_ENABLE, wxThreadEvent);

	class CalxMotorEventListener : public MotorEventListener {
		public:
			CalxMotorEventListener(CalxMotorCtrl*);
			virtual ~CalxMotorEventListener();

			virtual void use();
			virtual void unuse();
		private:
			CalxMotorCtrl *dev;
			int used;
	};


	class CalxMotorTimer : public wxTimer {
		public:
			CalxMotorTimer() : wxTimer::wxTimer() {
			}
			~CalxMotorTimer() {}
			void setCtrl(CalxMotorCtrl *d) {this->ctrl = d;}
			virtual void Notify();
		private:
			CalxMotorCtrl *ctrl;
	};

	class CalxMotorCtrl : public wxPanel {
		public:
			friend class CalxMotorEventListener;
			CalxMotorCtrl(wxWindow*, wxWindowID, CalX::MotorController*);

			void updateUI();
			void stop();
			void setEnabled(bool);
			void setMaster(bool);
			bool isBusy() {
				return !queue->isEmpty();
			}
		private:
			void switchPowerClick(wxCommandEvent&);
			void rollToTrailer1(wxCommandEvent&);
			void rollToTrailer2(wxCommandEvent&);
			void stopClick(wxCommandEvent&);
			void moveClick(wxCommandEvent&);
			void rmoveClick(wxCommandEvent&);
			void threadUpdate(wxThreadEvent&);
			void OnExit(wxCloseEvent&);
			void OnEnableEvent(wxThreadEvent&);
			void OnConfigEvent(wxCommandEvent&);

			CalX::MotorController *dev;
			CalxActionQueue *queue;
			MotorEventListener *listener;
			CalxMotorTimer timer;
			bool master;

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
