#ifndef _8SMC1_UI_CALX_PANEL_H_
#define _8SMC1_UI_CALX_PANEL_H_

#include "calx.h"
#include "CalxDevicePanel.h"
#include <wx/notebook.h>

namespace CalX {
	
	class CalxPanel : public wxNotebook {
		public:
			CalxPanel(wxWindow*, wxWindowID);
		private:
			CalxDevicePanel *dev;
	};
}

#endif