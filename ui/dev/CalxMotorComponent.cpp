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

#include "ui/dev/CalxMotorComponent.h"
#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/device/DeviceManager.h"
#include "ui/CalxApp.h"
#include "ui/CalxErrorHandler.h"
#include "ui/config/CalxConfigEditor.h"
#include "ctrl-lib/logger/Shortcuts.h"
#include <iostream>
#include <limits.h>
#include <signal.h>
#include <wx/statbox.h>

namespace CalXUI {
	wxDEFINE_EVENT(wxEVT_MOTOR_CTRL_ENABLE, wxThreadEvent);

	void CalxMotorTimer::Notify() {
		ctrl->updateUI();
	}

	CalxMotorEventListener::CalxMotorEventListener(CalxMotorComponent *ctrl) {
		this->dev = ctrl;
		this->used = 0;
	}

	CalxMotorEventListener::~CalxMotorEventListener() {}

	void CalxMotorEventListener::onUse() {
		if (this->used == 0) {
			dev->setEnabled(false);
		}
		this->used++;
	}

	void CalxMotorEventListener::onUnuse() {
		this->used--;
		if (this->used == 0) {
			dev->setEnabled(true);
		}
	}

	void deviceActionFinished(MotorController &dev, ErrorCode errcode) {
		Info(wxGetApp().getJournal())
		    << "Device #" << dev.getID() << " action finished with error code "
		    << static_cast<int>(errcode);
	}

	void deviceActionStopped(MotorController &dev) {
		Info(wxGetApp().getJournal())
		    << "Device #" << dev.getID() << " action stopped";
	}

	class CalxMotorMoveAction : public CalxAction {
	 public:
		CalxMotorMoveAction(CalxMotorComponent *ctrl, MotorController *dev,
		                    int dest, float speed, bool rel,
		                    ActionResult *act_res = nullptr) {
			this->ctrl = ctrl;
			this->dev = dev;
			this->dest = dest;
			this->speed = speed;
			this->rel = rel;
			this->action_result = act_res;
			if (act_res != nullptr) {
				this->action_result->ready = false;
				this->action_result->stopped = false;
				this->action_result->errcode = ErrorCode::NoError;
			}
		}

		virtual void perform(SystemManager *sysman) {
			ErrorCode errcode;
			Info(wxGetApp().getJournal())
			    << "Starting device #" << dev->getID() << (rel ? " relative " : " ")
			    << "move"
			    << " to " << dest << " with speed " << speed;
			dev->use();
			if (rel) {
				errcode = dev->startRelativeMove(dest, speed);
			} else {
				errcode = dev->startMove(dest, speed);
			}
			dev->unuse();
			wxGetApp().getErrorHandler()->handle(errcode);
			deviceActionFinished(*dev, errcode);
			if (this->action_result != nullptr) {
				this->action_result->ready = true;
				this->action_result->errcode = errcode;
			}
		}

		virtual void stop() {
			dev->stop();
			deviceActionStopped(*dev);
			if (this->action_result != nullptr) {
				this->action_result->stopped = true;
			}
		}

	 private:
		CalxMotorComponent *ctrl;
		MotorController *dev;
		int dest;
		float speed;
		bool rel;
		ActionResult *action_result;
	};

	class CalxMotorCalibrationAction : public CalxAction {
	 public:
		CalxMotorCalibrationAction(CalxMotorComponent *ctrl, MotorController *dev,
		                           TrailerId tr, ActionResult *act_res = nullptr) {
			this->ctrl = ctrl;
			this->dev = dev;
			this->tr = tr;
			this->action_result = act_res;
			if (act_res != nullptr) {
				this->action_result->ready = false;
				this->action_result->stopped = false;
				this->action_result->errcode = ErrorCode::NoError;
			}
		}

		virtual void perform(SystemManager *sysman) {
			Info(wxGetApp().getJournal())
			    << "Starting device #" << dev->getID() << " calibration to trailer #"
			    << static_cast<int>(tr);
			dev->use();
			ErrorCode errcode = dev->moveToTrailer(tr);
			dev->unuse();
			wxGetApp().getErrorHandler()->handle(errcode);
			deviceActionFinished(*dev, errcode);
			if (this->action_result != nullptr) {
				this->action_result->errcode = errcode;
				this->action_result->ready = true;
			}
		}

		virtual void stop() {
			dev->stop();
			deviceActionStopped(*dev);
			if (this->action_result != nullptr) {
				this->action_result->stopped = true;
			}
		}

	 private:
		CalxMotorComponent *ctrl;
		MotorController *dev;
		TrailerId tr;
		ActionResult *action_result;
	};

	CalxMotorComponent::CalxMotorComponent(
	    wxWindow *win, wxWindowID id, std::shared_ptr<CalX::MotorController> dev)
	    : CalxDeviceHandle::CalxDeviceHandle(win, id) {
		this->dev = dev;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->listener = std::make_shared<CalxMotorEventListener>(this);

		wxStaticBox *box = new wxStaticBox(
		    this, wxID_ANY,
		    FORMAT(__("Device #%s"), std::to_string(this->dev->getID())));
		wxStaticBoxSizer *sizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
		this->SetSizer(sizer);

		// Info panel elements
		this->infoPanel = new wxPanel(box, wxID_ANY);
		wxBoxSizer *infoSizer = new wxBoxSizer(wxVERTICAL);
		this->position = new wxStaticText(infoPanel, wxID_ANY,
		                                  __("Position") + std::string(": "));
		infoSizer->Add(position);
		this->power =
		    new wxStaticText(infoPanel, wxID_ANY, __("Power") + std::string(": "));
		infoSizer->Add(power);
		this->state =
		    new wxStaticText(infoPanel, wxID_ANY, __("State") + std::string(": "));
		infoSizer->Add(state);
		this->trailer1 = new wxStaticText(infoPanel, wxID_ANY,
		                                  __("Trailer 1") + std::string(": "));
		infoSizer->Add(trailer1);
		this->trailer2 = new wxStaticText(infoPanel, wxID_ANY,
		                                  __("Trailer 2") + std::string(": "));
		infoSizer->Add(trailer2);
		this->hardwareInfo = new wxStaticText(
		    infoPanel, wxID_ANY, __("Hardware info") + std::string(": "));
		infoSizer->Add(hardwareInfo);
		this->runtimeInfo = new wxStaticText(
		    infoPanel, wxID_ANY, __("Runtime info") + std::string(": "));
		infoSizer->Add(runtimeInfo);
		infoPanel->SetSizer(infoSizer);
		sizer->Add(infoPanel, 0, wxALL, 10);

		// Move panel layout
		this->movePanel = new wxPanel(box, wxID_ANY);
		wxBoxSizer *moveSizer = new wxBoxSizer(wxVERTICAL);

		wxPanel *moveDestPanel = new wxPanel(movePanel, wxID_ANY);
		wxBoxSizer *moveDestSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *moveDestText = new wxStaticText(
		    moveDestPanel, wxID_ANY, __("Destination") + std::string(": "));
		moveDestSizer->Add(moveDestText, 0, wxALIGN_CENTER);
		moveSpin = new wxSpinCtrl(moveDestPanel, wxID_ANY, wxEmptyString,
		                          wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
		                          INT_MIN, INT_MAX, 0);
		moveDestSizer->Add(moveSpin, 1, wxEXPAND);
		moveDestPanel->SetSizer(moveDestSizer);
		moveSizer->Add(moveDestPanel, 1, wxEXPAND);

		wxPanel *moveSpeedPanel = new wxPanel(movePanel, wxID_ANY);
		wxBoxSizer *moveSpeedSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *moveSpeedText = new wxStaticText(
		    moveSpeedPanel, wxID_ANY, __("Speed") + std::string(": "));
		moveSpeedSizer->Add(moveSpeedText, 0, wxALIGN_CENTER);
		this->moveSpeedSpin =
		    new wxSpinCtrl(moveSpeedPanel, wxID_ANY, wxEmptyString,
		                   wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0,
		                   (int) wxGetApp()
		                       .getSystemManager()
		                       ->getConfiguration()
		                       .getEntry(CalxConfiguration::Core)
		                       ->getInt(CalxMotorConfiguration::MaxSpeed, 4000),
		                   (int) wxGetApp()
		                       .getSystemManager()
		                       ->getConfiguration()
		                       .getEntry(CalxConfiguration::Core)
		                       ->getInt(CalxMotorConfiguration::MaxSpeed, 4000));
		moveSpeedSizer->Add(moveSpeedSpin, 1, wxEXPAND);
		moveSpeedPanel->SetSizer(moveSpeedSizer);
		moveSizer->Add(moveSpeedPanel, 1, wxEXPAND);

		wxPanel *moveCtrlPanel = new wxPanel(movePanel, wxID_ANY);
		wxBoxSizer *moveCtrlSizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton *moveButton = new wxButton(moveCtrlPanel, wxID_ANY, __("Move"));
		moveButton->Bind(wxEVT_BUTTON, &CalxMotorComponent::moveClick, this);
		moveCtrlSizer->Add(moveButton, 1, wxEXPAND);

		wxButton *rmoveButton =
		    new wxButton(moveCtrlPanel, wxID_ANY, __("Relative Move"));
		rmoveButton->Bind(wxEVT_BUTTON, &CalxMotorComponent::rmoveClick, this);
		moveCtrlSizer->Add(rmoveButton, 1, wxEXPAND);

		moveCtrlPanel->SetSizer(moveCtrlSizer);
		moveSizer->Add(moveCtrlPanel, 0, wxALL | wxEXPAND);

		movePanel->SetSizer(moveSizer);
		sizer->Add(movePanel, 0, wxALL, 10);

		// Action panel layout
		this->actionPanel = new wxPanel(box, wxID_ANY);
		wxBoxSizer *actionSizer = new wxBoxSizer(wxVERTICAL);

		wxButton *switchPowerButton =
		    new wxButton(actionPanel, wxID_ANY, __("Switch Power"));
		switchPowerButton->Bind(wxEVT_BUTTON, &CalxMotorComponent::switchPowerClick,
		                        this);
		actionSizer->Add(switchPowerButton, 1, wxEXPAND);

		wxButton *roll1Button =
		    new wxButton(actionPanel, wxID_ANY, __("Roll to trailer 1"));
		roll1Button->Bind(wxEVT_BUTTON, &CalxMotorComponent::rollToTrailer1, this);
		actionSizer->Add(roll1Button, 1, wxEXPAND);

		wxButton *roll2Button =
		    new wxButton(actionPanel, wxID_ANY, __("Roll to trailer 2"));
		roll2Button->Bind(wxEVT_BUTTON, &CalxMotorComponent::rollToTrailer2, this);
		actionSizer->Add(roll2Button, 1, wxEXPAND);

		this->stopButton = new wxButton(actionPanel, wxID_ANY, __("Stop"));
		stopButton->Bind(wxEVT_BUTTON, &CalxMotorComponent::stopClick, this);
		actionSizer->Add(stopButton, 1, wxEXPAND);

		wxButton *configButton =
		    new wxButton(actionPanel, wxID_ANY, __("Configure"));
		configButton->Bind(wxEVT_BUTTON, &CalxMotorComponent::OnConfigEvent, this);
		actionSizer->Add(configButton, 1, wxEXPAND);

		actionPanel->SetSizer(actionSizer);
		sizer->Add(actionPanel, 0, wxALL, 10);

		this->Bind(wxEVT_COMMAND_QUEUE_UPDATE, &CalxMotorComponent::threadUpdate,
		           this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxMotorComponent::OnExit, this);
		Bind(wxEVT_MOTOR_CTRL_ENABLE, &CalxMotorComponent::OnEnableEvent, this);
		// Post init
		this->dev->addEventListener(this->listener);
		this->queue->Run();
		updateUI();
		setEnabled(true);
		this->timer.setCtrl(this);
		this->timer.Start(100);
	}

	void CalxMotorComponent::setEnabled(bool e) {
		wxThreadEvent evt(wxEVT_MOTOR_CTRL_ENABLE);
		evt.SetPayload(e);
		wxPostEvent(this, evt);
	}

	DeviceController *CalxMotorComponent::getController() {
		return this->dev.get();
	}

	void CalxMotorComponent::updateUI() {
		std::string pos = __("Position") + std::string(": ") +
		                  std::to_string(this->dev->getPosition());
		std::string pwr =
		    __("Power") + std::string(": ") +
		    std::string(dev->getMotor().getPowerState() == Power::FullPower
		                    ? __("full")
		                    : (dev->getMotor().getPowerState() == Power::HalfPower
		                           ? __("half")
		                           : __("no")));
		std::string stat =
		    __("State") + std::string(": ") +
		    std::string(dev->getMotor().isRunning() ? __("Running")
		                                            : __("Not running"));
		std::string tra1 =
		    __("Trailer 1") + std::string(": ") +
		    std::string(dev->getMotor().isTrailerPressed(1) ? __("Pushed")
		                                                    : __("Unpushed"));
		std::string tra2 =
		    __("Trailer 2") + std::string(": ") +
		    std::string(dev->getMotor().isTrailerPressed(2) ? __("Pushed")
		                                                    : __("Unpushed"));
		std::string hwinf = dev->getMotor().getDeviceInfo();
		std::string rtinf = dev->getMotor().getRuntimeInfo();
		this->position->SetLabel(pos);
		this->power->SetLabel(pwr);
		this->state->SetLabel(stat);
		this->trailer1->SetLabel(tra1);
		this->trailer2->SetLabel(tra2);
		this->hardwareInfo->SetLabel(hwinf);
		this->runtimeInfo->SetLabel(rtinf);
		Layout();
	}

	void CalxMotorComponent::stop() {
		this->timer.Stop();
		this->queue->stop();
	}

	bool CalxMotorComponent::isBusy() {
		return this->queue->isBusy();
	}

	ErrorCode CalxMotorComponent::setPower(bool pw) {
		return dev->enablePower(pw);
	}

	void CalxMotorComponent::switchPowerClick(wxCommandEvent &evt) {
		wxGetApp().getErrorHandler()->handle(dev->flipPower());
	}

	ErrorCode CalxMotorComponent::roll(TrailerId tr, ActionResult *act_res) {
		this->queue->addAction(std::make_unique<CalxMotorCalibrationAction>(
		    this, dev.get(), tr, act_res));
		return ErrorCode::NoError;
	}

	void CalxMotorComponent::rollToTrailer1(wxCommandEvent &evt) {
		this->roll(TrailerId::Trailer1);
	}

	void CalxMotorComponent::rollToTrailer2(wxCommandEvent &evt) {
		this->roll(TrailerId::Trailer2);
	}

	void CalxMotorComponent::stopMovement() {
		this->queue->stopCurrent();
	}

	void CalxMotorComponent::stopClick(wxCommandEvent &evt) {
		this->stopMovement();
	}

	ErrorCode CalxMotorComponent::move(motor_coord_t dest, float speed,
	                                   bool relative, ActionResult *act_res) {
		this->queue->addAction(std::make_unique<CalxMotorMoveAction>(
		    this, dev.get(), dest, speed, relative, act_res));
		return ErrorCode::NoError;
	}

	void CalxMotorComponent::moveClick(wxCommandEvent &evt) {
		this->move(this->moveSpin->GetValue(), this->moveSpeedSpin->GetValue(),
		           false);
	}

	void CalxMotorComponent::rmoveClick(wxCommandEvent &evt) {
		this->move(this->moveSpin->GetValue(), this->moveSpeedSpin->GetValue(),
		           true);
	}

	void CalxMotorComponent::threadUpdate(wxThreadEvent &evt) {}

	void CalxMotorComponent::OnExit(wxCloseEvent &evt) {
		this->dev->removeEventListener(this->listener);
		this->dev->stop();
		Destroy();
	}

	void CalxMotorComponent::OnEnableEvent(wxThreadEvent &evt) {
		bool e = evt.GetPayload<bool>();
		movePanel->Enable(e);
		for (auto i = actionPanel->GetChildren().begin();
		     i != actionPanel->GetChildren().end(); ++i) {
			if (*i != this->stopButton) {
				(*i)->Enable(e);
			}
		}
		this->stopButton->Enable(!e && this->queue->isBusy());
	}

	void CalxMotorComponent::OnConfigEvent(wxCommandEvent &evt) {
		CalxConfigDialog *editor = new CalxConfigDialog(
		    this, wxID_ANY, this->dev->getMotor().getConfiguration());
		editor->ShowModal();
		delete editor;
	}
}  // namespace CalXUI
