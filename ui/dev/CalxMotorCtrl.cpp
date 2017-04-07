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


#include "CalxApp.h"
#include "CalxErrorHandler.h"
#include "CalxMotorCtrl.h"
#include "ctrl-lib/SystemManager.h"
#include "device/DeviceManager.h"
#include <wx/statbox.h>
#include <limits.h>
#include <iostream>

using namespace CalX;

namespace CalXUI {
	
	void CalxMotorTimer::Notify() {
		ctrl->updateUI();
	}

	CalxMotorEventListener::CalxMotorEventListener(CalxMotorCtrl *ctrl) {
		this->dev = ctrl;
		this->used = 0;
	}
	
	CalxMotorEventListener::~CalxMotorEventListener() {
		
	}
	
	void CalxMotorEventListener::use() {
		if (this->used == 0) {
			dev->setEnabled(false);
		}
		this->used++;
	}
	
	void CalxMotorEventListener::unuse() {
		this->used--;
		if (this->used == 0) {
			dev->setEnabled(true);
		}
	}

	class CalxMotorMoveAction : public CalxAction {
		public:
			CalxMotorMoveAction(CalxMotorCtrl *ctrl, MotorController *dev, int dest, float speed, bool rel) {
				this->ctrl = ctrl;
				this->dev = dev;
				this->dest = dest;
				this->speed = speed;
				this->rel = rel;
			}
			
			virtual ~CalxMotorMoveAction() {
				
			}
			
			virtual void perform(SystemManager *sysman) {
				ctrl->setMaster(true);
				if (rel) {
					wxGetApp().getErrorHandler()->handle(dev->startRelativeMove(dest, speed, 8));
				} else {
					wxGetApp().getErrorHandler()->handle(dev->startMove(dest, speed, 8));
				}
				ctrl->setMaster(false);
			}
			
			virtual void stop() {
				dev->stop();
			}
		private:
			CalxMotorCtrl *ctrl;
			MotorController *dev;
			int dest;
			float speed;
			bool rel;
	};

	class CalxMotorCalibrationAction : public CalxAction {
		public:
			CalxMotorCalibrationAction(CalxMotorCtrl *ctrl, MotorController *dev, int tr) {
				this->ctrl = ctrl;
				this->dev = dev;
				this->tr = tr;
			}
			
			virtual ~CalxMotorCalibrationAction() {
				
			}
			
			virtual void perform(SystemManager *sysman) {
				ctrl->setMaster(true);
				wxGetApp().getErrorHandler()->handle(dev->moveToTrailer(tr));
				ctrl->setMaster(false);
			}
			
			virtual void stop() {
				dev->stop();
			}
		private:
			CalxMotorCtrl *ctrl;
			MotorController *dev;
			int tr;
	};
	
	CalxMotorCtrl::CalxMotorCtrl(wxWindow *win, wxWindowID id, CalX::MotorController *dev)
		: wxPanel::wxPanel(win, id) {
		this->dev = dev;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->listener = new CalxMotorEventListener(this);
		this->master = false;
		
		wxStaticBox *box = new wxStaticBox(this, wxID_ANY, "Device #" + std::to_string(this->dev->getID()));
		wxStaticBoxSizer *sizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
		this->SetSizer(sizer);
		
		// Info panel elements
		this->infoPanel = new wxPanel(box, wxID_ANY);
		wxBoxSizer *infoSizer = new wxBoxSizer(wxVERTICAL);
		this->position = new wxStaticText(infoPanel, wxID_ANY, "Position: ");
		infoSizer->Add(position);
		this->power = new wxStaticText(infoPanel, wxID_ANY, "Power: ");
		infoSizer->Add(power);
		this->state = new wxStaticText(infoPanel, wxID_ANY, "State: ");
		infoSizer->Add(state);
		this->trailer1 = new wxStaticText(infoPanel, wxID_ANY, "Trailer 1: ");
		infoSizer->Add(trailer1);
		this->trailer2 = new wxStaticText(infoPanel, wxID_ANY, "Trailer 2: ");
		infoSizer->Add(trailer2);
		infoPanel->SetSizer(infoSizer);
		sizer->Add(infoPanel, 0, wxALL, 10);
		
		// Move panel layout
		this->movePanel = new wxPanel(box, wxID_ANY);
		wxBoxSizer *moveSizer = new wxBoxSizer(wxVERTICAL);
		
		wxPanel *moveDestPanel = new wxPanel(movePanel, wxID_ANY);
		wxBoxSizer *moveDestSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *moveDestText = new wxStaticText(moveDestPanel, wxID_ANY, "Destination: ");
		moveDestSizer->Add(moveDestText, 0, wxALIGN_CENTER);
		moveSpin = new wxSpinCtrl(moveDestPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,  wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		moveDestSizer->Add(moveSpin, 1, wxEXPAND);
		moveDestPanel->SetSizer(moveDestSizer);
		moveSizer->Add(moveDestPanel, 1, wxEXPAND);
		
		wxPanel *moveSpeedPanel = new wxPanel(movePanel, wxID_ANY);
		wxBoxSizer *moveSpeedSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *moveSpeedText = new wxStaticText(moveSpeedPanel, wxID_ANY, "Speed: ");
		moveSpeedSizer->Add(moveSpeedText, 0, wxALIGN_CENTER);
		this->moveSpeedSpin = new wxSpinCtrl(moveSpeedPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,  wxDefaultSize, wxSP_ARROW_KEYS, 0, wxGetApp().getSystemManager()->getConfiguration()->getEntry("core")->getInt("maxspeed", 4000), wxGetApp().getSystemManager()->getConfiguration()->getEntry("core")->getInt("maxspeed", 4000));
		moveSpeedSizer->Add(moveSpeedSpin, 1, wxEXPAND);
		moveSpeedPanel->SetSizer(moveSpeedSizer);
		moveSizer->Add(moveSpeedPanel, 1, wxEXPAND);
		
		
		wxPanel *moveCtrlPanel = new wxPanel(movePanel, wxID_ANY);
		wxBoxSizer *moveCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
		
		wxButton *moveButton = new wxButton(moveCtrlPanel, wxID_ANY, "Move");
		moveButton->Bind(wxEVT_BUTTON, &CalxMotorCtrl::moveClick, this);
		moveCtrlSizer->Add(moveButton, 1, wxEXPAND);
		
		wxButton *rmoveButton = new wxButton(moveCtrlPanel, wxID_ANY, "Relative Move");
		rmoveButton->Bind(wxEVT_BUTTON, &CalxMotorCtrl::rmoveClick, this);
		moveCtrlSizer->Add(rmoveButton, 1, wxEXPAND);
		
		moveCtrlPanel->SetSizer(moveCtrlSizer);
		moveSizer->Add(moveCtrlPanel, 0, wxALL | wxEXPAND);
		
		movePanel->SetSizer(moveSizer);
		sizer->Add(movePanel, 0, wxALL, 10);
		
		// Action panel layout
		this->actionPanel = new wxPanel(box, wxID_ANY);
		wxBoxSizer *actionSizer = new wxBoxSizer(wxVERTICAL);
		
		wxButton *switchPowerButton = new wxButton(actionPanel, wxID_ANY, "Switch Power");
		switchPowerButton->Bind(wxEVT_BUTTON, &CalxMotorCtrl::switchPowerClick, this);
		actionSizer->Add(switchPowerButton, 1, wxEXPAND);
		
		wxButton *roll1Button = new wxButton(actionPanel, wxID_ANY, "Roll to trailer 1");
		roll1Button->Bind(wxEVT_BUTTON, &CalxMotorCtrl::rollToTrailer1, this);
		actionSizer->Add(roll1Button, 1, wxEXPAND);
		
		wxButton *roll2Button = new wxButton(actionPanel, wxID_ANY, "Roll to trailer 2");
		roll2Button->Bind(wxEVT_BUTTON, &CalxMotorCtrl::rollToTrailer2, this);
		actionSizer->Add(roll2Button, 1, wxEXPAND);
		
		this->stopButton = new wxButton(actionPanel, wxID_ANY, "Stop");
		stopButton->Bind(wxEVT_BUTTON, &CalxMotorCtrl::stopClick, this);
		actionSizer->Add(stopButton, 1, wxEXPAND);
		
		actionPanel->SetSizer(actionSizer);
		sizer->Add(actionPanel, 0, wxALL, 10);

		
		this->Bind(wxEVT_COMMAND_QUEUE_UPDATE, &CalxMotorCtrl::threadUpdate, this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxMotorCtrl::OnExit, this);
		// Post init
		this->dev->addEventListener(this->listener);
		this->queue->Run();
		updateUI();
		setEnabled(true);
		this->timer.setCtrl(this);
		this->timer.Start(100);
	}
	
	void CalxMotorCtrl::setEnabled(bool e) {
		movePanel->Enable(e);
		for (auto i = actionPanel->GetChildren().begin(); i != actionPanel->GetChildren().end(); ++i) {
			if (*i != this->stopButton) {
				(*i)->Enable(e);
			}
		}
		this->stopButton->Enable(!e && this->master);
	}
	
	void CalxMotorCtrl::setMaster(bool m) {
		this->master = m;
	}
	
	
	void CalxMotorCtrl::updateUI() {
		std::string pos = "Position: " + std::to_string(this->dev->getPosition());
		std::string pwr = "Power: " + std::string(dev->getMotor()->getPowerState() == Power::FullPower ?
								"full" : (dev->getMotor()->getPowerState() == Power::HalfPower ?
								"half" : "no"));
		std::string stat = "State: " + std::string(dev->getMotor()->isRunning() ? "Running" : "Not running");
		std::string tra1 = "Trailer 1: " + std::string(dev->getMotor()->isTrailerPressed(1) ? "Pushed" : "Unpushed");
		std::string tra2 = "Trailer 2: " + std::string(dev->getMotor()->isTrailerPressed(2) ? "Pushed" : "Unpushed");
		this->position->SetLabel(pos);
		this->power->SetLabel(pwr);
		this->state->SetLabel(stat);
		this->trailer1->SetLabel(tra1);
		this->trailer2->SetLabel(tra2);
	}
	
	void CalxMotorCtrl::stop() {
		this->queue->stop();
	}
	
	void CalxMotorCtrl::switchPowerClick(wxCommandEvent &evt) {
		dev->getMotor()->flipPower();
	}
	
	void CalxMotorCtrl::rollToTrailer1(wxCommandEvent &evt) {
		this->queue->addAction(new CalxMotorCalibrationAction(this, dev, 1));
	}
	
	void CalxMotorCtrl::rollToTrailer2(wxCommandEvent &evt) {
		this->queue->addAction(new CalxMotorCalibrationAction(this, dev, 2));
	}
	
	void CalxMotorCtrl::stopClick(wxCommandEvent &evt) {
		this->queue->stopCurrent();
	}
	
	void CalxMotorCtrl::moveClick(wxCommandEvent &evt) {
		this->queue->addAction(new CalxMotorMoveAction(this, dev, this->moveSpin->GetValue(),
			this->moveSpeedSpin->GetValue(), false));
	}
	
	void CalxMotorCtrl::rmoveClick(wxCommandEvent &evt) {
		this->queue->addAction(new CalxMotorMoveAction(this, dev, this->moveSpin->GetValue(),
			this->moveSpeedSpin->GetValue(), true));
	}
	
	void CalxMotorCtrl::threadUpdate(wxThreadEvent &evt) {
	}
	
	void CalxMotorCtrl::OnExit(wxCloseEvent &evt) {
		this->timer.Stop();
		this->dev->removeEventListener(this->listener);
		this->dev->stop();
		Destroy();
	}
}