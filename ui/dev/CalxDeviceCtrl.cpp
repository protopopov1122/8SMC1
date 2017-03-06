#include "CalxDeviceCtrl.h"
#include "CalxApp.h"
#include "ctrl-lib/SystemManager.h"
#include "device/DeviceManager.h"
#include <wx/statbox.h>
#include <limits.h>
#include <iostream>

using namespace _8SMC1;

namespace CalX {
	
	void CalxDeviceTimer::Notify() {
		ctrl->updateUI();
	}

	CalxMotorEventListener::CalxMotorEventListener(CalxDeviceCtrl *ctrl) {
		this->dev = ctrl;
		this->used = 0;
	}
	
	CalxMotorEventListener::~CalxMotorEventListener() {
		
	}
	
	void CalxMotorEventListener::use() {
		if (this->used == 0) {
			dev->Enable(false);
		}
		this->used++;
	}
	
	void CalxMotorEventListener::unuse() {
		this->used--;
		if (this->used == 0) {
			dev->Enable(true);
		}
	}

	class CalxDevMoveAction : public CalxAction {
		public:
			CalxDevMoveAction(DeviceController *dev, int dest, float speed, bool rel) {
				this->dev = dev;
				this->dest = dest;
				this->speed = speed;
				this->rel = rel;
			}
			
			virtual ~CalxDevMoveAction() {
				
			}
			
			virtual void perform(SystemManager *sysman) {
				if (rel) {
					dev->startRelativeMove(dest, speed, 8);
				} else {
					dev->startMove(dest, speed, 8);
				}
			}
			
			virtual void stop() {
				dev->stop();
			}
		private:
			DeviceController *dev;
			int dest;
			float speed;
			bool rel;
	};

	class CalxDevCalAction : public CalxAction {
		public:
			CalxDevCalAction(DeviceController *dev, int tr) {
				this->dev = dev;
				this->tr = tr;
			}
			
			virtual ~CalxDevCalAction() {
				
			}
			
			virtual void perform(SystemManager *sysman) {
				dev->moveToTrailer(tr, TRAILER_COMEBACK);
			}
			
			virtual void stop() {
				dev->stop();
			}
		private:
			DeviceController *dev;
			int tr;
	};
	
	CalxDeviceCtrl::CalxDeviceCtrl(wxWindow *win, wxWindowID id, _8SMC1::DeviceController *dev)
		: wxPanel::wxPanel(win, id) {
		this->dev = dev;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->listener = new CalxMotorEventListener(this);
		
		wxStaticBox *box = new wxStaticBox(this, wxID_ANY, "Device #" + std::to_string(this->dev->getID()));
		wxStaticBoxSizer *sizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
		this->SetSizer(sizer);
		
		// Info panel elements
		wxPanel *infoPanel = new wxPanel(box, wxID_ANY);
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
		wxButton *updateInfoButton = new wxButton(infoPanel, wxID_ANY, "Update");
		updateInfoButton->Bind(wxEVT_BUTTON, &CalxDeviceCtrl::updateButtonClick, this);
		infoSizer->Add(updateInfoButton, 1, wxEXPAND);
		infoPanel->SetSizer(infoSizer);
		sizer->Add(infoPanel, 0, wxALL, 10);
		
		// Move panel layout
		wxPanel *movePanel = new wxPanel(box, wxID_ANY);
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
		this->moveSpeedSpin = new wxSpinCtrl(moveSpeedPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,  wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		moveSpeedSizer->Add(moveSpeedSpin, 1, wxEXPAND);
		moveSpeedPanel->SetSizer(moveSpeedSizer);
		moveSizer->Add(moveSpeedPanel, 1, wxEXPAND);
		
		
		wxPanel *moveCtrlPanel = new wxPanel(movePanel, wxID_ANY);
		wxBoxSizer *moveCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
		
		wxButton *moveButton = new wxButton(moveCtrlPanel, wxID_ANY, "Move");
		moveButton->Bind(wxEVT_BUTTON, &CalxDeviceCtrl::moveClick, this);
		moveCtrlSizer->Add(moveButton, 1, wxEXPAND);
		
		wxButton *rmoveButton = new wxButton(moveCtrlPanel, wxID_ANY, "Relative Move");
		rmoveButton->Bind(wxEVT_BUTTON, &CalxDeviceCtrl::rmoveClick, this);
		moveCtrlSizer->Add(rmoveButton, 1, wxEXPAND);
		
		moveCtrlPanel->SetSizer(moveCtrlSizer);
		moveSizer->Add(moveCtrlPanel, 0, wxALL | wxEXPAND);
		
		movePanel->SetSizer(moveSizer);
		sizer->Add(movePanel, 0, wxALL, 10);
		
		// Action panel layout
		wxPanel *actionPanel = new wxPanel(box, wxID_ANY);
		wxBoxSizer *actionSizer = new wxBoxSizer(wxVERTICAL);
		
		wxButton *switchPowerButton = new wxButton(actionPanel, wxID_ANY, "Switch Power");
		switchPowerButton->Bind(wxEVT_BUTTON, &CalxDeviceCtrl::switchPowerClick, this);
		actionSizer->Add(switchPowerButton, 1, wxEXPAND);
		
		wxButton *roll1Button = new wxButton(actionPanel, wxID_ANY, "Roll to trailer 1");
		roll1Button->Bind(wxEVT_BUTTON, &CalxDeviceCtrl::rollToTrailer1, this);
		actionSizer->Add(roll1Button, 1, wxEXPAND);
		
		wxButton *roll2Button = new wxButton(actionPanel, wxID_ANY, "Roll to trailer 2");
		roll2Button->Bind(wxEVT_BUTTON, &CalxDeviceCtrl::rollToTrailer2, this);
		actionSizer->Add(roll2Button, 1, wxEXPAND);
		
		wxButton *stopButton = new wxButton(actionPanel, wxID_ANY, "Stop");
		stopButton->Bind(wxEVT_BUTTON, &CalxDeviceCtrl::stopClick, this);
		actionSizer->Add(stopButton, 1, wxEXPAND);
		
		actionPanel->SetSizer(actionSizer);
		sizer->Add(actionPanel, 0, wxALL, 10);

		
		this->Bind(wxEVT_COMMAND_QUEUE_UPDATE, &CalxDeviceCtrl::threadUpdate, this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxDeviceCtrl::OnExit, this);
		// Post init
		this->dev->addEventListener(this->listener);
		this->queue->Run();
		updateUI();
		this->timer.setCtrl(this);
		this->timer.Start(100);
	}
	
	void CalxDeviceCtrl::updateUI() {
		std::string pos = "Position: " + std::to_string(this->dev->getPosition());
		std::string pwr = "Power: " + std::string(dev->getDevice()->getPowerState() == Power::FullPower ?
								"full" : (dev->getDevice()->getPowerState() == Power::HalfPower ?
								"half" : "no"));
		std::string stat = "State: " + std::string(dev->getDevice()->isRunning() ? "Running" : "Not running") +
										std::string(dev->getDevice()->isRunning() && dev->getDevice()->hasFullSpeed() ? " at full speed" : "");
		std::string tra1 = "Trailer 1: " + std::string(dev->getDevice()->isTrailerPressed(1) ? "Pushed" : "Unpushed");
		std::string tra2 = "Trailer 2: " + std::string(dev->getDevice()->isTrailerPressed(2) ? "Pushed" : "Unpushed");
		this->position->SetLabel(pos);
		this->power->SetLabel(pwr);
		this->state->SetLabel(stat);
		this->trailer1->SetLabel(tra1);
		this->trailer2->SetLabel(tra2);
	}
	
	void CalxDeviceCtrl::stop() {
		this->queue->stop();
	}
	
	void CalxDeviceCtrl::updateButtonClick(wxCommandEvent &evt) {
		this->updateUI();
	}
	
	void CalxDeviceCtrl::switchPowerClick(wxCommandEvent &evt) {
		dev->getDevice()->flipPower();
		updateUI();
	}
	
	void CalxDeviceCtrl::rollToTrailer1(wxCommandEvent &evt) {
		this->queue->addAction(new CalxDevCalAction(dev, 1));
	}
	
	void CalxDeviceCtrl::rollToTrailer2(wxCommandEvent &evt) {
		this->queue->addAction(new CalxDevCalAction(dev, 2));
	}
	
	void CalxDeviceCtrl::stopClick(wxCommandEvent &evt) {
		dev->stop();
		updateUI();
	}
	
	void CalxDeviceCtrl::moveClick(wxCommandEvent &evt) {
		this->queue->addAction(new CalxDevMoveAction(dev, this->moveSpin->GetValue(),
			this->moveSpeedSpin->GetValue(), false));
	}
	
	void CalxDeviceCtrl::rmoveClick(wxCommandEvent &evt) {
		this->queue->addAction(new CalxDevMoveAction(dev, this->moveSpin->GetValue(),
			this->moveSpeedSpin->GetValue(), true));
	}
	
	void CalxDeviceCtrl::threadUpdate(wxThreadEvent &evt) {
	}
	
	void CalxDeviceCtrl::OnExit(wxCloseEvent &evt) {
		this->timer.Stop();
		this->dev->removeEventListener(this->listener);
		this->dev->stop();
	}
}