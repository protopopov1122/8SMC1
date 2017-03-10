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

#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/timer.h>
#include "calx.h"
#include "CalxActionQueue.h"

namespace CalXUI {
	
	class CalxDeviceCtrl; // Forward reference
	
	class CalxMotorEventListener : public MotorEventListener {
		public:
			CalxMotorEventListener(CalxDeviceCtrl*);
			virtual ~CalxMotorEventListener();
			
			virtual void use();
			virtual void unuse();
		private:
			CalxDeviceCtrl *dev;
			int used;
	};
	
	
	class CalxDeviceTimer : public wxTimer {
		public:
			CalxDeviceTimer() : wxTimer::wxTimer() {
			}
			~CalxDeviceTimer() {}
			void setCtrl(CalxDeviceCtrl *d) {this->ctrl = d;}
			virtual void Notify();
		private:
			CalxDeviceCtrl *ctrl;
	};
	
	class CalxDeviceCtrl : public wxPanel {
		public:
			friend class CalxMotorEventListener;
			CalxDeviceCtrl(wxWindow*, wxWindowID, CalX::DeviceController*);
			
			void updateUI();
			void stop();
			void setEnabled(bool);
			void setMaster(bool);
		private:
			void switchPowerClick(wxCommandEvent&);
			void rollToTrailer1(wxCommandEvent&);
			void rollToTrailer2(wxCommandEvent&);
			void stopClick(wxCommandEvent&);
			void moveClick(wxCommandEvent&);
			void rmoveClick(wxCommandEvent&);
			void threadUpdate(wxThreadEvent&);
			void OnExit(wxCloseEvent&);
		
			CalX::DeviceController *dev;
			CalxActionQueue *queue;
			MotorEventListener *listener;
			CalxDeviceTimer timer;
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
	};	
}

#endif