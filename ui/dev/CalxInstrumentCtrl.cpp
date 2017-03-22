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


#include "CalxInstrumentCtrl.h"
#include <wx/sizer.h>

namespace CalXUI {
	
	class CalxInstrumentEventListener : public InstrumentEventListener  {
		public:
			CalxInstrumentEventListener(CalxInstrumentCtrl *ctrl) {
				this->ctrl = ctrl;
				this->used = 0;
			}
			
			virtual void use() {
				if (used == 0) {
					ctrl->Enable(false);
				}
				used++;
			}
			
			virtual void unuse() {
				used--;
				if (used == 0) {
					ctrl->Enable(true);
				}
			}
		private:
			CalxInstrumentCtrl *ctrl;
			int used;
	};
	
	class CalxInstrumentTimer : public wxTimer {
		public:
			CalxInstrumentTimer(CalxInstrumentCtrl *ctrl) {
				this->ctrl = ctrl;
			}
			
			virtual void Notify() {
				ctrl->updateUI();
			}
		private:
			CalxInstrumentCtrl *ctrl;
	};
	
	class CalxInstrumentEnableAction : public CalxAction {
		public:
			CalxInstrumentEnableAction(InstrumentController *ctrl) {
				this->ctrl = ctrl;
			}
			
			virtual void perform(SystemManager *sysman) {
				ctrl->flipState();
			}
			
			virtual void stop() {
				
			}
		private:
			InstrumentController *ctrl;
	};
	
	class CalxInstrumentStateAction : public CalxAction {
		public:
			CalxInstrumentStateAction(InstrumentController *ctrl) {
				this->ctrl = ctrl;
			}
			
			virtual void perform(SystemManager *sysman) {
				ctrl->setRunnable(!ctrl->isRunnable());
			}
			
			virtual void stop() {
				
			}
		private:
			InstrumentController *ctrl;
	};
	
	CalxInstrumentCtrl::CalxInstrumentCtrl(wxWindow *win, wxWindowID id, InstrumentController *ctrl)
		: wxPanel::wxPanel(win, id) {
			
		this->ctrl = ctrl;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->listener = new CalxInstrumentEventListener(this);
		
		wxStaticBox *box = new wxStaticBox(this, wxID_ANY, "Instrument #" + std::to_string(ctrl->getID()));
		wxStaticBoxSizer *sizer = new wxStaticBoxSizer(box, wxHORIZONTAL);
		SetSizer(sizer);
		
		wxPanel *infoPanel = new wxPanel(box, wxID_ANY);
		sizer->Add(infoPanel, 0, wxALL, 10);
		wxBoxSizer *infoSizer = new wxBoxSizer(wxVERTICAL);
		infoPanel->SetSizer(infoSizer);
		this->infoText = new wxStaticText(infoPanel, wxID_ANY, "");
		infoSizer->Add(infoText);
		
		wxPanel *ctrlPanel = new wxPanel(box, wxID_ANY);
		sizer->Add(ctrlPanel, 0, wxALL, 10);
		wxBoxSizer *ctrlSizer = new wxBoxSizer(wxVERTICAL);
		ctrlPanel->SetSizer(ctrlSizer);
		wxButton *stateButton = new wxButton(ctrlPanel, wxID_ANY, "Switch state");
		enabledButton = new wxButton(ctrlPanel, wxID_ANY, "Enable/Disable");
		ctrlSizer->Add(stateButton, 0, wxALL | wxEXPAND);
		ctrlSizer->Add(enabledButton, 0, wxALL | wxEXPAND);
		stateButton->Bind(wxEVT_BUTTON, &CalxInstrumentCtrl::OnStateButton, this);
		enabledButton->Bind(wxEVT_BUTTON, &CalxInstrumentCtrl::OnEnableButton, this);
		
		updateUI();
		Layout();
		
		ctrl->addEventListener(listener);
		this->queue->Run();
		this->timer = new CalxInstrumentTimer(this);
		timer->Start(100);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxInstrumentCtrl::OnExit, this);
	}
	
	void CalxInstrumentCtrl::stop() {
		this->queue->stop();
	}
	
	void CalxInstrumentCtrl::updateUI() {
		std::string text = "State: " + std::string(ctrl->isRunnable() ? "on" : "off") +
		"\nEnabled: " + std::string(ctrl->isEnabled() ? "true" : "false");
		this->infoText->SetLabel(text);
		enabledButton->Enable(ctrl->isRunnable());
	}
	
	void CalxInstrumentCtrl::OnExit(wxCloseEvent &evt) {
		timer->Stop();
		delete timer;
		ctrl->removeEventListener(listener);
		delete this->queue;
		Destroy();
	}
	
	void CalxInstrumentCtrl::OnEnableButton(wxCommandEvent &evt) {
		queue->addAction(new CalxInstrumentEnableAction(this->ctrl));
	}
	
	void CalxInstrumentCtrl::OnStateButton(wxCommandEvent &evt) {
		queue->addAction(new CalxInstrumentStateAction(this->ctrl));
	}
}