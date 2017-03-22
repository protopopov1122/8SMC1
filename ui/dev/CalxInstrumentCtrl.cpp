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
	
	CalxInstrumentCtrl::CalxInstrumentCtrl(wxWindow *win, wxWindowID id, InstrumentController *ctrl)
		: wxPanel::wxPanel(win, id) {
			
		this->ctrl = ctrl;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		
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
		
		this->queue->Run();
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
		Destroy();
	}
	
	void CalxInstrumentCtrl::OnEnableButton(wxCommandEvent &evt) {
		ctrl->flipState();
		updateUI();
	}
	
	void CalxInstrumentCtrl::OnStateButton(wxCommandEvent &evt) {
		ctrl->setRunnable(!ctrl->isRunnable());
		updateUI();
	}
}