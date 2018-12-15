/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#include "ui/dev/CalxInstrumentComponent.h"
#include "ui/CalxErrorHandler.h"
#include "ui/config/CalxConfigEditor.h"
#include <wx/sizer.h>

namespace CalXUI {

	wxDEFINE_EVENT(wxEVT_INSTRUMENT_CTRL_ENABLE, wxThreadEvent);

	class CalxInstrumentEventListener : public InstrumentEventListener {
	 public:
		CalxInstrumentEventListener(CalxInstrumentComponent *ctrl) {
			this->ctrl = ctrl;
			this->used = 0;
		}

		void onUse() override {
			if (used == 0) {
				wxThreadEvent evt(wxEVT_INSTRUMENT_CTRL_ENABLE);
				evt.SetPayload(false);
				wxPostEvent(ctrl, evt);
			}
			used++;
		}

		void onUnuse() override {
			used--;
			if (used == 0) {
				wxThreadEvent evt(wxEVT_INSTRUMENT_CTRL_ENABLE);
				evt.SetPayload(true);
				wxPostEvent(ctrl, evt);
			}
		}

	 private:
		CalxInstrumentComponent *ctrl;
		int used;
	};

	class CalxInstrumentTimer : public wxTimer {
	 public:
		CalxInstrumentTimer(CalxInstrumentComponent *ctrl) {
			this->ctrl = ctrl;
		}

		virtual void Notify() {
			ctrl->updateUI();
		}

	 private:
		CalxInstrumentComponent *ctrl;
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

		virtual void stop() {}

	 private:
		InstrumentController *ctrl;
	};

	class CalxInstrumentSessionAction : public CalxAction {
	 public:
		CalxInstrumentSessionAction(InstrumentController *ctrl) {
			this->ctrl = ctrl;
		}

		virtual void perform(SystemManager *sysman) {
			ctrl->use();
			if (ctrl->isSessionOpened()) {
				wxGetApp().getErrorHandler()->handle(ctrl->close_session());
			} else {
				wxGetApp().getErrorHandler()->handle(ctrl->open_session());
			}
			ctrl->unuse();
		}

		virtual void stop() {}

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

		virtual void stop() {}

	 private:
		InstrumentController *ctrl;
	};

	class CalxInstrumentModeAction : public CalxAction {
	 public:
		CalxInstrumentModeAction(InstrumentController *ctrl, InstrumentMode m) {
			this->ctrl = ctrl;
			this->mode = m;
		}

		virtual void perform(SystemManager *sysman) {
			ctrl->use();
			this->ctrl->setMode(this->mode);
			ctrl->unuse();
		}

		virtual void stop() {}

	 private:
		InstrumentController *ctrl;
		InstrumentMode mode;
	};

	CalxInstrumentComponent::CalxInstrumentComponent(
	    wxWindow *win, wxWindowID id, std::shared_ptr<InstrumentController> ctrl)
	    : CalxDeviceHandle::CalxDeviceHandle(win, id) {
		this->ctrl = ctrl;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->listener = std::make_shared<CalxInstrumentEventListener>(this);

		wxStaticBox *box = new wxStaticBox(
		    this, wxID_ANY,
		    FORMAT(__("Instrument #%s"), std::to_string(ctrl->getID())));
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
		sessionButton = new wxButton(ctrlPanel, wxID_ANY, __("Open/Close session"));
		ctrlSizer->Add(sessionButton, 0, wxALL | wxEXPAND);
		sessionButton->Bind(wxEVT_BUTTON, &CalxInstrumentComponent::OnSessionSwitch,
		                    this);
		enabledButton = new wxButton(ctrlPanel, wxID_ANY, __("Enable/Disable"));
		ctrlSizer->Add(enabledButton, 0, wxALL | wxEXPAND);
		enabledButton->Bind(wxEVT_BUTTON, &CalxInstrumentComponent::OnEnableButton,
		                    this);
		wxButton *confButton =
		    new wxButton(ctrlPanel, wxID_ANY, __("Configuration"));
		ctrlSizer->Add(confButton, 0, wxALL | wxEXPAND);
		confButton->Bind(wxEVT_BUTTON, &CalxInstrumentComponent::OnConfClick, this);

		wxPanel *modePanel = new wxPanel(mainPanel, wxID_ANY);
		mainSizer->Add(modePanel, 0, wxALL, 10);
		wxBoxSizer *modeSizer = new wxBoxSizer(wxHORIZONTAL);
		modePanel->SetSizer(modeSizer);
		modeSizer->Add(
		    new wxStaticText(modePanel, wxID_ANY, __("Mode") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_CENTER, 5);
		this->modeChoice = new wxChoice(modePanel, wxID_ANY);
		modeSizer->Add(this->modeChoice, 0, wxALIGN_CENTER);
		this->modeChoice->Append(__("Off"));
		this->modeChoice->Append(__("Adjustment"));
		this->modeChoice->Append(__("Maximal"));
		this->modeChoice->SetSelection(
		    this->ctrl->getMode() == InstrumentMode::Off
		        ? 0
		        : (this->ctrl->getMode() == InstrumentMode::Prepare ? 1 : 2));
		this->modeChoice->Bind(wxEVT_CHOICE, &CalxInstrumentComponent::OnModeClick,
		                       this);
		updateUI();
		Layout();

		ctrl->addEventListener(listener);
		this->queue->Run();
		this->timer = new CalxInstrumentTimer(this);
		timer->Start(100);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxInstrumentComponent::OnExit, this);
		this->Bind(wxEVT_INSTRUMENT_CTRL_ENABLE,
		           &CalxInstrumentComponent::OnEnableEvent, this);
	}

	DeviceController *CalxInstrumentComponent::getController() {
		return this->ctrl.get();
	}

	void CalxInstrumentComponent::stop() {
		timer->Stop();
		this->queue->stop();
	}

	void CalxInstrumentComponent::stopInstrument() {
		if (this->ctrl->isSessionOpened()) {
			this->ctrl->close_session();
		}
		this->queue->stopCurrent();
	}

	bool CalxInstrumentComponent::isBusy() {
		return this->queue->isBusy();
	}

	void CalxInstrumentComponent::updateUI() {
		std::string text =
		    __("Session") + std::string(": ") +
		    (this->ctrl->isSessionOpened() ? __("Opened") : __("Closed")) +
		    std::string("\n") + __("Enabled") + std::string(": ") +
		    std::string(ctrl->isEnabled() ? __("true") : __("false"));
		this->infoText->SetLabel(text);
		sessionButton->Enable(ctrl->isRunnable());
		enabledButton->Enable(ctrl->isRunnable());
		this->Layout();
	}

	void CalxInstrumentComponent::OnExit(wxCloseEvent &evt) {
		ctrl->removeEventListener(listener);
		Destroy();
	}

	void CalxInstrumentComponent::OnEnableButton(wxCommandEvent &evt) {
		queue->addAction(
		    std::make_unique<CalxInstrumentEnableAction>(this->ctrl.get()));
	}

	void CalxInstrumentComponent::OnSessionSwitch(wxCommandEvent &evt) {
		queue->addAction(
		    std::make_unique<CalxInstrumentSessionAction>(this->ctrl.get()));
	}

	void CalxInstrumentComponent::OnConfClick(wxCommandEvent &evt) {
		CalxConfigDialog *editor = new CalxConfigDialog(
		    this, wxID_ANY, this->ctrl->getInstrument().getConfiguration());
		editor->ShowModal();
		delete editor;
	}

	void CalxInstrumentComponent::OnModeClick(wxCommandEvent &evt) {
		if (this->modeChoice->GetSelection() != wxNOT_FOUND) {
			int sel = this->modeChoice->GetSelection();
			InstrumentMode mode = sel == 0 ? InstrumentMode::Off
			                               : (sel == 1 ? InstrumentMode::Prepare
			                                           : InstrumentMode::Full);
			queue->addAction(
			    std::make_unique<CalxInstrumentModeAction>(this->ctrl.get(), mode));
		}
	}

	void CalxInstrumentComponent::OnEnableEvent(wxThreadEvent &evt) {
		bool e = evt.GetPayload<bool>();
		Enable(e);
	}
}  // namespace CalXUI
