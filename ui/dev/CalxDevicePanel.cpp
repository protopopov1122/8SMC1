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


#include <vector>
#include "CalxApp.h"
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include "CalxDevicePanel.h"
#include "CalxDeviceCtrl.h"
#include "CalxInstrumentCtrl.h"
#include "CalxCOMSelectDialog.h"

namespace CalXUI {
	
	CalxDevicePanel::CalxDevicePanel(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
			
		CalxApp &app = wxGetApp();
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		
		wxPanel *connectPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(connectPanel, 0, wxALL | wxEXPAND);
		wxBoxSizer *connectSizer = new wxBoxSizer(wxHORIZONTAL);
		connectPanel->SetSizer(connectSizer);
		std::vector<DeviceConnectionType> devConType;
		std::vector<DeviceConnectionType> instrConType;
		app.getSystemManager()->getDeviceManager()->getConnectionTypes(devConType, instrConType);
		for (const auto& devCon : devConType) {
			switch (devCon) {
				case DeviceConnectionType::SerialPort: {
					wxButton *comButton = new wxButton(connectPanel, wxID_ANY, "Connect COM motor");
					connectSizer->Add(comButton, 0, wxALL, 5);
					comButton->Bind(wxEVT_BUTTON, &CalxDevicePanel::OnCOMConnectDevice, this);
				} break;
			}
		}
		for (const auto& instrCon : instrConType) {
			switch (instrCon) {
				case DeviceConnectionType::SerialPort: {
					wxButton *comButton = new wxButton(connectPanel, wxID_ANY, "Connect COM instrument");
					connectSizer->Add(comButton, 0, wxALL, 5);
					comButton->Bind(wxEVT_BUTTON, &CalxDevicePanel::OnCOMConnectInstrument, this);
				} break;
			}
		}
		
		this->SetSizer(sizer);
		
		updateUI();
		Layout();
		Bind(wxEVT_CLOSE_WINDOW, &CalxDevicePanel::OnExit, this);
	}
	
	void CalxDevicePanel::stop() {
		for (const auto& dev : this->devs) {
			dev->stop();
		}
		
		for (const auto& instr : this->instrs) {
			instr->stop();
		}
	}
	
	void CalxDevicePanel::OnExit(wxCloseEvent &evt) {
		for (const auto& instr : instrs) {
			instr->Close(true);
		}
		Destroy();
	}
	
	bool CalxDevicePanel::isBusy()  {
		for (const auto& d : devs) {
			if (d->isBusy()) {
				return true;
			}
		}
		for (const auto& i : instrs) {
			if (i->isBusy()) {
				return true;
			}
		}
		return false;
	}
	
	void CalxDevicePanel::updateUI() {
		for (const auto& dev : devs) {
			dev->stop();
			dev->Close(true);
		}
		devs.clear();
		
		for (const auto& instr : instrs) {
			instr->stop();
			instr->Close(true);
		}
		instrs.clear();

		CalxApp &app = wxGetApp();		
		
		for (size_t i = 0; i < app.getSystemManager()->getDeviceCount(); i++) {
			CalxDeviceCtrl *ctrl = new CalxDeviceCtrl(this, wxID_ANY,
				app.getSystemManager()->getDeviceController(i));
			devs.push_back(ctrl);
			GetSizer()->Add(ctrl, 0, wxEXPAND | wxALL, 10);
		}	
		
		for (size_t i = 0; i < app.getSystemManager()->getInstrumentCount(); i++) {
			CalxInstrumentCtrl *ctrl = new CalxInstrumentCtrl(this, wxID_ANY,
				app.getSystemManager()->getInstrumentController(i));
			instrs.push_back(ctrl);
			GetSizer()->Add(ctrl, 0, wxEXPAND | wxALL, 10);
		}

		Layout();
	}
	
	void CalxDevicePanel::OnCOMConnectDevice(wxCommandEvent &evt) {
		if (isBusy()) {
			wxMessageBox("Devices are busy", "Error", wxICON_ERROR);
			return;
		}
		CalxApp &app = wxGetApp();		
		CalxCOMSelectDialog *dialog = new CalxCOMSelectDialog(this, wxID_ANY);
		dialog->ShowModal();
		if (dialog->getPort() != -1) {
			DeviceSerialPortConnectionPrms prms;
			prms.port = dialog->getPort();
			prms.speed = 9600;
			prms.parity = SerialPortParity::No;
			DeviceController *ctrl = app.getSystemManager()->connectDevice(&prms);
			if (ctrl == nullptr) {
				wxMessageBox("Device can't be connected on COM" + std::to_string(dialog->getPort()),
					"Connection error", wxICON_WARNING);
			}
			updateUI();
		}
		dialog->Destroy();
	}
	
	void CalxDevicePanel::OnCOMConnectInstrument(wxCommandEvent &evt) {
		if (isBusy()) {
			wxMessageBox("Devices are busy", "Error", wxICON_ERROR);
			return;
		}
		CalxApp &app = wxGetApp();		
		CalxCOMSelectDialog *dialog = new CalxCOMSelectDialog(this, wxID_ANY);
		dialog->ShowModal();
		if (dialog->getPort() != -1) {
			DeviceSerialPortConnectionPrms prms;
			prms.port = dialog->getPort();
			prms.speed = dialog->getSpeed();
			prms.parity = dialog->getParity();
			InstrumentController *ctrl = app.getSystemManager()->connectInstrument(&prms);
			if (ctrl == nullptr) {
				wxMessageBox("Instrument can't be connected on COM" + std::to_string(dialog->getPort()),
					"Connection error", wxICON_WARNING);
			}
		}
		dialog->Destroy();
		updateUI();
	}
}