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

#ifndef CALX_UI_CALX_DEVICE_PANEL_H_
#define CALX_UI_CALX_DEVICE_PANEL_H_

#include <map>
#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/CalxPanelPane.h"
#include "ui/dev/CalxDeviceFactory.h"

namespace CalXUI {

	wxDECLARE_EVENT(wxEVT_DEVICE_PANEL_UPDATE, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_DEVICE_PANEL_DEVICE_APPEND, wxThreadEvent);

	class CalxDevicePanel : public CalxPanelPane, public CalxDevicePool {
	 public:
		CalxDevicePanel(wxWindow *, wxWindowID);
		void shutdown() override;
		bool isBusy() override;
		void updateUI() override;
		void appendDevice(CalxDeviceConstructor *, bool * = nullptr) override;
		virtual void appendDeviceFactory(std::string, CalxDeviceFactory *);

		std::size_t getMotorCount() override;
		CalxMotorHandle *getMotor(device_id_t) override;
		std::size_t getInstrumentCount() override;
		CalxInstrumentHandle *getInstrument(device_id_t) override;
		void stop() override;

	 private:
		void OnExit(wxCloseEvent &);
		void OnDeviceConnectClick(wxCommandEvent &);
		void OnDevicePanelUpdate(wxThreadEvent &);
		void OnDeviceAppend(wxThreadEvent &);

		std::vector<CalxDeviceHandle *> devices;
		std::map<wxObject *, CalxDeviceFactory *> factories;
		CalxActionQueue *queue;

		wxPanel *connectPanel;

		std::map<device_id_t, CalxMotorHandle *> motors;
		std::map<device_id_t, CalxInstrumentHandle *> instrs;
	};
}  // namespace CalXUI

#endif
