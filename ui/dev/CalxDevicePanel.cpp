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
	
	class CalxInstrumentSerialConnectProvider : public RequestProvider {
		public:
			CalxInstrumentSerialConnectProvider(CalxDevicePanel *devpanel)
				: RequestProvider::RequestProvider("connect.serial.instrument") {
				this->devpanel = devpanel;
			}
			
			virtual ~CalxInstrumentSerialConnectProvider() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_ARGC(req, 3)
				PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
				PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
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
	
	class CalxMotorSerialConnectProvider : public RequestProvider {
		public:
			CalxMotorSerialConnectProvider(CalxDevicePanel *devpanel)
				: RequestProvider::RequestProvider("connect.serial.motor") {
				this->devpanel = devpanel;
			}
			
			virtual ~CalxMotorSerialConnectProvider() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_ARGC(req, 3)
				PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
				PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
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
	
	class CalxMotorPowerProvider : public RequestProvider {
		public:
			CalxMotorPowerProvider(CalxDevicePanel *devpanel)
				: RequestProvider::RequestProvider("motor.power") {
				this->devpanel = devpanel;
			}
			
			virtual ~CalxMotorPowerProvider() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_ARGC(req, 2)
				PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Boolean)
				device_id_t devid = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				bool power = ((BoolConfigValue*) PROVIDER_ARG(req, 1))->getValue();
				MotorController *ctrl = sysman->getMotorController(devid);
				return ctrl->getMotor()->enablePower(power);
			}
		private:
			CalxDevicePanel *devpanel;
	};
	
	class CalxMotorConnectAction : public CalxAction {
		public:
			CalxMotorConnectAction(CalxDevicePanel *panel, DeviceConnectionPrms *prms) {
				this->panel = panel;
				this->prms = prms;
			}
			virtual ~CalxMotorConnectAction() {
				delete this->prms;
			}
			
			virtual void perform(SystemManager *sysman) {
				MotorController *ctrl = sysman->connectMotor(prms);
				if (ctrl == nullptr) {
					wxMessageBox(__("Motor can't be connected"),
						__("Connection error"), wxICON_WARNING);
				} else {
					panel->requestUpdate();
				}
			}
			
			virtual void stop() {
			}
		private:
			CalxDevicePanel *panel;
			DeviceConnectionPrms *prms;
	};
	
	class CalxInstrumentConnectAction : public CalxAction {
		public:
			CalxInstrumentConnectAction(CalxDevicePanel *panel, DeviceConnectionPrms *prms) {
				this->panel = panel;
				this->prms = prms;
			}
			virtual ~CalxInstrumentConnectAction() {
				delete this->prms;
			}
			
			virtual void perform(SystemManager *sysman) {
				InstrumentController *ctrl = sysman->connectInstrument(prms);
				if (ctrl == nullptr) {
					wxMessageBox(__("Instrument can't be connected"),
						__("Connection error"), wxICON_WARNING);
				} else {
					panel->requestUpdate();
				}
			}
			
			virtual void stop() {
			}
		private:
			CalxDevicePanel *panel;
			DeviceConnectionPrms *prms;
	};
	
	CalxDevicePanel::CalxDevicePanel(wxWindow *win, wxWindowID id)
		: wxScrolledWindow::wxScrolledWindow(win, id) {
			
		CalxApp &app = wxGetApp();
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxMotorSerialConnectProvider(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxInstrumentSerialConnectProvider(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxMotorPowerProvider(this));
		this->queue = new CalxActionQueue(app.getSystemManager(), this);
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
		this->queue->Run();
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
		this->queue->stop();
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
		CalxCOMSelectDialog *dialog = new CalxCOMSelectDialog(this, wxID_ANY);
		dialog->ShowModal();
		if (dialog->getPort() != -1) {
			DeviceSerialPortConnectionPrms *prms = new DeviceSerialPortConnectionPrms();
			prms->port = dialog->getPort();
			prms->speed = dialog->getSpeed();
			prms->parity = dialog->getParity();
			this->queue->addAction(new CalxMotorConnectAction(this, prms));
		}
		dialog->Destroy();
	}
	
	void CalxDevicePanel::OnCOMConnectInstrument(wxCommandEvent &evt) {
		if (isBusy()) {
			wxMessageBox(__("Devices are busy"), __("Error"), wxICON_ERROR);
			return;
		}
		CalxCOMSelectDialog *dialog = new CalxCOMSelectDialog(this, wxID_ANY);
		dialog->ShowModal();
		if (dialog->getPort() != -1) {
			DeviceSerialPortConnectionPrms *prms = new DeviceSerialPortConnectionPrms();
			prms->port = dialog->getPort();
			prms->speed = dialog->getSpeed();
			prms->parity = dialog->getParity();
			this->queue->addAction(new CalxInstrumentConnectAction(this, prms));
		}
		dialog->Destroy();
	}
	
	void CalxDevicePanel::OnDevicePanelUpdate(wxThreadEvent &evt) {
		updateUI();
		Refresh();
	}
}