
/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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

#include "calx/ui/coord/CalxCoordPane.h"
#include "calx/ui/CalxApp.h"
#include "calx/ui/CalxErrorHandler.h"
#include "calx/ui/coord/CalxCoordAdjuster.h"
#include "calx/ui/coord/CalxCoordPlaneWatcher.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/timer.h>

namespace CalX::UI {

	wxDEFINE_EVENT(wxEVT_COORD_CTRL_WATCHER, wxThreadEvent);
	wxDEFINE_EVENT(wxEVT_COORD_CTRL_ENABLE, wxThreadEvent);

	CalxCoordPane::CalxCoordPane(wxWindow *win, wxWindowID id,
	                             std::shared_ptr<CoordHandle> ctrl,
	                             std::size_t component_pane_count)
	    : wxScrolledWindow::wxScrolledWindow(win, id) {
		this->ctrl = ctrl;
		this->used = 0;

		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), [this]() {
			wxQueueEvent(this, new wxThreadEvent(wxEVT_COMMAND_QUEUE_UPDATE));
		});
		this->queue->start();

		this->controller = new CalxCoordController(this->ctrl, this->queue);
		this->watchers = new CalxWatcherPool(this, this->ctrl);

		this->listener = std::make_shared<CalxCoordEventListener>(this);
		this->ctrl->addEventListener(this->listener);
		this->xListener = std::make_shared<CalxCoordMotorListener>(this);
		this->yListener = std::make_shared<CalxCoordMotorListener>(this);
		this->ctrl->getController()->getXAxis()->addEventListener(this->xListener);
		this->ctrl->getController()->getYAxis()->addEventListener(this->yListener);
		if (this->ctrl->getController()->getInstrument() != nullptr) {
			this->instrListener = std::make_shared<CalxCoordInstrumentListener>(this);
			this->ctrl->getController()->getInstrument()->addEventListener(
			    this->instrListener);
		}
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);

		this->generalPanel = new wxPanel(this, wxID_ANY);
		wxStaticBox *generalBox =
		    new wxStaticBox(generalPanel, wxID_ANY, __("General info"));
		wxStaticBoxSizer *generalSizer =
		    new wxStaticBoxSizer(generalBox, wxHORIZONTAL);
		generalPanel->SetSizer(generalSizer);
		this->generalInfoText = new wxStaticText(generalPanel, wxID_ANY, "");
		wxPanel *generalButtonPanel = new wxPanel(generalPanel, wxID_ANY);
		wxBoxSizer *generalButtonSizer = new wxBoxSizer(wxVERTICAL);
		generalButtonPanel->SetSizer(generalButtonSizer);
		wxButton *watcherButton =
		    new wxButton(generalButtonPanel, wxID_ANY, __("Watcher"));
		watcherButton->Bind(wxEVT_BUTTON, &CalxCoordPane::OnWatcherClick, this);
		this->adjusterButton =
		    new wxButton(generalButtonPanel, wxID_ANY, __("Adjuster"));
		adjusterButton->Bind(wxEVT_BUTTON, &CalxCoordPane::OnAdjusterClick, this);
		this->stopButton = new wxButton(generalButtonPanel, wxID_ANY, __("Stop"));
		generalSizer->Add(this->generalInfoText, 0, wxTOP | wxEXPAND, 5);
		generalSizer->Add(generalButtonPanel, 0, wxALL, 10);
		generalButtonSizer->Add(watcherButton, 0, wxALL);
		generalButtonSizer->Add(adjusterButton, 0, wxALL);
		generalButtonSizer->Add(this->stopButton, 0, wxALL);
		this->stopButton->Bind(wxEVT_BUTTON, &CalxCoordPane::OnStopClick, this);
		sizer->Add(generalPanel, 0, wxALL | wxEXPAND, 0);

		this->component_panel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *actionSizer = new wxBoxSizer(wxHORIZONTAL);
		component_panel->SetSizer(actionSizer);

		for (std::size_t i = 0; i < component_pane_count; i++) {
			wxPanel *comPane = new wxPanel(this->component_panel, wxID_ANY);
			actionSizer->Add(comPane, 0, wxALL | wxEXPAND);
			wxBoxSizer *comSizer = new wxBoxSizer(wxVERTICAL);
			comPane->SetSizer(comSizer);
			this->component_panes.push_back(comPane);
		}

		sizer->Add(component_panel, 0, wxALL | wxEXPAND, 0);
		Bind(wxEVT_COORD_CTRL_WATCHER, &CalxCoordPane::OnWatcherRequest, this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxCoordPane::OnExit, this);
		Bind(wxEVT_COORD_CTRL_ENABLE, &CalxCoordPane::OnEnableEvent, this);
		updateUI();

		this->controller->addFilterListener(this);

		this->Layout();
		this->setEnabled(true);
		this->SetScrollRate(5, 5);

		this->timer.setCtrl(this);
		timer.Start(100);
	}

	bool CalxCoordPane::addComponent(const std::string &name,
	                                 CalxCoordComponentFactory *compFact,
	                                 std::size_t paneid, bool coll) {
		if (paneid >= this->component_panes.size()) {
			return false;
		}
		wxPanel *comPane = this->component_panes.at(paneid);

		wxCollapsiblePane *colPane = new wxCollapsiblePane(comPane, wxID_ANY, name);
		comPane->GetSizer()->Add(colPane, 0, wxALL | wxEXPAND);
		wxWindow *winPane = colPane->GetPane();
		wxBoxSizer *winSizer = new wxBoxSizer(wxHORIZONTAL);
		winPane->SetSizer(winSizer);
		CalxCoordComponent *comp =
		    compFact->newComponent(winPane, this->controller);
		winSizer->Add(comp, 0, wxALL);
		colPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED,
		              &CalxCoordPane::OnInterfaceUpdate, this);
		colPane->Collapse(coll);
		this->components.push_back(comp);
		return true;
	}

	std::size_t CalxCoordPane::getComponentPaneCount() {
		return this->component_panes.size();
	}

	std::shared_ptr<CoordHandle> CalxCoordPane::getHandle() {
		return this->ctrl;
	}

	CalxCoordController *CalxCoordPane::getController() {
		return this->controller;
	}

	bool CalxCoordPane::isBusy() {
		return this->queue->isBusy();
	}

	void CalxCoordPane::use() {
		this->used++;
		if (this->used == 1) {
			wxThreadEvent evt(wxEVT_COORD_CTRL_ENABLE);
			evt.SetPayload(false);
			wxPostEvent(this, evt);
		}
	}

	void CalxCoordPane::unuse() {
		this->used--;
		if (this->used == 0) {
			wxThreadEvent evt(wxEVT_COORD_CTRL_ENABLE);
			evt.SetPayload(true);
			wxPostEvent(this, evt);
		}
	}

	bool CalxCoordPane::isUsed() {
		return this->used != 0;
	}

	void CalxCoordPane::setEnabled(bool e) {
		component_panel->Enable(e);
		adjusterButton->Enable(e);
		stopButton->Enable(!e && this->queue->isBusy());
	}

	void CalxCoordPane::updateOffset(motor_point_t offset) {
		this->watchers->updateWatchers();
	}

	void CalxCoordPane::updateScale(motor_scale_t scale) {
		this->watchers->updateWatchers();
	}

	CalxWatcherPool *CalxCoordPane::getWatchers() {
		return this->watchers;
	}

	void CalxCoordPane::stop() {
		this->queue->stopCurrent();
	}

	void CalxCoordPane::updateUI() {
		std::string units = wxGetApp().getUnitProcessor().getUnits();
		std::string general =
		    FORMAT(__("Name: Coordinate plane #%s"),
		           std::to_string(ctrl->getID())) +
		    "\n" +
		    FORMAT(__("Devices: #%s #%s"),
		           std::to_string(ctrl->getController()->getXAxis()->getID()),
		           std::to_string(ctrl->getController()->getYAxis()->getID())) +
		    "\n" +
		    (ctrl->getController()->getInstrument() != nullptr
		         ? FORMAT(__("Instrument #%s"),
		                  std::to_string(
		                      ctrl->getController()->getInstrument()->getID()))
		         : __("Instrument: no")) +
		    "\n" +
		    FORMAT(__("Position: %sx%s %s"),
		           wxGetApp().getUnitProcessor().formatDouble(
		               ctrl->getFloatPlane()->getFloatPosition().x),
		           wxGetApp().getUnitProcessor().formatDouble(
		               ctrl->getFloatPlane()->getFloatPosition().y),
		           units) +
		    "\n" +
		    (ctrl->isMeasured()
		         ? FORMAT(__("Start: %sx%s %s"),
		                  wxGetApp().getUnitProcessor().formatDouble(
		                      ctrl->getFloatPlane()->getFloatSize().x),
		                  wxGetApp().getUnitProcessor().formatDouble(
		                      ctrl->getFloatPlane()->getFloatSize().y),
		                  units)
		         : __("Start: Not measured")) +
		    "\n" +
		    (ctrl->isMeasured()
		         ? FORMAT(__("Size: %sx%s %s"),
		                  wxGetApp().getUnitProcessor().formatDouble(
		                      ctrl->getFloatPlane()->getFloatSize().w),
		                  wxGetApp().getUnitProcessor().formatDouble(
		                      ctrl->getFloatPlane()->getFloatSize().h),
		                  units)
		         : __("Size: Not measured"));
		this->generalInfoText->SetLabel(general);
		Layout();
	}

	void CalxCoordPane::shutdown() {
		timer.Stop();
		this->queue->stop();
		ctrl->getController()->stop();
	}

	void CalxCoordPane::newWatcher() {
		wxCommandEvent evt(wxEVT_COORD_CTRL_WATCHER);
		wxPostEvent(this, evt);
	}

	void CalxCoordPane::OnWatcherClick(wxCommandEvent &evt) {
		this->watchers->newWatcher();
	}

	void CalxCoordPane::OnExit(wxCloseEvent &evt) {
		for (const auto &comp : this->components) {
			comp->Close(true);
		}
		this->controller->removeFilterListener(this);

		delete this->controller;
		delete this->watchers;

		this->ctrl->removeEventListener(this->listener);
		this->ctrl->getController()->getXAxis()->removeEventListener(
		    this->xListener);
		this->ctrl->getController()->getYAxis()->removeEventListener(
		    this->yListener);
		if (this->ctrl->getController()->getInstrument() != nullptr) {
			this->ctrl->getController()->getInstrument()->removeEventListener(
			    this->instrListener);
		}
		wxGetApp().getSystemManager().getCoordPlaneSet().removeCoord(ctrl->getID());

		Destroy();
	}

	void CalxCoordPane::OnStopClick(wxCommandEvent &evt) {
		this->queue->stopCurrent();
	}

	void CalxCoordPane::OnInterfaceUpdate(wxCollapsiblePaneEvent &evt) {
		Layout();
		Refresh();
	}

	void CalxCoordPane::OnWatcherRequest(wxThreadEvent &evt) {
		this->watchers->newWatcher();
	}

	void CalxCoordPane::OnEnableEvent(wxThreadEvent &evt) {
		setEnabled(evt.GetPayload<bool>());
		wxGetApp().getMainFrame()->getPanel()->updateUI();
	}

	void CalxCoordPane::OnAdjusterClick(wxCommandEvent &evt) {
		if (!this->ctrl->isMeasured()) {
			wxMessageBox(__("Plane need to be measured before adjustement"),
			             __("Warning"), wxICON_WARNING);
			return;
		}
		CalxCoordAdjustDialog *dialog =
		    new CalxCoordAdjustDialog(this, wxID_ANY, this->ctrl);
		dialog->ShowModal();
		delete dialog;
	}
}  // namespace CalX::UI
