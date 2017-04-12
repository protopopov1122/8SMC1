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
#include "CalxMotorCtrl.h"
#include "CalxInstrumentCtrl.h"
#include "CalxCOMSelectDialog.h"

namespace CalXUI {
	
	wxDEFINE_EVENT(wxEVT_DEVICE_PANEL_UPDATE, wxThreadEvent);
	
	class CalxInstrumentSerialConnectRequest : public RequestProvider {
		public:
			CalxInstrumentSerialConnectRequest(CalxDevicePanel *devpanel)
				: RequestProvider::RequestProvider("connect.serial.instrument") {
				this->devpanel = devpanel;
			}
			
			virtual ~CalxInstrumentSerialConnectRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 3)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
				uint8_t port = (uint8_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				uint32_t speed = (uint16_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 1))->getValue();
				int_conf_t par = ((IntegerConfigValue*) PROVIDER_ARG(req, 2))->getValue();
				SerialPortParity parity = SerialPortParity::No;
				switch (par) {
					case 0:
						parity = SerialPortParity::No;			
					break;
					case 1:
						parity = SerialPortParity::Odd;			
					break;
					case 2:
						parity = SerialPortParity::Even;			
					break;
					case 3:
						parity = SerialPortParity::Mark;			
					break;
					case 4:
						parity = SerialPortParity::Space;			
					break;
				}
				DeviceSerialPortConnectionPrms prms;
				prms.port = port;
				prms.speed = speed;
				prms.parity = parity;
				InstrumentController *ctrl = sysman->connectInstrument(&prms);
				if (ctrl == nullptr) {
					return false;
				}
				devpanel->requestUpdate();
				return true;
			}
		private:
			CalxDevicePanel *devpanel;
	};
	
	class CalxMotorSerialConnectRequest : public RequestProvider {
		public:
			CalxMotorSerialConnectRequest(CalxDevicePanel *devpanel)
				: RequestProvider::RequestProvider("connect.serial.motor") {
				this->devpanel = devpanel;
			}
			
			virtual ~CalxMotorSerialConnectRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 3)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
				uint8_t port = (uint8_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				uint32_t speed = (uint16_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 1))->getValue();
				int_conf_t par = ((IntegerConfigValue*) PROVIDER_ARG(req, 2))->getValue();
				SerialPortParity parity = SerialPortParity::No;
				switch (par) {
					case 0:
						parity = SerialPortParity::No;			
					break;
					case 1:
						parity = SerialPortParity::Odd;			
					break;
					case 2:
						parity = SerialPortParity::Even;			
					break;
					case 3:
						parity = SerialPortParity::Mark;			
					break;
					case 4:
						parity = SerialPortParity::Space;			
					break;
				}
				DeviceSerialPortConnectionPrms prms;
				prms.port = port;
				prms.speed = speed;
				prms.parity = parity;
				MotorController *ctrl = sysman->connectMotor(&prms);
				if (ctrl == nullptr) {
					return false;
				}
				devpanel->requestUpdate();
				return true;
			}
		private:
			CalxDevicePanel *devpanel;
	};
	
	CalxDevicePanel::CalxDevicePanel(wxWindow *win, wxWindowID id)
		: wxScrolledWindow::wxScrolledWindow(win, id) {
			
		CalxApp &app = wxGetApp();
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxMotorSerialConnectRequest(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxInstrumentSerialConnectRequest(this));
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
					wxButton *comButton = new wxButton(connectPanel, wxID_ANY, __("Connect COM motor"));
					connectSizer->Add(comButton, 0, wxALL, 5);
					comButton->Bind(wxEVT_BUTTON, &CalxDevicePanel::OnCOMConnectMotor, this);
				} break;
			}
		}
		for (const auto& instrCon : instrConType) {
			switch (instrCon) {
				case DeviceConnectionType::SerialPort: {
					wxButton *comButton = new wxButton(connectPanel, wxID_ANY, __("Connect COM instrument"));
					connectSizer->Add(comButton, 0, wxALL, 5);
					comButton->Bind(wxEVT_BUTTON, &CalxDevicePanel::OnCOMConnectInstrument, this);
				} break;
			}
		}
		
		this->SetSizer(sizer);
		
		updateUI();
		Layout();
        this->SetScrollRate(5, 5);
		Bind(wxEVT_CLOSE_WINDOW, &CalxDevicePanel::OnExit, this);
		Bind(wxEVT_DEVICE_PANEL_UPDATE, &CalxDevicePanel::OnDevicePanelUpdate, this);
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
		
		for (size_t i = 0; i < app.getSystemManager()->getMotorCount(); i++) {
			CalxMotorCtrl *ctrl = new CalxMotorCtrl(this, wxID_ANY,
				app.getSystemManager()->getMotorController(i));
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
	
	void CalxDevicePanel::requestUpdate() {
		wxThreadEvent evt(wxEVT_DEVICE_PANEL_UPDATE);
		wxPostEvent(this, evt);
	}
	
	void CalxDevicePanel::OnCOMConnectMotor(wxCommandEvent &evt) {
		if (isBusy()) {
			wxMessageBox(__("Devices are busy"), __("Error"), wxICON_ERROR);
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
			MotorController *ctrl = app.getSystemManager()->connectMotor(&prms);
			if (ctrl == nullptr) {
				wxMessageBox(__("Device can't be connected on COM") + std::to_string(dialog->getPort()),
					__("Connection error"), wxICON_WARNING);
			}
			updateUI();
		}
		dialog->Destroy();
	}
	
	void CalxDevicePanel::OnCOMConnectInstrument(wxCommandEvent &evt) {
		if (isBusy()) {
			wxMessageBox(__("Devices are busy"), __("Error"), wxICON_ERROR);
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
				wxMessageBox(__("Instrument can't be connected on COM") + std::to_string(dialog->getPort()),
					__("Connection error"), wxICON_WARNING);
			}
		}
		dialog->Destroy();
		updateUI();
	}
	
	void CalxDevicePanel::OnDevicePanelUpdate(wxThreadEvent &evt) {
		updateUI();
		Refresh();
	}
}