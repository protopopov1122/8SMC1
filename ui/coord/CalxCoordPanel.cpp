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


#include "CalxPanel.h"
#include <wx/listbook.h>
#include <wx/sizer.h>
#include "CalxCoordPanel.h"
#include "CalxCoordDialog.h"

namespace CalXUI {
	
	CalxCoordPanel::CalxCoordPanel(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		
		wxPanel *listPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *listSizer = new wxBoxSizer(wxVERTICAL);
		
		this->coordList = new wxListBox(listPanel, wxID_ANY);
		listSizer->Add(this->coordList, 1, wxBOTTOM | wxEXPAND, 5);
		this->coordList->Bind(wxEVT_LISTBOX, &CalxCoordPanel::OnListClick, this);
		wxButton *addButton = new wxButton(listPanel, wxID_ANY, "Add");
		listSizer->Add(addButton, 0, wxALL | wxEXPAND);
		addButton->Bind(wxEVT_BUTTON, &CalxCoordPanel::OnAddButtonClick, this);
		wxButton *removeButton = new wxButton(listPanel, wxID_ANY, "Remove");
		listSizer->Add(removeButton, 0, wxALL | wxEXPAND);
		removeButton->Bind(wxEVT_BUTTON, &CalxCoordPanel::OnRemoveButtonClick, this);
		
		listPanel->SetSizer(listSizer);
		sizer->Add(listPanel, 0, wxALL | wxEXPAND, 5);
		
		this->mainPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(mainPanel, 1, wxEXPAND | wxALL, 10);
		mainPanel->SetSizer(mainSizer);
		
		SetSizer(sizer);
		Bind(wxEVT_CLOSE_WINDOW, &CalxCoordPanel::OnExit, this);
		
		this->nextId = 0;
	}
	
	void CalxCoordPanel::updateUI() {
		for (const auto& ctrl : this->coords) {
			ctrl->Hide();
		}
		if (this->coordList->GetSelection() != wxNOT_FOUND) {
			this->coords.at(this->coordList->GetSelection())->Show(true);
		}
		mainPanel->Layout();
		Layout();
	}
	
	void CalxCoordPanel::stop() {
		for (const auto& ctrl : this->coords) {
			ctrl->stop();
		}
	}
	
	void CalxCoordPanel::addPlane(CoordHandle *handle) {
		CalxCoordCtrl *ctrl = new CalxCoordCtrl(this->mainPanel, wxID_ANY, handle);
		this->mainPanel->GetSizer()->Add(ctrl, 1, wxALL | wxEXPAND, 0);
		this->coords.push_back(ctrl);
		this->coordList->Append("Plane #" + std::to_string(handle->getID()));
		this->coordList->SetSelection(this->coordList->GetCount() - 1);
		wxGetApp().getMainFrame()->getPanel()->updateUI();
	}
	
	void CalxCoordPanel::removePlane(size_t pl) {
		this->coordList->Delete(pl);
		this->mainPanel->GetSizer()->Detach(coords.at(pl));
		this->coords.at(pl)->Close(true);
		this->coords.erase(this->coords.begin() + pl);
		wxGetApp().getMainFrame()->getPanel()->updateUI();
	}
	
	void CalxCoordPanel::OnExit(wxCloseEvent &evt) {
		while (!this->coords.empty()) {
			size_t pl = 0;
			this->mainPanel->GetSizer()->Detach(coords.at(pl));
			this->coordList->Delete(pl);
			coords.at(pl)->Close(true);
			this->coords.erase(this->coords.begin() + pl);
		}
		Destroy();
	}
	
	void CalxCoordPanel::OnAddButtonClick(wxCommandEvent &evt) {
		CalxCoordDialog *dialog = new CalxCoordDialog(this, wxID_ANY,
			wxGetApp().getSystemManager());
		dialog->ShowModal();
		if (dialog->getCoordPlane() != nullptr) {
			addPlane(dialog->getCoordPlane());
		}
		dialog->Destroy();
	}
	
	void CalxCoordPanel::OnRemoveButtonClick(wxCommandEvent &evt) {
		if (this->coordList->GetSelection() != wxNOT_FOUND) {
			removePlane(this->coordList->GetSelection());
		} else {
			wxMessageBox("Select plane to remove", "Warning", wxOK | wxICON_WARNING);
		}
	}
	
	void CalxCoordPanel::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}
}