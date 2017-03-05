#include <iostream>
#include <fstream>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "coord/CalxCoordPanel.h"
#include "CalxTaskPanel.h"
#include "CalxGcodeLoader.h"

namespace CalX {
	
	class CalxTaskAction : public CalxAction {
		public:
			CalxTaskAction(CalxTaskPanel *panel, CoordHandle *handle, CoordTask *task, TaskParameters prms) {
				this->panel = panel;
				this->handle = handle;
				this->task = task;
				this->prms = prms;
			}
			
			virtual void perform(SystemManager *sysman) {
				handle->getController()->sendUse();
				panel->Enable(false);
				task->perform(handle, prms, sysman);
				panel->Enable(true);
				handle->getController()->sendUnuse();
			}
		private:
			CalxTaskPanel *panel;
			CoordHandle *handle;
			CoordTask *task;
			TaskParameters prms;
	};
	
	CalxTaskPanel::CalxTaskPanel(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->queue->Run();
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		
		wxPanel *taskPanel = new wxPanel(this, wxID_ANY);
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
		buildButton->Bind(wxEVT_BUTTON, &CalxTaskPanel::OnBuildClick, this);
		
		SetSizer(sizer);
		Layout();
		
		taskList->Bind(wxEVT_LISTBOX, &CalxTaskPanel::OnListClick, this);		
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxTaskPanel::OnExit, this);	
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
			CoordHandle *handle = calxPanel->getCoords()->getCoord(i);
			plane->Append("Plane #" + std::to_string(handle->getID()));
		}
		if (calxPanel->getCoords()->getCoordCount() > 0) {
			plane->SetSelection(0);
		}
		Layout();
	}
	
	void CalxTaskPanel::OnExit(wxCloseEvent &evt) {
		this->queue->stop();
		this->queue->Kill();
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
			this->list.at(sel)->Destroy();
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
}