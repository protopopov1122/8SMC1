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

#include "ui/CalxPanel.h"
#include <wx/listbook.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include "ui/coord/CalxCoordPanel.h"
#include "ui/coord/CalxCoordDialog.h"
#include "ui/coord/CalxPlaneProviders.h"
#include "ui/CalxErrorHandler.h"

namespace CalXUI {

  wxDEFINE_EVENT(wxEVT_COORD_PANEL_UPDATE, wxThreadEvent);

  CalxCoordPanel::CalxCoordPanel(wxWindow *win, wxWindowID id)
	  : wxPanel::wxPanel(win, id) {

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(sizer);
	wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
	sizer->Add(splitter, 1, wxALL | wxEXPAND);

	wxPanel *listPanel = new wxPanel(splitter, wxID_ANY);
	wxBoxSizer *listSizer = new wxBoxSizer(wxVERTICAL);

	this->coordList = new wxListBox(listPanel, wxID_ANY);
	listSizer->Add(this->coordList, 1, wxBOTTOM | wxEXPAND, 5);
	this->coordList->Bind(wxEVT_LISTBOX, &CalxCoordPanel::OnListClick, this);
	wxButton *addButton = new wxButton(listPanel, wxID_ANY, __("Add"));
	listSizer->Add(addButton, 0, wxALL | wxEXPAND);
	addButton->Bind(wxEVT_BUTTON, &CalxCoordPanel::OnAddButtonClick, this);
	wxButton *removeButton = new wxButton(listPanel, wxID_ANY, __("Remove"));
	listSizer->Add(removeButton, 0, wxALL | wxEXPAND);
	removeButton->Bind(wxEVT_BUTTON, &CalxCoordPanel::OnRemoveButtonClick,
					   this);

	listPanel->SetSizer(listSizer);

	this->mainPanel = new wxPanel(splitter, wxID_ANY);
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel->SetSizer(mainSizer);

	splitter->Initialize(mainPanel);
	splitter->SplitVertically(listPanel, mainPanel);
	splitter->SetSashGravity(0.1f);

	Layout();
	Bind(wxEVT_CLOSE_WINDOW, &CalxCoordPanel::OnExit, this);
	Bind(wxEVT_COORD_PANEL_UPDATE, &CalxCoordPanel::OnCoordPlaneAdd, this);

	this->nextId = 0;
  }

  void CalxCoordPanel::updateList(CoordHandle *handle, bool *ready) {
	wxThreadEvent evt(wxEVT_COORD_PANEL_UPDATE);
	evt.SetPayload(std::make_pair(handle, ready));
	wxPostEvent(this, evt);
  }

  void CalxCoordPanel::updateUI() {
	for (const auto &ctrl : this->coords) {
	  ctrl->Hide();
	}
	if (this->coordList->GetSelection() != wxNOT_FOUND) {
	  this->coords.at((size_t) this->coordList->GetSelection())->Show(true);
	}
	mainPanel->Layout();
	Layout();
  }
  
  size_t CalxCoordPanel::getCoordCount() {
	return coords.size();
  }
  
  CoordHandle *CalxCoordPanel::getCoord(size_t sz) {
	return sz < coords.size() ? coords.at(sz)->getHandle() : nullptr;
  }
  
  CalxCoordCtrl *CalxCoordPanel::getCoordCtrl(size_t sz) {
	return sz < coords.size() ? coords.at(sz) : nullptr;
  }
  
  bool CalxCoordPanel::isBusy()  {
	for (const auto &c : coords) {
		if (c->isBusy()) {
			return true;
		}
	}
	return false;
 }
  
  void CalxCoordPanel::shutdown() {
	for (const auto &ctrl : this->coords) {
	  ctrl->shutdown();
	}
  }

  void CalxCoordPanel::addPlane(CoordHandle *handle) {
	CalxCoordCtrl *ctrl = new CalxCoordCtrl(this->mainPanel, wxID_ANY, handle);
	this->mainPanel->GetSizer()->Add(ctrl, 1, wxALL | wxEXPAND, 5);
	this->coords.push_back(ctrl);
	this->coordList->Append("Plane #" + std::to_string(handle->getID()));
	this->coordList->SetSelection((int) this->coordList->GetCount() - 1);
	wxGetApp().getMainFrame()->getPanel()->updateUI();
  }

  void CalxCoordPanel::removePlane(size_t pl) {
	if (coords.at(pl)->getWatchers()->hasWatchers()) {
	  wxMessageBox("Watchers are attached to this plane. Detach them first.",
				   "Error", wxICON_ERROR);
	  return;
	}
	this->coordList->Delete((unsigned int) pl);
	this->mainPanel->GetSizer()->Detach(coords.at(pl));
	this->coords.at(pl)->Close(true);
	this->coords.erase(this->coords.begin() + (std::ptrdiff_t) pl);
	wxGetApp().getMainFrame()->getPanel()->updateUI();
  }

  void CalxCoordPanel::OnExit(wxCloseEvent &evt) {
	while (!this->coords.empty()) {
	  size_t pl = 0;
	  this->mainPanel->GetSizer()->Detach(coords.at(pl));
	  this->coordList->Delete((unsigned int) pl);
	  coords.at(pl)->Close(true);
	  this->coords.erase(this->coords.begin() + (std::ptrdiff_t) pl);
	}
	Destroy();
  }

  void CalxCoordPanel::OnAddButtonClick(wxCommandEvent &evt) {
	CalxCoordDialog *dialog =
		new CalxCoordDialog(this, wxID_ANY, wxGetApp().getSystemManager());
	dialog->ShowModal();
	if (dialog->getCoordPlane() != nullptr) {
	  addPlane(dialog->getCoordPlane());
	}
	dialog->Destroy();
  }

  void CalxCoordPanel::OnRemoveButtonClick(wxCommandEvent &evt) {
	if (this->coordList->GetSelection() != wxNOT_FOUND) {
	  removePlane((size_t) this->coordList->GetSelection());
	} else {
	  wxMessageBox(__("Select plane to remove"), __("Warning"),
				   wxOK | wxICON_WARNING);
	}
  }

  void CalxCoordPanel::OnListClick(wxCommandEvent &evt) {
	updateUI();
  }

  void CalxCoordPanel::OnCoordPlaneAdd(wxThreadEvent &evt) {
	std::pair<CoordHandle *, bool *> pair =
		evt.GetPayload<std::pair<CoordHandle *, bool *>>();
	addPlane(pair.first);
	*pair.second = true;
  }
}
