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


#ifndef CALX_UI_CALX_DEVICE_PANEL_H_
#define CALX_UI_CALX_DEVICE_PANEL_H_

#include "ui/CalxApp.h"
#include "ui/CalxActionQueue.h"

namespace CalXUI {

	class CalxMotorCtrl; // Forward referencing
	class CalxInstrumentCtrl; // Forward referencing

	wxDECLARE_EVENT(wxEVT_DEVICE_PANEL_UPDATE, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_DEVICE_PANEL_MOTOR_APPEND, wxThreadEvent);
	wxDECLARE_EVENT(wxEVT_DEVICE_PANEL_INSTR_APPEND, wxThreadEvent);

	class CalxDevicePanel : public wxScrolledWindow {
		public:
			CalxDevicePanel(wxWindow*, wxWindowID);
			void stop();
			bool isBusy();
			void updateUI();
			void append(MotorController*);
			void append(InstrumentController*);
			void requestUpdate();
			void requestAppend(MotorController*);
			void requestAppend(InstrumentController*);
			void fullUpdate();
		private:
			void OnExit(wxCloseEvent&);
			void OnCOMConnectMotor(wxCommandEvent&);
			void OnCOMConnectInstrument(wxCommandEvent&);
			void OnDevicePanelUpdate(wxThreadEvent&);
			void OnDevicePanelMotorAppend(wxThreadEvent&);
			void OnDevicePanelInstrAppend(wxThreadEvent&);

			std::vector<CalxMotorCtrl*> devs;
			std::vector<CalxInstrumentCtrl*> instrs;
			CalxActionQueue *queue;
	};
}

#endif
