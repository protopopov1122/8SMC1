/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


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
	
	void CalxDevicePanel::OnExit(wxCloseEvent &evt) {
		
	}
	
	bool CalxDevicePanel::isBusy()  {
		for (const auto& d : devs) {
			if (d->isBusy()) {
				return true;
			}
		}
		return false;
	}
}