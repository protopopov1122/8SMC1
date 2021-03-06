/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ui/dev/CalxDevicePanel.h"
#include "calx/ui/CalxApp.h"
#include "calx/ui/dev/CalxDeviceFactory.h"
#include <vector>
#include <wx/listctrl.h>
#include <wx/sizer.h>

namespace CalX::UI {

	wxDEFINE_EVENT(wxEVT_DEVICE_PANEL_UPDATE, wxThreadEvent);
	wxDEFINE_EVENT(wxEVT_DEVICE_PANEL_DEVICE_APPEND, wxThreadEvent);

	CalxDevicePanel::CalxDevicePanel(wxWindow *win, wxWindowID id)
	    : CalxPanelPane::CalxPanelPane(win, id) {
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), [this]() {
			wxQueueEvent(this, new wxThreadEvent(wxEVT_COMMAND_QUEUE_UPDATE));
		});
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);

		connectPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(connectPanel, 0, wxALL | wxEXPAND);
		wxBoxSizer *connectSizer = new wxBoxSizer(wxHORIZONTAL);
		connectPanel->SetSizer(connectSizer);

		Layout();
		this->SetScrollRate(5, 5);
		Bind(wxEVT_CLOSE_WINDOW, &CalxDevicePanel::OnExit, this);
		Bind(wxEVT_DEVICE_PANEL_UPDATE, &CalxDevicePanel::OnDevicePanelUpdate,
		     this);
		Bind(wxEVT_DEVICE_PANEL_DEVICE_APPEND, &CalxDevicePanel::OnDeviceAppend,
		     this);
		this->queue->start();
	}

	void CalxDevicePanel::shutdown() {
		for (const auto &dev : this->devices) {
			dev->stop();
		}
	}

	void CalxDevicePanel::OnExit(wxCloseEvent &evt) {
		this->queue->stop();
		for (const auto &kv : this->factories) {
			delete kv.second;
		}
		for (const auto &dev : devices) {
			dev->Close(true);
		}
		Destroy();
	}

	bool CalxDevicePanel::isBusy() {
		for (const auto &d : devices) {
			if (d->isBusy()) {
				return true;
			}
		}
		return false;
	}

	void CalxDevicePanel::OnDeviceConnectClick(wxCommandEvent &evt) {
		if (this->factories.count(evt.GetEventObject()) != 0) {
			CalxDeviceFactory *fact = this->factories[evt.GetEventObject()];
			fact->newDevice(this, this, this->queue);
		}
	}

	void CalxDevicePanel::appendDevice(CalxDeviceConstructor *cnstr,
	                                   bool *ready) {
		wxThreadEvent evt(wxEVT_DEVICE_PANEL_DEVICE_APPEND);
		evt.SetPayload(std::make_pair(cnstr, ready));
		wxPostEvent(this, evt);
	}

	void CalxDevicePanel::appendDeviceFactory(const std::string &name,
	                                          CalxDeviceFactory *fact) {
		wxButton *button =
		    new wxButton(this->connectPanel, wxID_ANY, __("Connect ") + name);
		button->Bind(wxEVT_BUTTON, &CalxDevicePanel::OnDeviceConnectClick, this);
		this->factories[button] = fact;
		this->connectPanel->GetSizer()->Add(button);
	}

	void CalxDevicePanel::updateUI() {
		Layout();
	}

	void CalxDevicePanel::OnDevicePanelUpdate(wxThreadEvent &evt) {
		updateUI();
		Refresh();
	}

	void CalxDevicePanel::OnDeviceAppend(wxThreadEvent &evt) {
		std::pair<CalxDeviceConstructor *, bool *> pair =
		    evt.GetPayload<std::pair<CalxDeviceConstructor *, bool *>>();
		CalxDeviceConstructor *cnstr = pair.first;
		CalxDeviceHandle *device = cnstr->construct(this);
		if (device->getController()->getDevice().getType() == DeviceType::Motor) {
			device_id_t id = device->getController()->getID();
			this->motors.insert(
			    std::make_pair(id, dynamic_cast<CalxMotorHandle *>(device)));
		} else {
			this->instrs[device->getController()->getID()] =
			    dynamic_cast<CalxInstrumentHandle *>(device);
		}
		delete cnstr;
		GetSizer()->Add(device, 0, wxEXPAND | wxALL, 10);
		this->devices.push_back(device);
		if (pair.second != nullptr) {
			*pair.second = true;
		}
		updateUI();
		Refresh();
	}

	std::size_t CalxDevicePanel::getMotorCount() {
		return this->motors.size();
	}

	CalxMotorHandle *CalxDevicePanel::getMotor(device_id_t id) {
		if (this->motors.count(id) == 0) {
			return nullptr;
		} else {
			return this->motors[id];
		}
	}

	std::size_t CalxDevicePanel::getInstrumentCount() {
		return this->instrs.size();
	}

	CalxInstrumentHandle *CalxDevicePanel::getInstrument(device_id_t id) {
		if (this->instrs.count(id) == 0) {
			return nullptr;
		} else {
			return this->instrs[id];
		}
	}

	void CalxDevicePanel::stop() {
		for (const auto &instr : this->instrs) {
			instr.second->stopInstrument();
		}
		for (const auto &motor : this->motors) {
			motor.second->stopMovement();
		}
	}
}  // namespace CalX::UI
