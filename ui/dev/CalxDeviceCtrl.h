#ifndef _8SMC1_UI_CALX_DEVICE_CTRL_H_
#define _8SMC1_UI_CALX_DEVICE_CTRL_H_

#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/timer.h>
#include "calx.h"
#include "CalxActionQueue.h"

namespace CalX {
	
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
			CalxDeviceCtrl(wxWindow*, wxWindowID, _8SMC1::DeviceController*);
			
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
		
			_8SMC1::DeviceController *dev;
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