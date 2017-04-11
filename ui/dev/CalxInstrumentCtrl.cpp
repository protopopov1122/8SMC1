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
#include "CalxErrorHandler.h"
#include "CalxConfigEditor.h"
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
				ctrl->use();
				wxGetApp().getErrorHandler()->handle(ctrl->flipState());
				ctrl->unuse();
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
				ctrl->use();
				ctrl->setRunnable(!ctrl->isRunnable());
				ctrl->unuse();
			}
			
			virtual void stop() {
				
			}
		private:
			InstrumentController *ctrl;
	};
	
	class CalxInstrumentModeAction : public CalxAction {
		public:
			CalxInstrumentModeAction(InstrumentController *ctrl, size_t m) {
				this->ctrl = ctrl;
				this->mode = m;
			}
			
			virtual void perform(SystemManager *sysman) {
				ctrl->use();
				this->ctrl->setMode(this->mode);
				ctrl->unuse();
			}
			
			virtual void stop() {
				
			}
		private:
			InstrumentController *ctrl;
			size_t mode;
	};
	
	CalxInstrumentCtrl::CalxInstrumentCtrl(wxWindow *win, wxWindowID id, InstrumentController *ctrl)
		: wxPanel::wxPanel(win, id) {
			
		this->ctrl = ctrl;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->listener = new CalxInstrumentEventListener(this);
		
		wxStaticBox *box = new wxStaticBox(this, wxID_ANY, FORMAT(__("Instrument #%" DEVICE_ID_FMT), ctrl->getID()));
		wxStaticBoxSizer *sizer = new wxStaticBoxSizer(box, wxVERTICAL);
		SetSizer(sizer);
		
		this->instrInfoText = new wxStaticText(box, wxID_ANY, ctrl->getInfo());
		sizer->Add(instrInfoText, 0, wxLEFT | wxTOP, 5);
		
		wxPanel *mainPanel = new wxPanel(box, wxID_ANY);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
		mainPanel->SetSizer(mainSizer);
		sizer->Add(mainPanel);
		
		wxPanel *infoPanel = new wxPanel(mainPanel, wxID_ANY);
		mainSizer->Add(infoPanel, 0, wxALL, 10);
		wxBoxSizer *infoSizer = new wxBoxSizer(wxVERTICAL);
		infoPanel->SetSizer(infoSizer);
		this->infoText = new wxStaticText(infoPanel, wxID_ANY, "");
		infoSizer->Add(infoText);
		
		wxPanel *ctrlPanel = new wxPanel(mainPanel, wxID_ANY);
		mainSizer->Add(ctrlPanel, 0, wxALL, 10);
		wxBoxSizer *ctrlSizer = new wxBoxSizer(wxVERTICAL);
		ctrlPanel->SetSizer(ctrlSizer);
		wxButton *stateButton = new wxButton(ctrlPanel, wxID_ANY, __("Switch state"));
		enabledButton = new wxButton(ctrlPanel, wxID_ANY, __("Enable/Disable"));
		ctrlSizer->Add(stateButton, 0, wxALL | wxEXPAND);
		ctrlSizer->Add(enabledButton, 0, wxALL | wxEXPAND);
		stateButton->Bind(wxEVT_BUTTON, &CalxInstrumentCtrl::OnStateButton, this);
		enabledButton->Bind(wxEVT_BUTTON, &CalxInstrumentCtrl::OnEnableButton, this);
		wxButton *confButton = new wxButton(ctrlPanel, wxID_ANY, __("Configuration"));
		ctrlSizer->Add(confButton, 0, wxALL | wxEXPAND);
		confButton->Bind(wxEVT_BUTTON, &CalxInstrumentCtrl::OnConfClick, this);
		
		wxPanel *modePanel = new wxPanel(mainPanel, wxID_ANY);
		mainSizer->Add(modePanel, 0, wxALL, 10);
		wxBoxSizer *modeSizer = new wxBoxSizer(wxHORIZONTAL);
		modePanel->SetSizer(modeSizer);
		modeSizer->Add(new wxStaticText(modePanel, wxID_ANY, __("Mode") + std::string(":")), 0, wxRIGHT | wxALIGN_CENTER, 5);
		this->modeChoice = new wxChoice(modePanel, wxID_ANY);
		modeSizer->Add(this->modeChoice, 0, wxALIGN_CENTER);
		this->ctrl->getModes(modes);
		for (const auto& mode : modes) {
			this->modeChoice->Append(mode);
		}
		this->modeChoice->SetSelection(this->ctrl->getMode());
		this->modeChoice->Bind(wxEVT_CHOICE, &CalxInstrumentCtrl::OnModeClick, this);
		
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
		std::string text = __("State") + std::string(": ") + std::string(ctrl->isRunnable() ? __("on") : __("off")) +
		__("\nEnabled") + std::string(": ") + std::string(ctrl->isEnabled() ? __("true") : __("false"));
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
	
	void CalxInstrumentCtrl::OnModeClick(wxCommandEvent &evt) {
		if (this->modeChoice->GetSelection() != wxNOT_FOUND) {
			queue->addAction(new CalxInstrumentModeAction(this->ctrl, this->modeChoice->GetSelection()));
		}
	}
	
	void CalxInstrumentCtrl::OnConfClick(wxCommandEvent &evt) {
		CalxConfigDialog *editor = new CalxConfigDialog(this, wxID_ANY, this->ctrl->getConfiguration());
		if (editor->ShowModal() == wxID_OK) {
			
		}
		delete editor;
	}
}