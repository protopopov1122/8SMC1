/*
        Copyright (c) 2017 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_UI_DEV_CALX_DEVICE_HANDLE_H_
#define CALX_UI_DEV_CALX_DEVICE_HANDLE_H_

#include "ui/CalxApp.h"

namespace CalXUI {

	class CalxDeviceHandle : public wxPanel {
	 public:
		CalxDeviceHandle(wxWindow *win, wxWindowID id)
		    : wxPanel::wxPanel(win, id) {}
		virtual void updateUI() = 0;
		virtual void stop() = 0;
		virtual bool isBusy() = 0;
	};

	class CalxDeviceConstructor {
	 public:
		virtual ~CalxDeviceConstructor() {}
		virtual CalxDeviceHandle *construct(wxWindow *) = 0;
	};

	class CalxDevicePool {
	 public:
		virtual void appendDevice(CalxDeviceConstructor *) = 0;
	};
}

#endif
