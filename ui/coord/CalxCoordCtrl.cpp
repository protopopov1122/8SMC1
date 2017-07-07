
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

#include <math.h>
#include <sstream>
#include <algorithm>
#include "ui/CalxApp.h"
#include "ui/CalxErrorHandler.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include "ctrl-lib/graph/FunctionParser.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include "ui/coord/CalxCoordCtrl.h"
#include "ui/task/CalxTaskPanel.h"
#include "ui/coord/CalxCoordPlaneWatcher.h"
#include "ui/coord/CalxCoordAdjuster.h"

namespace CalXUI {

  wxDEFINE_EVENT(wxEVT_COORD_CTRL_WATCHER, wxThreadEvent);
  wxDEFINE_EVENT(wxEVT_COORD_CTRL_ENABLE, wxThreadEvent);

  CalxCoordCtrl::CalxCoordCtrl(wxWindow *win, wxWindowID id, CoordHandle *ctrl)
	  : wxScrolledWindow::wxScrolledWindow(win, id) {
	this->ctrl = ctrl;
	this->used = 0;
	this->master = false;

	this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
	this->queue->Run();
	
	this->controller = new CalxCoordController(this->ctrl, this, this, this->queue);
	
	this->listener = new CalxCoordEventListener(this);
	this->ctrl->addEventListener(this->listener);
	this->xListener = new CalxCoordMotorListener(this);
	this->yListener = new CalxCoordMotorListener(this);
	this->ctrl->getController()->getXAxis()->addEventListener(this->xListener);
	this->ctrl->getController()->getYAxis()->addEventListener(this->yListener);
	if (this->ctrl->getController()->getInstrument() != nullptr) {
	  this->instrListener = new CalxCoordInstrumentListener(this);
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
	watcherButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnWatcherClick, this);
	wxButton *adjusterButton =
		new wxButton(generalButtonPanel, wxID_ANY, __("Adjuster"));
	adjusterButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnAdjusterClick, this);
	this->stopButton = new wxButton(generalButtonPanel, wxID_ANY, __("Stop"));
	generalSizer->Add(this->generalInfoText, 0, wxTOP | wxEXPAND, 5);
	generalSizer->Add(generalButtonPanel, 0, wxALL, 10);
	generalButtonSizer->Add(watcherButton, 0, wxALL);
	generalButtonSizer->Add(adjusterButton, 0, wxALL);
	generalButtonSizer->Add(this->stopButton, 0, wxALL);
	this->stopButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnStopClick, this);
	sizer->Add(generalPanel, 0, wxALL | wxEXPAND, 0);

	this->actionPanel = new wxPanel(this, wxID_ANY);
	wxBoxSizer *actionSizer = new wxBoxSizer(wxHORIZONTAL);
	actionPanel->SetSizer(actionSizer);

	wxPanel *actionSubPanel = new wxPanel(actionPanel, wxID_ANY);
	wxBoxSizer *actionSubSizer = new wxBoxSizer(wxVERTICAL);
	actionSubPanel->SetSizer(actionSubSizer);
	actionSizer->Add(actionSubPanel, 0, wxALL | wxEXPAND);

	wxCollapsiblePane *linearPane =
		new wxCollapsiblePane(actionSubPanel, wxID_ANY, __("Linear movement"));
	actionSubSizer->Add(linearPane, 0, wxALL | wxEXPAND);
	wxWindow *linearPanel = linearPane->GetPane();
	wxBoxSizer *linearSizer = new wxBoxSizer(wxHORIZONTAL);
	linearPanel->SetSizer(linearSizer);
	this->linear = new CalxCoordLinearCtrl(linearPanel, wxID_ANY, this->controller);
	linearSizer->Add(linear, 0, wxALL);
	linearPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED,
					 &CalxCoordCtrl::OnInterfaceUpdate, this);

	wxCollapsiblePane *arcPane =
		new wxCollapsiblePane(actionSubPanel, wxID_ANY, __("Arc movement"));
	actionSubSizer->Add(arcPane, 0, wxALL | wxEXPAND);
	wxWindow *arcPanel = arcPane->GetPane();
	wxBoxSizer *arcSizer = new wxBoxSizer(wxHORIZONTAL);
	arcPanel->SetSizer(arcSizer);
	this->arc = new CalxCoordArcCtrl(arcPanel, wxID_ANY, this->controller);
	arcSizer->Add(arc, 0, wxALL);
	arcPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED,
				  &CalxCoordCtrl::OnInterfaceUpdate, this);

	wxCollapsiblePane *graphPane =
		new wxCollapsiblePane(actionSubPanel, wxID_ANY, __("Function graph"));
	actionSubSizer->Add(graphPane, 0, wxALL | wxEXPAND);
	wxWindow *graphPanel = graphPane->GetPane();
	wxBoxSizer *graphSizer = new wxBoxSizer(wxHORIZONTAL);
	graphPanel->SetSizer(graphSizer);
	this->graphCtrl = new CalxCoordGraphCtrl(graphPanel, wxID_ANY);
	graphSizer->Add(graphCtrl, 0, wxALL);
	graphCtrl->getBuildButton()->Bind(wxEVT_BUTTON,
									  &CalxCoordCtrl::OnGraphBuildClick, this);
	graphCtrl->getPreviewButton()->Bind(
		wxEVT_BUTTON, &CalxCoordCtrl::OnGraphPreviewClick, this);
	graphPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED,
					&CalxCoordCtrl::OnInterfaceUpdate, this);
	graphPane->Collapse(false);

	wxPanel *actionSub2Panel = new wxPanel(actionPanel, wxID_ANY);
	actionSizer->Add(actionSub2Panel, 1, wxLEFT | wxEXPAND, 5);
	wxStaticBox *otherBox =
		new wxStaticBox(actionSub2Panel, wxID_ANY, __("Other"));
	wxStaticBoxSizer *otherSizer = new wxStaticBoxSizer(otherBox, wxVERTICAL);
	actionSub2Panel->SetSizer(otherSizer);
	this->otherCtrl = new CalxCoordOtherCtrl(actionSub2Panel, wxID_ANY, this->controller, this);
	otherSizer->Add(otherCtrl, 1, wxALL | wxEXPAND);
	this->otherCtrl->getPositionPane()->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED,
				  &CalxCoordCtrl::OnInterfaceUpdate, this);
	this->otherCtrl->getFiltersPane()->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED,
				  &CalxCoordCtrl::OnInterfaceUpdate, this);
	

	sizer->Add(actionPanel, 0, wxALL | wxEXPAND, 0);
	Bind(wxEVT_COORD_CTRL_WATCHER, &CalxCoordCtrl::OnWatcherRequest, this);
	Bind(wxEVT_CLOSE_WINDOW, &CalxCoordCtrl::OnExit, this);
	graphCtrl->Bind(wxEVT_CLOSE_WINDOW, &CalxCoordGraphCtrl::OnClose,
					graphCtrl);
	Bind(wxEVT_COORD_CTRL_ENABLE, &CalxCoordCtrl::OnEnableEvent, this);
	updateUI();

	this->Layout();
	this->setEnabled(true);
	this->SetScrollRate(5, 5);

	this->timer.setCtrl(this);
	timer.Start(100);
  }

  void CalxCoordCtrl::use() {
	this->used++;
	if (this->used == 1) {
	  wxThreadEvent evt(wxEVT_COORD_CTRL_ENABLE);
	  evt.SetPayload(false);
	  wxPostEvent(this, evt);
	}
  }

  void CalxCoordCtrl::unuse() {
	this->used--;
	if (this->used == 0) {
	  wxThreadEvent evt(wxEVT_COORD_CTRL_ENABLE);
	  evt.SetPayload(true);
	  wxPostEvent(this, evt);
	}
  }

  bool CalxCoordCtrl::isUsed() {
	return this->used != 0;
  }

  void CalxCoordCtrl::setMaster(bool m) {
	this->master = m;
  }

  void CalxCoordCtrl::setEnabled(bool e) {
	actionPanel->Enable(e);
	stopButton->Enable(!e && this->master);
  }

  void CalxCoordCtrl::setOffset(motor_point_t offset) {
	motor_scale_t scale = this->controller->getMapFilter()->getScale();
	offset.x *= scale.x;
	offset.y *= scale.y;
	this->otherCtrl->setOffset(offset);
	this->controller->getMapFilter()->setOffset(offset);
	updateWatchers();
  }

  void CalxCoordCtrl::updateWatchers() {
	for (const auto &w : this->watchers) {
	  w->update();
	}
  }

  void CalxCoordCtrl::bindWatcher(CalxCoordPlaneWatcher *w) {
	this->watchers.push_back(w);
  }

  void CalxCoordCtrl::unbindWatcher(CalxCoordPlaneWatcher *w) {
	this->watchers.erase(
		std::remove(this->watchers.begin(), this->watchers.end(), w),
		this->watchers.end());
  }

  bool CalxCoordCtrl::hasWatchers() {
	return !this->watchers.empty();
  }

  void CalxCoordCtrl::requestMeasure(TrailerId tr) {
	bool ready = false;
	this->controller->measure(tr, &ready);
	while (!ready) {
	  wxThread::Yield();
	}
  }

  void CalxCoordCtrl::requestPosition(double x, double y) {
	ConfigManager *config = wxGetApp().getSystemManager()->getConfiguration();
	coord_point_t dest = {x, y};
	int_conf_t mspeed =
		config->getEntry("core")->getInt("maxspeed", 125);
	double speed = ((double) mspeed) / wxGetApp().getSpeedScale();
	bool ready = false;
	this->controller->move(dest, speed, &ready);
	while (!ready) {
	  wxThread::Yield();
	}
  }

  void CalxCoordCtrl::requestPositionAbs(motor_point_t mdest) {
	ConfigManager *config = wxGetApp().getSystemManager()->getConfiguration();
	int_conf_t mspeed =
		config->getEntry("core")->getInt("maxspeed", 125);
	coord_point_t dest = {mdest.x / wxGetApp().getUnitScale(), mdest.y / wxGetApp().getUnitScale()};
	double speed = ((double) mspeed) / wxGetApp().getSpeedScale();
	bool ready = false;
	this->controller->move(dest, speed, false, false, &ready);
	while (!ready) {
	  wxThread::Yield();
	}
  }

  void CalxCoordCtrl::requestCenter() {
	motor_point_t offset = ctrl->getPosition();
	this->controller->getMapFilter()->setOffset(offset);
	this->otherCtrl->setOffset(offset);
  }

  void CalxCoordCtrl::requestInvert() {
	motor_scale_t scale = controller->getMapFilter()->getScale();
	scale.x *= -1;
	scale.y *= -1;
	this->controller->getMapFilter()->setScale(scale);
	this->otherCtrl->setScale(scale);
  }

  void CalxCoordCtrl::requestWatcher() {
	wxThreadEvent evt(wxEVT_COORD_CTRL_WATCHER);
	wxPostEvent(this, evt);
  }

  void CalxCoordCtrl::updateUI() {
	std::string units = wxGetApp().getUnits();
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
		"\n" + FORMAT(__("Position: %sx%s %s"),
					  wxGetApp().formatDouble(
						  ctrl->getFloatPlane()->getFloatPosition().x),
					  wxGetApp().formatDouble(
						  ctrl->getFloatPlane()->getFloatPosition().y),
					  units) +
		"\n" + (ctrl->isMeasured()
					? FORMAT(__("Start: %sx%s %s"),
							 wxGetApp().formatDouble(
								 ctrl->getFloatPlane()->getFloatSize().x),
							 wxGetApp().formatDouble(
								 ctrl->getFloatPlane()->getFloatSize().y),
							 units)
					: __("Start: Not measured")) +
		"\n" + (ctrl->isMeasured()
					? FORMAT(__("Size: %sx%s %s"),
							 wxGetApp().formatDouble(
								 ctrl->getFloatPlane()->getFloatSize().w),
							 wxGetApp().formatDouble(
								 ctrl->getFloatPlane()->getFloatSize().h),
							 units)
					: __("Size: Not measured"));
	this->generalInfoText->SetLabel(general);
	Layout();
  }

  void CalxCoordCtrl::stop() {
	timer.Stop();
	this->queue->stop();
	ctrl->getController()->stop();
  }
  
  void CalxCoordCtrl::OnGraphBuildClick(wxCommandEvent &evt) {
	std::stringstream ss(graphCtrl->getExpression());
	FunctionLexer lexer(ss);
	FunctionParser parser(&lexer);
	Node *node = parser.parse();
	if (node == nullptr) {
	  wxGetApp().getErrorHandler()->handle(ErrorCode::MathExprError);
	  return;
	}
	double minx = graphCtrl->getXMin();
	double maxx = graphCtrl->getXMax();
	double miny = graphCtrl->getYMin();
	double maxy = graphCtrl->getYMax();
	double step = graphCtrl->getStep();
	float speed = graphCtrl->getSpeed();
	CoordTranslator *trans = graphCtrl->getCoordTranslator();
	coord_point_t min = { minx, miny };
	coord_point_t max = { maxx, maxy };
	GraphBuilder *graph = new GraphBuilder(node, min, max, step);
	this->controller->build(trans, graph, speed);
  }

  void CalxCoordCtrl::OnGraphPreviewClick(wxCommandEvent &evt) {
	if (!this->ctrl->isMeasured()) {
	  wxMessageBox(__("Plane need to be measured before preview"),
				   __("Warning"), wxICON_WARNING);
	  return;
	}
	std::stringstream ss(graphCtrl->getExpression());
	FunctionLexer lexer(ss);
	FunctionParser parser(&lexer);
	Node *node = parser.parse();
	if (node == nullptr) {
	  wxGetApp().getErrorHandler()->handle(ErrorCode::MathExprError);
	  return;
	}
	double minx = graphCtrl->getXMin();
	double maxx = graphCtrl->getXMax();
	double miny = graphCtrl->getYMin();
	double maxy = graphCtrl->getYMax();
	double step = graphCtrl->getStep();
	float speed = graphCtrl->getSpeed();
	CoordTranslator *trans = graphCtrl->getCoordTranslator();
	coord_point_t min = { minx, miny };
	coord_point_t max = { maxx, maxy };
	GraphBuilder *graph = new GraphBuilder(node, min, max, step);
	CalxVirtualPlaneDialog *dialog =
		new CalxVirtualPlaneDialog(this, wxID_ANY, ctrl, wxSize(500, 500));
	this->queue->addAction(
		new CalxCoordPreviewAction(this, dialog, trans, graph, speed, true));
	dialog->ShowModal();
	delete dialog;
  }

  void CalxCoordCtrl::OnWatcherClick(wxCommandEvent &evt) {
	if (!this->ctrl->isMeasured()) {
	  wxMessageBox(__("Plane need to be measured before preview"),
				   __("Warning"), wxICON_WARNING);
	  return;
	}
	CalxCoordPlaneWatcherDialog *watcher =
		new CalxCoordPlaneWatcherDialog(this, wxID_ANY, this->ctrl);
	watcher->Show(true);
  }

  void CalxCoordCtrl::OnExit(wxCloseEvent &evt) {
	delete this->controller;
	while (this->watchers.size() > 0) {
	  const auto &w = this->watchers.at(0);
	  w->Close(true);
	}

	this->ctrl->removeEventListener(this->listener);
	this->ctrl->getController()->getXAxis()->removeEventListener(
		this->xListener);
	this->ctrl->getController()->getYAxis()->removeEventListener(
		this->yListener);
	if (this->ctrl->getController()->getInstrument() != nullptr) {
	  this->ctrl->getController()->getInstrument()->removeEventListener(
		  this->instrListener);
	}
	wxGetApp().getSystemManager()->removeCoord(ctrl->getID());

	this->linear->Close(true);
	this->arc->Close(true);
	this->graphCtrl->Close(true);
	this->otherCtrl->Close(true);

	Destroy();
  }

  void CalxCoordCtrl::OnStopClick(wxCommandEvent &evt) {
	this->queue->stopCurrent();
  }

  void CalxCoordCtrl::OnInterfaceUpdate(wxCollapsiblePaneEvent &evt) {
	Layout();
	Refresh();
  }

  void CalxCoordCtrl::OnWatcherRequest(wxThreadEvent &evt) {
	if (this->ctrl->isMeasured()) {
	  CalxCoordPlaneWatcherDialog *watcher =
		  new CalxCoordPlaneWatcherDialog(this, wxID_ANY, this->ctrl);
	  watcher->Show(true);
	}
  }

  void CalxCoordCtrl::OnEnableEvent(wxThreadEvent &evt) {
	setEnabled(evt.GetPayload<bool>());
	wxGetApp().getMainFrame()->getPanel()->getTasks()->updateUI();
  }

  void CalxCoordCtrl::OnAdjusterClick(wxCommandEvent &evt) {
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
}
