#ifndef _8SMC1_UI_CALX_PANEL_H_
#define _8SMC1_UI_CALX_PANEL_H_

#include "calx.h"
#include "CalxDevicePanel.h"
#include "CalxCoordPanel.h"
#include "CalxTaskPanel.h"
#include <wx/notebook.h>

namespace CalX {
	
	class CalxCoordPanel; // Forward referencing
	class CalxTaskPanel; // Forward referencing
	
	class CalxPanel : public wxNotebook {
		public:
			CalxPanel(wxWindow*, wxWindowID);
			CalxDevicePanel *getDevices() {return this->dev;}
			CalxCoordPanel *getCoords() {return this->coord;}
			CalxTaskPanel *getTasks() {return this->task;}
			
			void updateUI();
		private:
			CalxDevicePanel *dev;
			CalxCoordPanel *coord;
			CalxTaskPanel *task;
	};
}

#endif