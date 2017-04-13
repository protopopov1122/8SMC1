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
#include <wx/splitter.h>
#include "CalxCoordPanel.h"
#include "CalxCoordDialog.h"
#include "CalxErrorHandler.h"

namespace CalXUI {
	
	wxDEFINE_EVENT(wxEVT_COORD_PANEL_UPDATE, wxThreadEvent);
	
	class CalxCoordPlaneAddRequest : public RequestProvider {
		public:
			CalxCoordPlaneAddRequest(CalxCoordPanel *panel)
				: RequestProvider::RequestProvider("plane.create") {
				this->panel = panel;
			}
			
			virtual ~CalxCoordPlaneAddRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 3)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
				device_id_t dev1 = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				device_id_t dev2 = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 1))->getValue();
				device_id_t instr = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 2))->getValue();
				CoordHandle *handle = sysman->createCoord(dev1, dev2, instr);
				if (handle == nullptr) {
					return false;
				}
				bool ready = false;
				panel->updateList(handle, &ready);
				while (!ready) wxThread::Yield();	

				return true;
			}
		private:
			CalxCoordPanel *panel;
	};
	
	class CalxCoordPlaneMeasureRequest : public RequestProvider {
		public:
			CalxCoordPlaneMeasureRequest(CalxCoordPanel *panel)
				: RequestProvider::RequestProvider("plane.measure") {
				this->panel = panel;
			}
			
			virtual ~CalxCoordPlaneMeasureRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 2)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
				device_id_t plid = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				device_id_t tr = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 1))->getValue() % 2;
				TrailerId trailer = tr == 1 ? TrailerId::Trailer1 : TrailerId::Trailer2;
				if (sysman->getCoord(plid) != nullptr) {
					panel->measure(plid, trailer);
					return true;
				} else {
					return false;
				}
			}
		private:
			CalxCoordPanel *panel;
	};
	
	class CalxCoordPlanePositionRequest : public RequestProvider {
		public:
			CalxCoordPlanePositionRequest(CalxCoordPanel *panel)
				: RequestProvider::RequestProvider("plane.position") {
				this->panel = panel;
			}
			
			virtual ~CalxCoordPlanePositionRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 3)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Real)
				PROVIDER_PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Real)
				device_id_t plid = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				double x =  ((RealConfigValue*) PROVIDER_ARG(req, 1))->getValue();
				double y =  ((RealConfigValue*) PROVIDER_ARG(req, 2))->getValue();
				if (sysman->getCoord(plid) != nullptr) {
					panel->position(plid, x, y);
					return true;
				} else {
					return false;
				}
			}
		private:
			CalxCoordPanel *panel;
	};
	
	class CalxCoordPlanePositionAbsRequest : public RequestProvider {
		public:
			CalxCoordPlanePositionAbsRequest(CalxCoordPanel *panel)
				: RequestProvider::RequestProvider("plane.position.abs") {
				this->panel = panel;
			}
			
			virtual ~CalxCoordPlanePositionAbsRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 3)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
				PROVIDER_PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
				device_id_t plid = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				motor_coord_t x = (motor_coord_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 1))->getValue();
				motor_coord_t y = (motor_coord_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 2))->getValue();
				motor_point_t dest = {x, y};
				if (sysman->getCoord(plid) != nullptr) {
					panel->positionAbs(plid, dest);
					return true;
				} else {
					return false;
				}
			}
		private:
			CalxCoordPanel *panel;
	};
	
	class CalxCoordPlaneCenterRequest : public RequestProvider {
		public:
			CalxCoordPlaneCenterRequest(CalxCoordPanel *panel)
				: RequestProvider::RequestProvider("plane.position.center") {
				this->panel = panel;
			}
			
			virtual ~CalxCoordPlaneCenterRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 1)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				device_id_t plid = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				if (sysman->getCoord(plid) != nullptr) {
					panel->center(plid);
					return true;
				} else {
					return false;
				}
			}
		private:
			CalxCoordPanel *panel;
	};
	
	class CalxCoordPlaneInvertRequest : public RequestProvider {
		public:
			CalxCoordPlaneInvertRequest(CalxCoordPanel *panel)
				: RequestProvider::RequestProvider("plane.axis.invert") {
				this->panel = panel;
			}
			
			virtual ~CalxCoordPlaneInvertRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 1)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				device_id_t plid = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				if (sysman->getCoord(plid) != nullptr) {
					panel->invert(plid);
					return true;
				} else {
					return false;
				}
			}
		private:
			CalxCoordPanel *panel;
	};
	
	class CalxCoordPlaneWatcherRequest : public RequestProvider {
		public:
			CalxCoordPlaneWatcherRequest(CalxCoordPanel *panel)
				: RequestProvider::RequestProvider("plane.watcher") {
				this->panel = panel;
			}
			
			virtual ~CalxCoordPlaneWatcherRequest() {
				
			}
			
			virtual bool execute(Request *req, SystemManager *sysman) {
				PROVIDER_PROVIDER_ARGC(req, 1)
				PROVIDER_PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
				device_id_t plid = (device_id_t) ((IntegerConfigValue*) PROVIDER_ARG(req, 0))->getValue();
				if (sysman->getCoord(plid) != nullptr) {
					panel->watcher(plid);
					return true;
				} else {
					return false;
				}
			}
		private:
			CalxCoordPanel *panel;
	};
	
	CalxCoordPanel::CalxCoordPanel(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
		
		CalxApp &app = wxGetApp();
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxCoordPlaneAddRequest(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxCoordPlaneMeasureRequest(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxCoordPlanePositionRequest(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxCoordPlanePositionAbsRequest(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxCoordPlaneCenterRequest(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxCoordPlaneInvertRequest(this));
		app.getSystemManager()->getRequestResolver()->registerProvider(new CalxCoordPlaneWatcherRequest(this));
		
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
		removeButton->Bind(wxEVT_BUTTON, &CalxCoordPanel::OnRemoveButtonClick, this);
		
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
	
	void CalxCoordPanel::measure(device_id_t id, TrailerId tr) {
		for (const auto& ctrl : this->coords) {
			if (ctrl->getHandle()->getID() == id) {
				ctrl->measure(tr);
				break;
			}
		}
	}
	
	void CalxCoordPanel::position(device_id_t id, double x, double y) {
		for (const auto& ctrl : this->coords) {
			if (ctrl->getHandle()->getID() == id) {
				ctrl->position(x, y);
				break;
			}
		}
	}
	
	void CalxCoordPanel::positionAbs(device_id_t id, motor_point_t dest) {
		for (const auto& ctrl : this->coords) {
			if (ctrl->getHandle()->getID() == id) {
				ctrl->positionAbs(dest);
				break;
			}
		}
	}
	
	void CalxCoordPanel::center(device_id_t id) {
		for (const auto& ctrl : this->coords) {
			if (ctrl->getHandle()->getID() == id) {
				ctrl->center();
				break;
			}
		}
	}
	
	void CalxCoordPanel::invert(device_id_t id) {
		for (const auto& ctrl : this->coords) {
			if (ctrl->getHandle()->getID() == id) {
				ctrl->invert();
				break;
			}
		}
	}
	
	void CalxCoordPanel::watcher(device_id_t id) {
		for (const auto& ctrl : this->coords) {
			if (ctrl->getHandle()->getID() == id) {
				ctrl->watcher();
				break;
			}
		}
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
		this->mainPanel->GetSizer()->Add(ctrl, 1, wxALL | wxEXPAND, 5);
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
			wxMessageBox(__("Select plane to remove"), __("Warning"), wxOK | wxICON_WARNING);
		}
	}
	
	void CalxCoordPanel::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}
	
	void CalxCoordPanel::OnCoordPlaneAdd(wxThreadEvent &evt) {
		std::pair<CoordHandle*, bool*> pair = evt.GetPayload<std::pair<CoordHandle*, bool*>>();
		addPlane(pair.first);
		*pair.second = true;
	}
}