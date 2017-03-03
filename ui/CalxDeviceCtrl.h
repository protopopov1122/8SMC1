#ifndef _8SMC1_UI_CALX_DEVICE_CTRL_H_
#define _8SMC1_UI_CALX_DEVICE_CTRL_H_

#include "calx.h"
#include "CalxActionQueue.h"
#include <wx/stattext.h>
#include <wx/spinctrl.h>

namespace CalX {
	
	class CalxDeviceCtrl; // Forward reference
	
	class CalxMotorEventListener : public MotorEventListener {
		public:
			CalxMotorEventListener(CalxDeviceCtrl*);
			virtual ~CalxMotorEventListener();
			
			virtual void moving(MotorMoveEvent&);
			virtual void moved(MotorMoveEvent&);
			virtual void stopped(MotorErrorEvent&);
			virtual void rolling(MotorRollEvent&);
			virtual void rolled(MotorRollEvent&);
		private:
			CalxDeviceCtrl *dev;
	};
	
	class CalxDeviceCtrl : public wxPanel {
		public:
			friend class CalxMotorEventListener;
			CalxDeviceCtrl(wxWindow*, wxWindowID, _8SMC1::DeviceController*);
			
			void updateUI();
		private:
			void updateButtonClick(wxCommandEvent&);
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