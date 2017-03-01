#include "CalxDevicePanel.h"
#include "CalxApp.h"
#include <wx/listctrl.h>
#include <wx/sizer.h>

namespace CalX {
	
	CalxDevicePanel::CalxDevicePanel(wxWindow *win, wxWindowID id)
		: wxListbook::wxListbook(win, id) {
			
		CalxApp &app = wxGetApp();
		for (size_t i = 0; i < app.getSystemManager()->getDeviceCount(); i++) {
			wxPanel *panel = new wxPanel(this);
			this->AddPage(panel, "Device #" + std::to_string(i));
		}
	}
}