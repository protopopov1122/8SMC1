#include <sstream>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include "ctrl-lib/graph/FunctionParser.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include "CalxApp.h"
#include "CalxCoordCtrl.h"
#include "task/CalxTaskPanel.h"

namespace CalX {
	
	CalxCoordCtrl::CalxCoordCtrl(wxWindow *win, wxWindowID id, CoordHandle *ctrl)
		: wxScrolledWindow::wxScrolledWindow(win, id) {
		this->ctrl = ctrl;
		this->used = 0;
		this->master = false;
		
		motor_point_t validateMin = {INT_MIN, INT_MIN};
		motor_point_t validateMax = {INT_MAX, INT_MAX};
		this->validator = new CoordPlaneValidator(validateMin, validateMax, 4000, ctrl->peekPlane());
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
		this->xListener = new CalxCoordDeviceListener(this);
		this->yListener = new CalxCoordDeviceListener(this);
		this->ctrl->getController()->getXAxis()->addEventListener(this->xListener);
		this->ctrl->getController()->getYAxis()->addEventListener(this->yListener);
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);
		
		this->generalPanel = new wxPanel(this, wxID_ANY);
		wxStaticBox *generalBox = new wxStaticBox(generalPanel, wxID_ANY, "General info");
		wxStaticBoxSizer *generalSizer = new wxStaticBoxSizer(generalBox, wxVERTICAL);
		this->generalInfoText = new wxStaticText(generalPanel, wxID_ANY, "");
		this->stopButton = new wxButton(generalPanel, wxID_ANY, "Stop");
		generalSizer->Add(this->generalInfoText, 0, wxALL | wxEXPAND, 5);
		generalSizer->Add(this->stopButton, 0, wxALL, 5);
		this->stopButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnStopClick, this);
		generalPanel->SetSizer(generalSizer);
		sizer->Add(generalPanel, 0, wxALL | wxEXPAND, 0);
		
		this->actionPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *actionSizer = new wxBoxSizer(wxHORIZONTAL);
		actionPanel->SetSizer(actionSizer);
		
		wxPanel *actionSubPanel = new wxPanel(actionPanel, wxID_ANY);
		wxBoxSizer *actionSubSizer = new wxBoxSizer(wxVERTICAL);
		actionSubPanel->SetSizer(actionSubSizer);
		actionSizer->Add(actionSubPanel, 0, wxALL | wxEXPAND);
		
		wxPanel *linearPanel = new wxPanel(actionSubPanel, wxID_ANY);
		wxStaticBox *linearBox = new wxStaticBox(linearPanel, wxID_ANY, "Linear movement");
		wxStaticBoxSizer *linearSizer = new wxStaticBoxSizer(linearBox, wxHORIZONTAL);	
		linearPanel->SetSizer(linearSizer);
		this->linear = new CalxCoordLinearCtrl(this, linearPanel, wxID_ANY);
		linearSizer->Add(linear, 0, wxALL);
		actionSubSizer->Add(linearPanel, 0, wxALL | wxEXPAND);
		
		wxPanel *arcPanel = new wxPanel(actionSubPanel, wxID_ANY);
		wxStaticBox *arcBox = new wxStaticBox(arcPanel, wxID_ANY, "Arc movement");
		wxStaticBoxSizer *arcSizer = new wxStaticBoxSizer(arcBox, wxHORIZONTAL);	
		arcPanel->SetSizer(arcSizer);
		this->arc = new CalxCoordArcCtrl(this, arcPanel, wxID_ANY);
		arcSizer->Add(arc, 0, wxALL);
		actionSubSizer->Add(arcPanel, 0, wxALL | wxEXPAND);
		
		wxPanel *graphPanel = new wxPanel(actionSubPanel, wxID_ANY);
		wxStaticBox *graphBox = new wxStaticBox(graphPanel, wxID_ANY, "Function graph");
		wxStaticBoxSizer *graphSizer = new wxStaticBoxSizer(graphBox, wxHORIZONTAL);	
		graphPanel->SetSizer(graphSizer);
		graphCtrl = new CalxCoordGraphCtrl(this, graphPanel, wxID_ANY);
		graphSizer->Add(graphCtrl, 0, wxALL);
		actionSubSizer->Add(graphPanel, 0, wxALL | wxEXPAND);
		
		wxPanel *actionSub2Panel = new wxPanel(actionPanel, wxID_ANY);
		actionSizer->Add(actionSub2Panel, 1, wxLEFT | wxEXPAND, 5);
		wxStaticBox *otherBox = new wxStaticBox(actionSub2Panel, wxID_ANY, "Other");
		wxStaticBoxSizer *otherSizer = new wxStaticBoxSizer(otherBox, wxVERTICAL);
		actionSub2Panel->SetSizer(otherSizer);
		this->otherCtrl = new CalxCoordOtherCtrl(this, actionSub2Panel, wxID_ANY);
		otherSizer->Add(otherCtrl, 1, wxALL | wxEXPAND);
		
		sizer->Add(actionPanel, 0, wxALL | wxEXPAND, 0);
		Bind(wxEVT_COMMAND_QUEUE_UPDATE, &CalxCoordCtrl::OnQueueUpdate, this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxCoordCtrl::OnExit, this);
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
		std::string general = "Name: Coordinate plane #" + std::to_string(ctrl->getID()) +
								"\nDevices: #" + std::to_string(ctrl->getController()->getXAxis()->getID()) +
									" #" + std::to_string(ctrl->getController()->getYAxis()->getID()) +
								"\nPosition: " + std::to_string(ctrl->getPosition().x) +
									+ "x" + std::to_string(ctrl->getPosition().y); +
								"\nSize: " + std::to_string(ctrl->getSize().w) +
									+ "x" + std::to_string(ctrl->getSize().h);
		this->generalInfoText->SetLabel(general);
	}
	
	void CalxCoordCtrl::stop() {
		timer.Stop();
		ctrl->getController()->kill();
		this->queue->stop();
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
		this->queue->addAction(new CalxCoordMoveAction(this, ctrl, false, linear->isRelative(), dest, linear->getSpeed(), linear->getDivisor()));
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
		long double minx = graphCtrl->getXMin();
		long double maxx = graphCtrl->getXMax();
		long double miny = graphCtrl->getYMin();
		long double maxy = graphCtrl->getYMax();
		long double step = graphCtrl->getStep();
		float speed = graphCtrl->getSpeed();
		CoordTranslator *trans = graphCtrl->getCoordTranslator();
		coord_point_t min = {minx, miny};
		coord_point_t max = {maxx, maxy};
		GraphBuilder *graph = new GraphBuilder(node, min, max, step);
		this->queue->addAction(new CalxCoordGraphAction(this, ctrl, trans, graph, speed));
	}
	
	void CalxCoordCtrl::OnCalibrateClick(wxCommandEvent &evt) {
		TrailerId tr = otherCtrl->getTrailer();
		this->queue->addAction(new CalxCoordCalibrateAction(this, ctrl, tr));
	}
	
	void CalxCoordCtrl::OnUpdateFiltersClick(wxCommandEvent &evt) {
		log->setLoggingActions(otherCtrl->isLoggingActions());
		log->setLoggingErrors(otherCtrl->isLoggingErrors());
		motor_point_t moffset = {otherCtrl->getXOffset(), otherCtrl->getYOffset()};
		motor_scale_t mscale = {otherCtrl->getXScale(), otherCtrl->getYScale()};
		map->setOffset(moffset);
		map->setScale(mscale);
		motor_point_t min = {otherCtrl->getMinX(), otherCtrl->getMinY()};
		motor_point_t max = {otherCtrl->getMaxX(), otherCtrl->getMaxY()};
		validator->setMinimum(min);
		validator->setMaximum(max);
		validator->setMaxSpeed(otherCtrl->getSpeed());
	}
	
	void CalxCoordCtrl::OnExit(wxCloseEvent &evt) {
		this->ctrl->removeEventListener(this->listener);
		this->ctrl->getController()->getXAxis()->removeEventListener(this->xListener);
		this->ctrl->getController()->getYAxis()->removeEventListener(this->yListener);
		wxGetApp().getSystemManager()->removeCoord(ctrl->getID());
	}
	
	void CalxCoordCtrl::OnQueueUpdate(wxThreadEvent &evt) {
		
	}
	
	void CalxCoordCtrl::OnStopClick(wxCommandEvent &evt) {
		this->queue->stopCurrent();
	}
}