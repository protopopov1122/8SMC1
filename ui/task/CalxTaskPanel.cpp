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


#include <iostream>
#include <fstream>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "coord/CalxCoordPanel.h"
#include "CalxTaskPanel.h"
#include "CalxGcodeLoader.h"

namespace CalXUI {
	
	class CalxTaskAction : public CalxAction {
		public:
			CalxTaskAction(CalxTaskPanel *panel, CoordHandle *handle, CoordTask *task, TaskParameters prms) {
				this->panel = panel;
				this->handle = handle;
				this->task = task;
				this->prms = prms;
			}
			
			virtual void perform(SystemManager *sysman) {
				panel->setEnabled(false);
				task->perform(handle, prms, sysman, &state);
				panel->setEnabled(true);
			}
			
			virtual void stop() {
				state.stop();
			}
		private:
			CalxTaskPanel *panel;
			CoordHandle *handle;
			CoordTask *task;
			TaskParameters prms;
			TaskState state;
	};
	
	CalxTaskPanel::CalxTaskPanel(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->queue->Run();
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		
		this->taskPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *taskSizer = new wxBoxSizer(wxVERTICAL);
		taskPanel->SetSizer(taskSizer);
		sizer->Add(taskPanel, 0, wxALL | wxEXPAND, 5);
		this->taskList = new wxListBox(taskPanel, wxID_ANY);
		taskSizer->Add(this->taskList, 1, wxALL | wxEXPAND);
		wxButton *newGcodeButton = new wxButton(taskPanel, wxID_ANY, "Load GCode");
		taskSizer->Add(newGcodeButton, 0, wxALL | wxEXPAND);
		wxButton *removeButton = new wxButton(taskPanel, wxID_ANY, "Remove");
		taskSizer->Add(removeButton, 0, wxALL | wxEXPAND);
		newGcodeButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnNewGcodeClick, this);
		removeButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnRemoveClick, this);
		
		this->mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxALL | wxEXPAND);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		mainPanel->SetSizer(mainSizer);
		
		this->execPanel = new wxPanel(this->mainPanel, wxID_ANY);
		mainSizer->Add(execPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *execSizer = new wxBoxSizer(wxHORIZONTAL);
		execPanel->SetSizer(execSizer);
		wxButton *buildButton = new wxButton(execPanel, wxID_ANY, "Build");
		execSizer->Add(buildButton);
		this->plane = new wxChoice(execPanel, wxID_ANY);
		this->speed = new wxSpinCtrl(execPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 4000, 4000);
		execSizer->Add(new wxStaticText(execPanel, wxID_ANY, " on "), 0, wxALL | wxALIGN_CENTER);
		execSizer->Add(plane, 0, wxALL, 5);
		execSizer->Add(new wxStaticText(execPanel, wxID_ANY, " with speed "), 0, wxALL | wxALIGN_CENTER);
		execSizer->Add(speed, 0, wxALL, 5);
		this->stopButton = new wxButton(execPanel, wxID_ANY, "Stop");
		execSizer->Add(stopButton);
		buildButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnBuildClick, this);
		stopButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnStopClick, this);
		
		SetSizer(sizer);
		Layout();
		setEnabled(true);
		
		taskList->Bind(wxEVT_LISTBOX, &CalxTaskPanel::OnListClick, this);		
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxTaskPanel::OnExit, this);	
	}
	
	void CalxTaskPanel::stop() {
		this->queue->stop();
	}
	
	void CalxTaskPanel::updateUI() {
		for (const auto& t : this->list) {
			t->Show(false);
		}
		if (taskList->GetSelection() != wxNOT_FOUND) {
			this->list.at(taskList->GetSelection())->Show(true);
		}
		plane->Clear();
		CalxPanel *calxPanel = wxGetApp().getMainFrame()->getPanel();
		for (size_t i = 0; i < calxPanel->getCoords()->getCoordCount(); i++) {
			if (calxPanel->getCoords()->getCoordCtrl(i)->isUsed()) {
				continue;
			}
			CoordHandle *handle = calxPanel->getCoords()->getCoord(i);
			plane->Append("Plane #" + std::to_string(handle->getID()));
		}
		if (calxPanel->getCoords()->getCoordCount() > 0) {
			plane->SetSelection(0);
		}
		Layout();
	}
	
	void CalxTaskPanel::setEnabled(bool e) {
		for (const auto& h : list) {
			h->Enable(e);
		}
		taskPanel->Enable(e);
		for (auto i = execPanel->GetChildren().begin(); i != execPanel->GetChildren().end(); ++i) {
			if (*i != this->stopButton) {
				(*i)->Enable(e);
			}
		}
		this->stopButton->Enable(!e);
	}
	
	void CalxTaskPanel::OnExit(wxCloseEvent &evt) {
		for (const auto& h : list) {
			h->Close(true);
		}
	}
	
	void CalxTaskPanel::OnNewGcodeClick(wxCommandEvent &evt) {
		CalxGcodeLoader *loader = new CalxGcodeLoader(this, wxID_ANY);
		loader->ShowModal();
		if (loader->isLoaded()) {
			std::fstream is(loader->getPath());
			CalxGcodeHandle *handle = new CalxGcodeHandle(mainPanel, wxID_ANY, loader->getPath(), &is, loader->getTranslator());
			is.close();
			
			list.push_back(handle);
			taskList->Append(handle->getId());
			mainPanel->GetSizer()->Add(handle, 1, wxALL | wxEXPAND, 5);
			taskList->SetSelection(list.size() - 1);
			Layout();
		}
		loader->Destroy();
		updateUI();
	}
	
	void CalxTaskPanel::OnRemoveClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND) {
			size_t sel = taskList->GetSelection();
			taskList->Delete(sel);
			this->list.at(sel)->Close(true);
			this->list.erase(this->list.begin() + sel);
		}
		updateUI();
	}
	
	void CalxTaskPanel::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}
	
	void CalxTaskPanel::OnBuildClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND
			&& plane->GetSelection() != wxNOT_FOUND) {
			CoordTask *task = list.at(taskList->GetSelection())->getTask();
			CoordHandle *handle = wxGetApp().getMainFrame()->getPanel()->getCoords()->getCoord(plane->GetSelection());
			TaskParameters prms = {(float) this->speed->GetValue()};
			queue->addAction(new CalxTaskAction(this, handle, task, prms));
		}
	}
	
	void CalxTaskPanel::OnStopClick(wxCommandEvent &evt) {
		this->queue->stopCurrent();
	}
}