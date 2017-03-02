#include "CalxDevicePanel.h"
#include "CalxDeviceCtrl.h"
#include "CalxApp.h"
#include <wx/listctrl.h>
#include <wx/sizer.h>

namespace CalX {
	
	CalxDevicePanel::CalxDevicePanel(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
			
		CalxApp &app = wxGetApp();
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		for (size_t i = 0; i < app.getSystemManager()->getDeviceCount(); i++) {
			CalxDeviceCtrl *ctrl = new CalxDeviceCtrl(this, wxID_ANY,
				app.getSystemManager()->getDeviceController(i));
			sizer->Add(ctrl, 0, wxEXPAND | wxALL, 10);
		}
		this->SetSizer(sizer);
	}
}