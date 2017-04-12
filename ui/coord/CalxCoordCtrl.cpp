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
#include "CalxApp.h"
#include "CalxErrorHandler.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include "ctrl-lib/graph/FunctionParser.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include "CalxCoordCtrl.h"
#include "task/CalxTaskPanel.h"
#include "CalxCoordPlaneWatcher.h"

namespace CalXUI {
	
	CalxCoordCtrl::CalxCoordCtrl(wxWindow *win, wxWindowID id, CoordHandle *ctrl)
		: wxScrolledWindow::wxScrolledWindow(win, id) {
		this->ctrl = ctrl;
		this->used = 0;
		this->master = false;
		
		motor_point_t validateMin = {INT_MIN, INT_MIN};
		motor_point_t validateMax = {INT_MAX, INT_MAX};
		this->validator = new CoordPlaneValidator(validateMin, validateMax, wxGetApp().getSystemManager()->getConfiguration()->getEntry("core")->getInt("maxspeed", 4000), ctrl->peekPlane());
		ctrl->pushPlane(this->validator);
		this->log = new CoordPlaneLog(ctrl->peekPlane(), &std::cout, "Plane #" + std::to_string(ctrl->getID()) + ": ");
		ctrl->pushPlane(this->log);
		motor_point_t mapOffset = {0, 0};
		motor_scale_t mapScale = {1.0f, 1.0f};
		this->map = new CoordPlaneMap(mapOffset, mapScale, ctrl->peekPlane());
		ctrl->pushPlane(this->map);
		
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->queue->Run();
		this->listener = new CalxCoordEventListener(this);
		this->ctrl->addEventListener(this->listener);
		this->xListener = new CalxCoordMotorListener(this);
		this->yListener = new CalxCoordMotorListener(this);
		this->ctrl->getController()->getXAxis()->addEventListener(this->xListener);
		this->ctrl->getController()->getYAxis()->addEventListener(this->yListener);
		if (this->ctrl->getController()->getInstrument() != nullptr) {
			this->instrListener = new CalxCoordInstrumentListener(this);
			this->ctrl->getController()->getInstrument()->addEventListener(this->instrListener);
		}
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);
		
		this->generalPanel = new wxPanel(this, wxID_ANY);
		wxStaticBox *generalBox = new wxStaticBox(generalPanel, wxID_ANY, __("General info"));
		wxStaticBoxSizer *generalSizer = new wxStaticBoxSizer(generalBox, wxHORIZONTAL);
		generalPanel->SetSizer(generalSizer);
		this->generalInfoText = new wxStaticText(generalPanel, wxID_ANY, "");
		wxPanel *generalButtonPanel = new wxPanel(generalPanel, wxID_ANY);
		wxBoxSizer *generalButtonSizer = new wxBoxSizer(wxVERTICAL);
		generalButtonPanel->SetSizer(generalButtonSizer);
		wxButton *watcherButton = new wxButton(generalButtonPanel, wxID_ANY, __("Watcher"));
		watcherButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnWatcherClick, this);
		this->stopButton = new wxButton(generalButtonPanel, wxID_ANY, __("Stop"));
		generalSizer->Add(this->generalInfoText, 0, wxTOP | wxEXPAND, 5);
		generalSizer->Add(generalButtonPanel, 0, wxALL, 10);
		generalButtonSizer->Add(watcherButton, 0, wxALL);
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
		
		wxCollapsiblePane *linearPane = new wxCollapsiblePane(actionSubPanel, wxID_ANY, __("Linear movement"));
		actionSubSizer->Add(linearPane, 0, wxALL | wxEXPAND);
		wxWindow *linearPanel = linearPane->GetPane();
		wxBoxSizer *linearSizer = new wxBoxSizer(wxHORIZONTAL);
		linearPanel->SetSizer(linearSizer);
		this->linear = new CalxCoordLinearCtrl(linearPanel, wxID_ANY);
		linearSizer->Add(linear, 0, wxALL);
		linear->getMoveButton()->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnLinearMoveClick, this);
		linear->getJumpButton()->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnLinearJumpClick, this);
		linearPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &CalxCoordCtrl::OnInterfaceUpdate, this);
		
		
		wxCollapsiblePane *arcPane = new wxCollapsiblePane(actionSubPanel, wxID_ANY, __("Arc movement"));
		actionSubSizer->Add(arcPane, 0, wxALL | wxEXPAND);
		wxWindow *arcPanel = arcPane->GetPane();
		wxBoxSizer *arcSizer = new wxBoxSizer(wxHORIZONTAL);
		arcPanel->SetSizer(arcSizer);
		this->arc = new CalxCoordArcCtrl(arcPanel, wxID_ANY);
		arcSizer->Add(arc, 0, wxALL);
		arc->getMoveButton()->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnArcMoveClick, this);
		arcPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &CalxCoordCtrl::OnInterfaceUpdate, this);
		
		wxCollapsiblePane *graphPane = new wxCollapsiblePane(actionSubPanel, wxID_ANY, __("Function graph"));
		actionSubSizer->Add(graphPane, 0, wxALL | wxEXPAND);
		wxWindow *graphPanel = graphPane->GetPane();
		wxBoxSizer *graphSizer = new wxBoxSizer(wxHORIZONTAL);
		graphPanel->SetSizer(graphSizer);
		this->graphCtrl = new CalxCoordGraphCtrl(graphPanel, wxID_ANY);
		graphSizer->Add(graphCtrl, 0, wxALL);
		graphCtrl->getBuildButton()->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnGraphBuildClick, this);
		graphCtrl->getPreviewButton()->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnGraphPreviewClick, this);
		graphPane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &CalxCoordCtrl::OnInterfaceUpdate, this);
		graphPane->Collapse(false);
		
		wxPanel *actionSub2Panel = new wxPanel(actionPanel, wxID_ANY);
		actionSizer->Add(actionSub2Panel, 1, wxLEFT | wxEXPAND, 5);
		wxStaticBox *otherBox = new wxStaticBox(actionSub2Panel, wxID_ANY, __("Other"));
		wxStaticBoxSizer *otherSizer = new wxStaticBoxSizer(otherBox, wxVERTICAL);
		actionSub2Panel->SetSizer(otherSizer);
		this->otherCtrl = new CalxCoordOtherCtrl(this, actionSub2Panel, wxID_ANY);
		otherSizer->Add(otherCtrl, 1, wxALL | wxEXPAND);
		
		sizer->Add(actionPanel, 0, wxALL | wxEXPAND, 0);
		Bind(wxEVT_COMMAND_QUEUE_UPDATE, &CalxCoordCtrl::OnQueueUpdate, this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxCoordCtrl::OnExit, this);
		graphCtrl->Bind(wxEVT_CLOSE_WINDOW, &CalxCoordGraphCtrl::OnClose, graphCtrl);
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
			setEnabled(false);
			wxGetApp().getMainFrame()->getPanel()->getTasks()->updateUI();
		}
	}
	
	void CalxCoordCtrl::unuse() {
		this->used--;
		if (this->used == 0) {
			setEnabled(true);
			wxGetApp().getMainFrame()->getPanel()->getTasks()->updateUI();
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
	
	void CalxCoordCtrl::updateUI() {
		std::string general = FORMAT(__("Name: Coordinate plane #%s"), std::to_string(ctrl->getID())) + "\n" +
							  FORMAT(__("Devices: #%s #%s"), std::to_string(ctrl->getController()->getXAxis()->getID()),
									std::to_string(ctrl->getController()->getYAxis()->getID())) + "\n" +
							  (ctrl->getController()->getInstrument() != nullptr ?
								FORMAT(__("Instrument #%s"), std::to_string(ctrl->getController()->getInstrument()->getID())) :
								__("Instrument: no")) + "\n" +
							  FORMAT(__("Position: %sx%s"), std::to_string(ctrl->getPosition().x),
									std::to_string(ctrl->getPosition().y)) + "\n" +
							  FORMAT(__("Start: %s"), (ctrl->isMeasured() ? (std::to_string(ctrl->getSize().x) +
									+ __("x") + std::to_string(ctrl->getSize().y)) : __("Not measured"))) + "\n" +
							  FORMAT(__("Size: %s"), (ctrl->isMeasured() ? (std::to_string(ctrl->getSize().w) +
									+ __("x") + std::to_string(ctrl->getSize().h)) : __("Not measured")));
		this->generalInfoText->SetLabel(general);
	}
	
	void CalxCoordCtrl::stop() {
		timer.Stop();
		this->queue->stop();
		ctrl->getController()->stop();
	}
	
	CoordPlaneLog *CalxCoordCtrl::getPlaneLog() {
		return this->log;
	}
	CoordPlaneMap *CalxCoordCtrl::getPlaneMap() {
		return this->map;
	}
	CoordPlaneValidator *CalxCoordCtrl::getPlaneValidator() {
		return this->validator;
	}
	
	void CalxCoordCtrl::OnLinearMoveClick(wxCommandEvent &evt) {
		motor_point_t dest = {linear->getCoordX(), linear->getCoordY()};
		this->queue->addAction(new CalxCoordMoveAction(this, ctrl, true, linear->isRelative(), dest, linear->getSpeed(), linear->getDivisor()));
	}
	
	void CalxCoordCtrl::OnLinearJumpClick(wxCommandEvent &evt) {
		motor_point_t dest = {linear->getCoordX(), linear->getCoordY()};
		this->queue->addAction(new CalxCoordMoveAction(this, ctrl, false, linear->isRelative(), dest, linear->getSpeed(), linear->getDivisor()));
	}
	
	void CalxCoordCtrl::OnArcMoveClick(wxCommandEvent &evt) {
		motor_point_t dest = {arc->getCoordX(), arc->getCoordY()};
		motor_point_t cen = {arc->getCenterCoordX(), arc->getCenterCoordY()};
		this->queue->addAction(new CalxCoordArcAction(this, ctrl, arc->isRelative(), dest, cen, arc->getSplitter(), arc->getSpeed(), arc->getDivisor(), arc->isClockwise()));
	}
	
	void CalxCoordCtrl::OnGraphBuildClick(wxCommandEvent &evt) {
		std::stringstream ss(graphCtrl->getExpression());
		FunctionLexer lexer(ss);
		FunctionParser parser(&lexer);
		Node *node = parser.parse();
		if (node == nullptr) {
			wxGetApp().getErrorHandler()->handle(ErrorCode::MathExprError);
		}
		double minx = graphCtrl->getXMin();
		double maxx = graphCtrl->getXMax();
		double miny = graphCtrl->getYMin();
		double maxy = graphCtrl->getYMax();
		double step = graphCtrl->getStep();
		float speed = graphCtrl->getSpeed();
		CoordTranslator *trans = graphCtrl->getCoordTranslator();
		coord_point_t min = {minx, miny};
		coord_point_t max = {maxx, maxy};
		GraphBuilder *graph = new GraphBuilder(node, min, max, step);
		this->queue->addAction(new CalxCoordGraphAction(this, ctrl, trans, graph, speed));
	}
	
	void CalxCoordCtrl::OnGraphPreviewClick(wxCommandEvent &evt) {
		if (!this->ctrl->isMeasured()) {
			wxMessageBox(__("Plane need to be measured before preview"), __("Warning"), wxICON_WARNING);
			return;
		}
		std::stringstream ss(graphCtrl->getExpression());
		FunctionLexer lexer(ss);
		FunctionParser parser(&lexer);
		Node *node = parser.parse();
		if (node == nullptr) {
			wxGetApp().getErrorHandler()->handle(ErrorCode::MathExprError);
		}
		double minx = graphCtrl->getXMin();
		double maxx = graphCtrl->getXMax();
		double miny = graphCtrl->getYMin();
		double maxy = graphCtrl->getYMax();
		double step = graphCtrl->getStep();
		float speed = graphCtrl->getSpeed();
		CoordTranslator *trans = graphCtrl->getCoordTranslator();
		coord_point_t min = {minx, miny};
		coord_point_t max = {maxx, maxy};
		GraphBuilder *graph = new GraphBuilder(node, min, max, step);
		CalxVirtualPlaneDialog *dialog = new CalxVirtualPlaneDialog(this, wxID_ANY, ctrl, wxSize(500, 500));
		this->queue->addAction(new CalxCoordPreviewAction(this, dialog, trans, graph, speed));
		dialog->Enable(false);
		dialog->ShowModal();
		delete dialog;
	}
	
	void CalxCoordCtrl::OnCalibrateClick(wxCommandEvent &evt) {
		TrailerId tr = otherCtrl->getTrailer();
		this->queue->addAction(new CalxCoordCalibrateAction(this, ctrl, tr));
	}
	
	void CalxCoordCtrl::OnMeasureClick(wxCommandEvent &evt) {
		TrailerId tr = otherCtrl->getMeasureTrailer();
		this->queue->addAction(new CalxCoordMeasureAction(this, ctrl, tr));
	}
	
	void CalxCoordCtrl::OnUpdateFiltersClick(wxCommandEvent &evt) {
		log->setLoggingActions(otherCtrl->isLoggingActions());
		log->setLoggingErrors(otherCtrl->isLoggingErrors());
		motor_point_t moffset = {otherCtrl->getXOffset(), otherCtrl->getYOffset()};
		motor_scale_t mscale = {otherCtrl->getXScale(), otherCtrl->getYScale()};
		if (std::isnan(mscale.x)) {
			wxMessageBox(__("Enter valid real value"), __("Error"), wxICON_ERROR);
			otherCtrl->setXScale(map->getScale().x);
			return;
		}
		if (std::isnan(mscale.y)) {
			wxMessageBox(__("Enter valid real value"), __("Error"), wxICON_ERROR);
			otherCtrl->setYScale(map->getScale().y);
			return;
		}
		map->setOffset(moffset);
		map->setScale(mscale);
		motor_point_t min = {otherCtrl->getMinX(), otherCtrl->getMinY()};
		motor_point_t max = {otherCtrl->getMaxX(), otherCtrl->getMaxY()};
		validator->setMinimum(min);
		validator->setMaximum(max);
		validator->setMaxSpeed(otherCtrl->getSpeed());
	}
	
	void CalxCoordCtrl::OnWatcherClick(wxCommandEvent &evt) {
		if (!this->ctrl->isMeasured()) {
			wxMessageBox(__("Plane need to be measured before preview"), __("Warning"), wxICON_WARNING);
			return;
		}
		CalxCoordPlaneWatcherDialog *watcher = new CalxCoordPlaneWatcherDialog(this, wxID_ANY, this->ctrl);
		watcher->Show(true);
	}
	
	void CalxCoordCtrl::OnExit(wxCloseEvent &evt) {
		stop();
		
		this->ctrl->removeEventListener(this->listener);
		this->ctrl->getController()->getXAxis()->removeEventListener(this->xListener);
		this->ctrl->getController()->getYAxis()->removeEventListener(this->yListener);
		if (this->ctrl->getController()->getInstrument() != nullptr) {
			this->ctrl->getController()->getInstrument()->removeEventListener(this->instrListener);
		}
		wxGetApp().getSystemManager()->removeCoord(ctrl->getID());
		
		this->linear->Close(true);
		this->arc->Close(true);
		this->graphCtrl->Close(true);
		this->otherCtrl->Close(true);
		
		delete this->queue;
		
		Destroy();
	}
	
	void CalxCoordCtrl::OnQueueUpdate(wxThreadEvent &evt) {
		
	}
	
	void CalxCoordCtrl::OnStopClick(wxCommandEvent &evt) {
		this->queue->stopCurrent();
	}
	
	void CalxCoordCtrl::OnInstrumentEnableClick(wxCommandEvent &evt) {
		this->ctrl->getController()->getInstrument()->setRunnable(this->otherCtrl->isInstrumentEnabled());
	}
	
	void CalxCoordCtrl::OnInterfaceUpdate(wxCollapsiblePaneEvent &evt) {
		Layout();
		Refresh();
	}
}