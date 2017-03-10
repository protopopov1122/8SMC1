#include <wx/listctrl.h>
#include <wx/sizer.h>
#include "CalxApp.h"
#include "CalxDevicePanel.h"
#include "CalxDeviceCtrl.h"

namespace CalXUI {
	
	CalxDevicePanel::CalxDevicePanel(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
			
		CalxApp &app = wxGetApp();
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		for (size_t i = 0; i < app.getSystemManager()->getDeviceCount(); i++) {
			CalxDeviceCtrl *ctrl = new CalxDeviceCtrl(this, wxID_ANY,
				app.getSystemManager()->getDeviceController(i));
			devs.push_back(ctrl);
			sizer->Add(ctrl, 0, wxEXPAND | wxALL, 10);
		}
		this->SetSizer(sizer);
	}
	
	void CalxDevicePanel::stop() {
		for (const auto& dev : this->devs) {
			dev->stop();
		}
	}
}