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


#include <sstream>
#include <iostream>
#include <fstream>
#include "CalxApp.h"
#include "CalxErrorHandler.h"
#include "CalxTaskPanel.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/splitter.h>
#include "coord/CalxCoordPanel.h"
#include "coord/CalxVirtualPlane.h"
#include "ctrl-lib/misc/GCodeWriter.h"
#include "CalxGcodeLoader.h"

namespace CalXUI {

	wxDEFINE_EVENT(wxEVT_TASK_PANEL_ENABLE, wxThreadEvent);
	
	class CalxTaskAction : public CalxAction {
		public:
			CalxTaskAction(CalxTaskPanel *panel, CoordPlane *handle, CoordTask *task, TaskParameters prms) {
				this->panel = panel;
				this->handle = handle;
				this->task = task;
				this->prms = prms;
			}
			
			virtual void perform(SystemManager *sysman) {
				handle->open_session();
				panel->setEnabled(false);
				wxGetApp().getErrorHandler()->handle(task->perform(handle, prms, sysman, &state));
				panel->setEnabled(true);
				handle->close_session();
			}
			
			virtual void stop() {
				state.stop();
			}
		private:
			CalxTaskPanel *panel;
			CoordPlane *handle;
			CoordTask *task;
			TaskParameters prms;
			TaskState state;
	};
	
	class CalxPreviewAction : public CalxAction {
		public:
			CalxPreviewAction(CalxTaskPanel *panel, CalxVirtualPlaneDialog *dialog, CoordTask *task, TaskParameters prms) {
				this->panel = panel;
				this->dialog = dialog;
				this->task = task;
				this->prms = prms;
			}
			
			virtual void perform(SystemManager *sysman) {
				panel->setEnabled(false);
				wxGetApp().getErrorHandler()->handle(task->perform(dialog->getPlane(), prms, sysman, &state));
				dialog->Refresh();
				dialog->Enable(true);
				panel->setEnabled(true);
			}
			
			virtual void stop() {
				state.stop();
			}
		private:
			CalxTaskPanel *panel;
			CalxVirtualPlaneDialog *dialog;
			CoordTask *task;
			TaskParameters prms;
			TaskState state;
	};
	
	CalxTaskPanel::CalxTaskPanel(wxWindow *win, wxWindowID id)
		: wxScrolledWindow::wxScrolledWindow(win, id) {
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->queue->Run();
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);
		wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
		sizer->Add(splitter, 1, wxALL | wxEXPAND);
		
		this->taskPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *taskSizer = new wxBoxSizer(wxVERTICAL);
		taskPanel->SetSizer(taskSizer);
		this->taskList = new wxListBox(taskPanel, wxID_ANY);
		taskSizer->Add(this->taskList, 1, wxALL | wxEXPAND);
		wxButton *newGcodeButton = new wxButton(taskPanel, wxID_ANY, __("Load GCode"));
		taskSizer->Add(newGcodeButton, 0, wxALL | wxEXPAND);
		wxButton *newProgrammedeButton = new wxButton(taskPanel, wxID_ANY, __("New Programmed"));
		taskSizer->Add(newProgrammedeButton, 0, wxALL | wxEXPAND);
		wxButton *removeButton = new wxButton(taskPanel, wxID_ANY, __("Remove"));
		taskSizer->Add(removeButton, 0, wxALL | wxEXPAND);
		newGcodeButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnNewGcodeClick, this);
		newProgrammedeButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnNewProgrammedClick, this);
		removeButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnRemoveClick, this);
		
		this->mainPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		mainPanel->SetSizer(mainSizer);
		
		this->execPanel = new wxPanel(this->mainPanel, wxID_ANY);
		mainSizer->Add(execPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *execSizer = new wxBoxSizer(wxHORIZONTAL);
		execPanel->SetSizer(execSizer);
		wxButton *buildButton = new wxButton(execPanel, wxID_ANY, __("Build"));
		execSizer->Add(buildButton, 0, wxALL | wxALIGN_CENTER);
		this->plane = new wxChoice(execPanel, wxID_ANY);
        this->speed = new wxSpinCtrl(execPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                     wxDefaultSize, wxSP_ARROW_KEYS, 0,
                                     (int) wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("core")->getInt("maxspeed", 4000),
                                     (int) wxGetApp().getSystemManager()->getConfiguration()->
                                        getEntry("core")->getInt("maxspeed", 4000));
		execSizer->Add(new wxStaticText(execPanel, wxID_ANY, __("on")), 0, wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		execSizer->Add(plane, 0, wxALL, 5);
		execSizer->Add(new wxStaticText(execPanel, wxID_ANY, __("with speed")), 0, wxLEFT | wxRIGHT | wxALIGN_CENTER, 5);
		execSizer->Add(speed, 0, wxALL, 5);
		this->stopButton = new wxButton(execPanel, wxID_ANY, __("Stop"));
		execSizer->Add(stopButton);
		wxButton *previewButton = new wxButton(execPanel, wxID_ANY, __("Preview"));
		execSizer->Add(previewButton);
		wxButton *linearizeButton = new wxButton(execPanel, wxID_ANY, __("Linearize to GCode"));
		execSizer->Add(linearizeButton);
		buildButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnBuildClick, this);
		stopButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnStopClick, this);
		previewButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnPreviewClick, this);
		linearizeButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnLinearizeClick, this);
		
		splitter->Initialize(mainPanel);
		splitter->SplitVertically(taskPanel, mainPanel);
		splitter->SetSashGravity(0.1f);
		
		Layout();
		setEnabled(true);
	        this->SetScrollRate(5, 5);
		
		taskList->Bind(wxEVT_LISTBOX, &CalxTaskPanel::OnListClick, this);		
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxTaskPanel::OnExit, this);
		this->Bind(wxEVT_TASK_PANEL_ENABLE, &CalxTaskPanel::OnEnableEvent, this);
	}
	
	void CalxTaskPanel::stop() {
		this->queue->stop();
	}
	
	void CalxTaskPanel::updateUI() {
		for (const auto& t : this->list) {
			t->Show(false);
		}
		if (taskList->GetSelection() != wxNOT_FOUND) {
            this->list.at((size_t) taskList->GetSelection())->Show(true);
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
		this->mainPanel->Layout();
		Layout();
	}
	
	void CalxTaskPanel::setEnabled(bool e) {
		wxThreadEvent evt(wxEVT_TASK_PANEL_ENABLE);
		evt.SetPayload(e);
		wxPostEvent(this, evt);
	}
	
	void CalxTaskPanel::OnExit(wxCloseEvent &evt) {
		for (const auto& h : list) {
			h->Close(true);
		}
		Destroy();
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
            taskList->SetSelection((int) list.size() - 1);
			Layout();
		}
		loader->Destroy();
		updateUI();
	}
	
	void CalxTaskPanel::OnNewProgrammedClick(wxCommandEvent &evt) {
		CalxProgrammedTaskHandle *handle = new CalxProgrammedTaskHandle(mainPanel, wxID_ANY);
		list.push_back(handle);
		taskList->Append(FORMAT(__("Task #%s"), std::to_string(list.size())));
		mainPanel->GetSizer()->Add(handle, 1, wxALL | wxEXPAND, 5);
        taskList->SetSelection((int) list.size() - 1);
		Layout();
		updateUI();
	}
	
	void CalxTaskPanel::OnRemoveClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND) {
            size_t sel = (size_t) taskList->GetSelection();
            taskList->Delete((unsigned int) sel);
			this->list.at(sel)->Close(true);
            this->list.erase(this->list.begin() + (std::ptrdiff_t) sel);
			updateUI();
		} else {
			std::string message = __("Select task to remove");
			wxMessageDialog *dialog = new wxMessageDialog(this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}
	
	void CalxTaskPanel::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}
	
	void CalxTaskPanel::OnBuildClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND
			&& plane->GetSelection() != wxNOT_FOUND) {
            list.at((size_t) taskList->GetSelection())->update();
            CoordTask *task = list.at((size_t) taskList->GetSelection())->getTask();
            CoordHandle *handle = wxGetApp().getMainFrame()->getPanel()->getCoords()->getCoord((size_t) plane->GetSelection());
			float speed = (float) this->speed->GetValue();
			speed *= wxGetApp().getSystemManager()->getConfiguration()->getEntry("ui")->getReal("speed_scale", 1);
            TaskParameters prms = {(float) speed};
			queue->addAction(new CalxTaskAction(this, handle, task, prms));
		} else {
			std::string message = __("Select coordinate plane");
			if (taskList->GetSelection() == wxNOT_FOUND) {
				message = __("Select task to build");
			}
			wxMessageDialog *dialog = new wxMessageDialog(this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}
	
	void CalxTaskPanel::OnPreviewClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND
			&& plane->GetSelection() != wxNOT_FOUND) {
            list.at((size_t) taskList->GetSelection())->update();
            CoordTask *task = list.at((size_t) taskList->GetSelection())->getTask();
            CoordHandle *handle = wxGetApp().getMainFrame()->getPanel()->getCoords()->getCoord((size_t) plane->GetSelection());
			if (!handle->isMeasured()) {
				wxMessageBox(__("Plane need to be measured before preview"), __("Warning"), wxICON_WARNING);
				return;
			}
			TaskParameters prms = {(float) this->speed->GetValue()};
			CalxVirtualPlaneDialog *dialog = new CalxVirtualPlaneDialog(this, wxID_ANY, handle, wxSize(500, 500));
			queue->addAction(new CalxPreviewAction(this, dialog, task, prms));
			dialog->Enable(false);
			dialog->ShowModal();
			delete dialog;
		} else {
			std::string message = __("Select coordinate plane");
			if (taskList->GetSelection() == wxNOT_FOUND) {
				message = __("Select task to build");
			}
			wxMessageDialog *dialog = new wxMessageDialog(this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}

	void CalxTaskPanel::OnLinearizeClick(wxCommandEvent &evt) {
		if (taskList->GetSelection() != wxNOT_FOUND
			&& plane->GetSelection() != wxNOT_FOUND) {
            list.at((size_t) taskList->GetSelection())->update();
            CoordTask *task = list.at((size_t) taskList->GetSelection())->getTask();
            CoordHandle *handle = wxGetApp().getMainFrame()->getPanel()->getCoords()->getCoord((size_t) plane->GetSelection());
			if (!handle->isMeasured()) {
				wxMessageBox(__("Plane need to be measured to linearize"), __("Warning"), wxICON_WARNING);
				return;
			}
			TaskParameters prms = {(float) this->speed->GetValue()};

			std::stringstream ss;
			TaskState state;
            GCodeWriter *writer = new GCodeWriter(handle->getBase()->getPosition(),
                                                  handle->getBase()->getSize(),
                                                  list.at((size_t) taskList->GetSelection())->getTranslator(), &ss);
			CoordPlane *plane = handle->clone(writer);
			this->setEnabled(false);
			wxGetApp().getErrorHandler()->handle(task->perform(plane, prms, wxGetApp().getSystemManager(), &state));
			this->setEnabled(true);
			delete plane;
			delete writer;



			
			wxFileDialog *dialog = new wxFileDialog(this, __("Export linearized GCode"), "", "",
                       "", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if (dialog->ShowModal() == wxID_OK) {
				std::string path = dialog->GetPath().ToStdString();
				std::ofstream out(path);
				out << ss.str();
				out.close();
			}
			ss.seekg(0);

            CalxGcodeHandle *gcodeHandle = new CalxGcodeHandle(mainPanel, wxID_ANY, __("Linear ") +
                                                               taskList->GetStringSelection().ToStdString(),
                                                               &ss,
                                                               list.at((size_t) taskList->GetSelection())->
                                                                    getTranslator()->clone(nullptr));
			
			list.push_back(gcodeHandle);
			taskList->Append(gcodeHandle->getId());
			mainPanel->GetSizer()->Add(gcodeHandle, 1, wxALL | wxEXPAND, 5);
            taskList->SetSelection((int) list.size() - 1);
			Layout();
			updateUI();
			dialog->Destroy();
		} else {
			std::string message = __("Select coordinate plane");
			if (taskList->GetSelection() == wxNOT_FOUND) {
				message = __("Select task to build");
			}
			wxMessageDialog *dialog = new wxMessageDialog(this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}
	
	void CalxTaskPanel::OnStopClick(wxCommandEvent &evt) {
		this->queue->stopCurrent();
	}

	void CalxTaskPanel::OnEnableEvent(wxThreadEvent &evt) {
		bool e = evt.GetPayload<bool>();	
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
}
