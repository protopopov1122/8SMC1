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


#include "CalxTaskHandle.h"
#include <wx/sizer.h>

namespace CalXUI {
	CalxProgrammedTaskHandle::CalxProgrammedTaskHandle(wxWindow *win, wxWindowID id)
		: CalxTaskHandle::CalxTaskHandle(win, id, CalxTaskType::CalxProgrammed) {
		this->task = new ProgrammedCoordTask();	
		
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);
		
		wxPanel *listPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(listPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *listSizer = new wxBoxSizer(wxVERTICAL);
		listPanel->SetSizer(listSizer);
		this->stepList = new wxListBox(listPanel, wxID_ANY);
		listSizer->Add(this->stepList, 1, wxALL | wxEXPAND, 0);
		stepList->Bind(wxEVT_LISTBOX, &CalxProgrammedTaskHandle::OnListClick, this);
		wxButton *moveButton = new wxButton(listPanel, wxID_ANY, "Add move step");
		listSizer->Add(moveButton, 0, wxALL | wxEXPAND);
		moveButton->Bind(wxEVT_BUTTON, &CalxProgrammedTaskHandle::OnMoveAddClick, this);
		wxButton *removeButton = new wxButton(listPanel, wxID_ANY, "Remove step");
		listSizer->Add(removeButton, 0, wxALL | wxEXPAND);
		removeButton->Bind(wxEVT_BUTTON, &CalxProgrammedTaskHandle::OnRemoveClick, this);
		
		this->mainPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		mainPanel->SetSizer(mainSizer);
		sizer->Add(mainPanel, 1, wxALL | wxEXPAND, 5);
		
		
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxProgrammedTaskHandle::OnExit, this);	
		Layout();
	}
	
	CoordTask *CalxProgrammedTaskHandle::getTask() {
		return this->task;
	}
	
	void CalxProgrammedTaskHandle::update() {
		for (const auto& s : this->steps) {
			s->update();
		}
	}
	
	void CalxProgrammedTaskHandle::updateUI() {
		for (const auto& s : this->steps) {
			s->Hide();
		}
		if (stepList->GetSelection() != wxNOT_FOUND) {
			this->steps.at(stepList->GetSelection())->Show(true);
		}
		mainPanel->Layout();
		Layout();
	}
	
	void CalxProgrammedTaskHandle::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}
	
	void CalxProgrammedTaskHandle::OnMoveAddClick(wxCommandEvent &evt) {
		CalxTaskLinearStepHandle *handle = new CalxTaskLinearStepHandle(this->mainPanel, wxID_ANY);
		this->mainPanel->GetSizer()->Add(handle, 0, wxALL | wxEXPAND, 5);
		steps.push_back(handle);
		stepList->Append("#" + std::to_string(steps.size()) + " Move");
		stepList->SetSelection(steps.size() - 1);
		task->addStep(handle->getTaskStep());
		updateUI();
	}
	
	void CalxProgrammedTaskHandle::OnRemoveClick(wxCommandEvent &evt) {
		if (this->stepList->GetSelection() != wxNOT_FOUND) {
			task->removeStep(this->stepList->GetSelection());
			this->steps.at(this->stepList->GetSelection())->Destroy();
			this->steps.erase(this->steps.begin() + this->stepList->GetSelection());
			stepList->Delete(this->stepList->GetSelection());
			updateUI();
		}
	}
	
	void CalxProgrammedTaskHandle::OnExit(wxCloseEvent &evt) {
		delete this->task;
	}
}