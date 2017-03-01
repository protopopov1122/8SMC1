#include "CalxPanel.h"

namespace CalX {
	
	CalxPanel::CalxPanel(wxWindow *win, wxWindowID id)
		: wxNotebook::wxNotebook(win, id) {
		
		this->dev = new CalxDevicePanel(this, wxID_ANY);
		this->AddPage(this->dev, "Devices");
	}
}