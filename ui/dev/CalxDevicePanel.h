#ifndef _8SMC1_UI_CALX_DEVICE_PANEL_H_
#define _8SMC1_UI_CALX_DEVICE_PANEL_H_

#include "calx.h"

namespace CalX {
	
	class CalxDeviceCtrl; // Forward referencing
	
	class CalxDevicePanel : public wxPanel {
		public:
			CalxDevicePanel(wxWindow*, wxWindowID);
			void stop();
		private:
			std::vector<CalxDeviceCtrl*> devs;
	};
}

#endif