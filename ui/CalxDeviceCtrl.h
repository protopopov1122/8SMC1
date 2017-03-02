#ifndef _8SMC1_UI_CALX_DEVICE_CTRL_H_
#define _8SMC1_UI_CALX_DEVICE_CTRL_H_

#include "calx.h"
#include "CalxActionQueue.h"
#include <wx/stattext.h>
#include <wx/spinctrl.h>

namespace CalX {
	
	class CalxDeviceCtrl : public wxPanel {
		public:
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
			
			// Value windows
			wxSpinCtrl *moveSpin;
			
			// Info texts
			wxStaticText *position;
			wxStaticText *power;
			wxStaticText *state;
			wxStaticText *trailer1;
			wxStaticText *trailer2;
	};	
}

#endif