#ifndef _8SMC1_UI_CALX_DEVICE_PANEL_H_
#define _8SMC1_UI_CALX_DEVICE_PANEL_H_

#include "calx.h"
#include <wx/listbook.h>

namespace CalX {
	
	class CalxDevicePanel : public wxListbook {
		public:
			CalxDevicePanel(wxWindow*, wxWindowID);
	};
}

#endif