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
#include "ui/CalxApp.h"
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include "ui/dev/CalxDevicePanel.h"
#include "ui/dev/CalxMotorCtrl.h"
#include "ui/dev/CalxInstrumentCtrl.h"
#include "ui/dev/CalxCOMSelectDialog.h"
#include "ui/dev/CalxDeviceProviders.h"

namespace CalXUI {

  wxDEFINE_EVENT(wxEVT_DEVICE_PANEL_UPDATE, wxThreadEvent);
  wxDEFINE_EVENT(wxEVT_DEVICE_PANEL_MOTOR_APPEND, wxThreadEvent);
  wxDEFINE_EVENT(wxEVT_DEVICE_PANEL_INSTR_APPEND, wxThreadEvent);

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
		wxMessageBox(__("Motor can't be connected"), __("Connection error"),
					 wxICON_WARNING);
	  } else {
		wxThreadEvent evt(wxEVT_DEVICE_PANEL_MOTOR_APPEND);
		evt.SetPayload(ctrl);
		wxPostEvent(panel, evt);
	  }
	}

	virtual void stop() {}

   private:
	CalxDevicePanel *panel;
	DeviceConnectionPrms *prms;
  };

  class CalxInstrumentConnectAction : public CalxAction {
   public:
	CalxInstrumentConnectAction(CalxDevicePanel *panel,
								DeviceConnectionPrms *prms) {
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
		wxThreadEvent evt(wxEVT_DEVICE_PANEL_INSTR_APPEND);
		evt.SetPayload(ctrl);
		wxPostEvent(panel, evt);
	  }
	}

	virtual void stop() {}

   private:
	CalxDevicePanel *panel;
	DeviceConnectionPrms *prms;
  };

  CalxDevicePanel::CalxDevicePanel(wxWindow *win, wxWindowID id)
	  : wxScrolledWindow::wxScrolledWindow(win, id) {
	CalxApp &app = wxGetApp();
	this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *connectPanel = new wxPanel(this, wxID_ANY);
	sizer->Add(connectPanel, 0, wxALL | wxEXPAND);
	wxBoxSizer *connectSizer = new wxBoxSizer(wxHORIZONTAL);
	connectPanel->SetSizer(connectSizer);
	std::vector<DeviceConnectionType> devConType;
	std::vector<DeviceConnectionType> instrConType;
	app.getSystemManager()->getDeviceManager()->getConnectionTypes(
		devConType, instrConType);
	for (const auto &devCon : devConType) {
	  switch (devCon) {
		case DeviceConnectionType::SerialPort: {
		  wxButton *comButton =
			  new wxButton(connectPanel, wxID_ANY, __("Connect COM motor"));
		  connectSizer->Add(comButton, 0, wxALL, 5);
		  comButton->Bind(wxEVT_BUTTON, &CalxDevicePanel::OnCOMConnectMotor,
						  this);
		} break;
	  }
	}
	for (const auto &instrCon : instrConType) {
	  switch (instrCon) {
		case DeviceConnectionType::SerialPort: {
		  wxButton *comButton = new wxButton(connectPanel, wxID_ANY,
											 __("Connect COM instrument"));
		  connectSizer->Add(comButton, 0, wxALL, 5);
		  comButton->Bind(wxEVT_BUTTON,
						  &CalxDevicePanel::OnCOMConnectInstrument, this);
		} break;
	  }
	}

	this->SetSizer(sizer);

	fullUpdate();
	Layout();
	this->SetScrollRate(5, 5);
	Bind(wxEVT_CLOSE_WINDOW, &CalxDevicePanel::OnExit, this);
	Bind(wxEVT_DEVICE_PANEL_UPDATE, &CalxDevicePanel::OnDevicePanelUpdate,
		 this);
	Bind(wxEVT_DEVICE_PANEL_MOTOR_APPEND,
		 &CalxDevicePanel::OnDevicePanelMotorAppend, this);
	Bind(wxEVT_DEVICE_PANEL_INSTR_APPEND,
		 &CalxDevicePanel::OnDevicePanelInstrAppend, this);
	this->queue->Run();
  }

  void CalxDevicePanel::shutdown() {
	for (const auto &dev : this->devs) {
	  dev->stop();
	}

	for (const auto &instr : this->instrs) {
	  instr->stop();
	}
  }

  void CalxDevicePanel::OnExit(wxCloseEvent &evt) {
	this->queue->stop();
	for (const auto &instr : instrs) {
	  instr->Close(true);
	}
	Destroy();
  }

  bool CalxDevicePanel::isBusy() {
	for (const auto &d : devs) {
	  if (d->isBusy()) {
		return true;
	  }
	}
	for (const auto &i : instrs) {
	  if (i->isBusy()) {
		return true;
	  }
	}
	return false;
  }

  void CalxDevicePanel::fullUpdate() {
	for (const auto &dev : devs) {
	  dev->stop();
	  dev->Close(true);
	}
	devs.clear();

	for (const auto &instr : instrs) {
	  instr->stop();
	  instr->Close(true);
	}
	instrs.clear();

	CalxApp &app = wxGetApp();

	for (size_t i = 0; i < app.getSystemManager()->getMotorCount(); i++) {
	  CalxMotorCtrl *ctrl = new CalxMotorCtrl(
		  this, wxID_ANY,
		  app.getSystemManager()->getMotorController((device_id_t) i));
	  devs.push_back(ctrl);
	  GetSizer()->Add(ctrl, 0, wxEXPAND | wxALL, 10);
	}

	for (size_t i = 0; i < app.getSystemManager()->getInstrumentCount(); i++) {
	  CalxInstrumentCtrl *ctrl = new CalxInstrumentCtrl(
		  this, wxID_ANY,
		  app.getSystemManager()->getInstrumentController((device_id_t) i));
	  instrs.push_back(ctrl);
	  GetSizer()->Add(ctrl, 0, wxEXPAND | wxALL, 10);
	}

	Layout();
  }

  void CalxDevicePanel::updateUI() {
	Layout();
  }

  void CalxDevicePanel::append(MotorController *_ctrl) {
	CalxMotorCtrl *ctrl = new CalxMotorCtrl(this, wxID_ANY, _ctrl);
	devs.push_back(ctrl);
	GetSizer()->Add(ctrl, 0, wxEXPAND | wxALL, 10);
  }

  void CalxDevicePanel::append(InstrumentController *_ctrl) {
	CalxInstrumentCtrl *ctrl = new CalxInstrumentCtrl(this, wxID_ANY, _ctrl);
	instrs.push_back(ctrl);
	GetSizer()->Add(ctrl, 0, wxEXPAND | wxALL, 10);
  }

  void CalxDevicePanel::OnCOMConnectMotor(wxCommandEvent &evt) {
	if (isBusy()) {
	  wxMessageBox(__("Devices are busy"), __("Error"), wxICON_ERROR);
	  return;
	}
	CalxCOMSelectDialog *dialog = new CalxCOMSelectDialog(this, wxID_ANY);
	dialog->ShowModal();
	if (dialog->getPort() != -1) {
	  DeviceSerialPortConnectionPrms *prms =
		  new DeviceSerialPortConnectionPrms();
	  prms->port = (uint8_t) dialog->getPort();
	  prms->speed = (uint32_t) dialog->getSpeed();
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
	  DeviceSerialPortConnectionPrms *prms =
		  new DeviceSerialPortConnectionPrms();
	  prms->port = (uint8_t) dialog->getPort();
	  prms->speed = (uint16_t) dialog->getSpeed();
	  prms->parity = dialog->getParity();
	  this->queue->addAction(new CalxInstrumentConnectAction(this, prms));
	}
	dialog->Destroy();
  }

  void CalxDevicePanel::OnDevicePanelUpdate(wxThreadEvent &evt) {
	updateUI();
	Refresh();
  }

  void CalxDevicePanel::OnDevicePanelMotorAppend(wxThreadEvent &evt) {
	MotorController *ctrl = evt.GetPayload<MotorController *>();
	append(ctrl);
	updateUI();
	Refresh();
  }

  void CalxDevicePanel::OnDevicePanelInstrAppend(wxThreadEvent &evt) {
	InstrumentController *ctrl = evt.GetPayload<InstrumentController *>();
	append(ctrl);
	updateUI();
	Refresh();
  }
}
