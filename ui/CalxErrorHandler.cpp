/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "ui/CalxErrorHandler.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalX::UI {

	CalxErrorHandler::CalxErrorHandler(SystemManager &sysman) : sysman(sysman) {}

	void CalxErrorHandler::handle(ErrorCode code) {
		wxThreadEvent evt(wxEVT_APP_ERROR);
		evt.SetPayload(code);
		wxPostEvent(&wxGetApp(), evt);
	}

	void CalxErrorHandler::display(ErrorCode code) {
		if (code != ErrorCode::NoError) {
			std::string text = __("Error occured during execution.\nError code") +
			                   std::string(": ") +
			                   std::to_string(static_cast<int>(code)) + "\n";
			switch (code) {
				case ErrorCode::LowLevelError: {
					text += "Device low level error log:\n";
					DeviceManager &devman = this->sysman.getDeviceManager();
					while (devman.hasError()) {
						text += "\t" + devman.pollError() + "\n";
					}
				} break;
				default:
					text += __("Detailed description not available.");
					break;
			}
			wxMessageBox(text, __("Error occured"), wxICON_ERROR);
		}
	}

	void CalxErrorHandler::alert(std::string text, std::string title,
	                             long style) {
		auto params = std::make_tuple(text, title, style);
		wxThreadEvent evt(wxEVT_APP_ALERT);
		evt.SetPayload(params);
		wxPostEvent(&wxGetApp(), evt);
	}
}  // namespace CalX::UI
