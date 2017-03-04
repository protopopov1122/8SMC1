#include "CalxCoordCtrl.h"
#include "CalxApp.h"
#include <wx/stattext.h>
#include <wx/sizer.h>

namespace CalX {
	
	void CalxCoordLinearCtrl::init() {
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->speed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 4000, 4000);
		this->divisor = new wxChoice(this, wxID_ANY);
		this->divisor->Append("1");
		this->divisor->Append("2");
		this->divisor->Append("4");
		this->divisor->Append("8");
		this->divisor->SetSelection(3);
		this->relative = new wxCheckBox(this, wxID_ANY, "Relative");
		wxButton *moveButton = new wxButton(this, wxID_ANY, "Move");
		wxButton *jumpButton = new wxButton(this, wxID_ANY, "Jump");
		moveButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnLinearMoveClick, ctrl);
		jumpButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnLinearJumpClick, ctrl);
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Destination:"));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(xCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(yCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Speed:"));
		sizer->Add(speed, 0, wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Divisor:"));
		sizer->Add(divisor);
		sizer->Add(relative, 0, wxALIGN_CENTER);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(moveButton);
		sizer->Add(jumpButton);
	}
	
	void CalxCoordArcCtrl::init() {
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->cxCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->cyCoord = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->speed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 4000, 4000);
		this->divisor = new wxChoice(this, wxID_ANY);
		this->divisor->Append("1");
		this->divisor->Append("2");
		this->divisor->Append("4");
		this->divisor->Append("8");
		this->divisor->SetSelection(3);
		this->splitter = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 200);
		this->clockwise = new wxCheckBox(this, wxID_ANY, "Clockwise");
		this->relative = new wxCheckBox(this, wxID_ANY, "Relative");
		wxButton *moveButton = new wxButton(this, wxID_ANY, "Move");
		moveButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnArcMoveClick, ctrl);
		
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Destination:"));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(xCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(yCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Center:"));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, "x:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(cxCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "y:"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		sizer->Add(cyCoord, 0, wxALL | wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Speed:"));
		sizer->Add(speed, 0, wxEXPAND);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Divisor:"));
		sizer->Add(divisor);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Splitter:"));
		sizer->Add(splitter, 0, wxEXPAND);
		sizer->Add(clockwise);
		sizer->Add(relative);
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(moveButton);
		
		
	}
	
	CalxCoordEventListener::CalxCoordEventListener(CalxCoordCtrl *ctrl) {
		this->ctrl = ctrl;
	}
	
	CalxCoordEventListener::~CalxCoordEventListener() {
	}
	
	void CalxCoordEventListener::moving(CoordMoveEvent &evt) {
		this->ctrl->Enable(false);
		this->ctrl->updateUI();
	}
	
	void CalxCoordEventListener::moved(CoordMoveEvent &evt) {
		this->ctrl->Enable(true);
		this->ctrl->updateUI();
	}
	
	void CalxCoordEventListener::stopped(CoordErrorEvent &evt) {
		this->ctrl->Enable(true);
		this->ctrl->updateUI();
	}
	
	void CalxCoordEventListener::calibrating(CoordCalibrateEvent &evt) {
		this->ctrl->Enable(false);
		this->ctrl->updateUI();
	}
	
	void CalxCoordEventListener::calibrated(CoordCalibrateEvent &evt) {
		this->ctrl->Enable(true);
		this->ctrl->updateUI();
	}
	
	class CalxCoordMoveAction : public CalxAction {
		public:
			CalxCoordMoveAction(CoordHandle *handle, bool jump, bool relative, motor_point_t dest, float speed, int div) {
				this->handle = handle;
				this->jump = jump;
				this->relative = relative;
				this->dest = dest;
				this->speed = speed;
				this->div = div;
			}
			
			virtual void perform(SystemManager *sysman) {
				if (relative) {
					handle->relativeMove(dest, speed, div, jump);
				} else {
					handle->move(dest, speed, div, jump);
				}
			}
		private:
			CoordHandle *handle;
			bool jump;
			bool relative;
			motor_point_t dest;
			float speed;
			int div;
	};
	
	class CalxCoordArcAction : public CalxAction {
		public:
			CalxCoordArcAction(CoordHandle *handle, bool relative, motor_point_t dest, motor_point_t cen, int splitter, float speed, int div, bool clockwise) {
				this->handle = handle;
				this->relative = relative;
				this->dest = dest;
				this->cen = cen;
				this->splitter = splitter;
				this->speed = speed;
				this->div = div;
				this->clockwise = clockwise;
			}
			
			virtual void perform(SystemManager *sysman) {
				if (relative) {
					handle->relativeArc(dest, cen, splitter, speed, div, clockwise);
				} else {
					handle->arc(dest, cen, splitter, speed, div, clockwise);
				}
			}
		private:
			CoordHandle *handle;
			bool relative;
			motor_point_t dest;
			motor_point_t cen;
			int splitter;
			float speed;
			int div;
			bool clockwise;
	};
	
	CalxCoordCtrl::CalxCoordCtrl(wxWindow *win, wxWindowID id, CoordHandle *ctrl)
		: wxPanel::wxPanel(win, id) {
		this->ctrl = ctrl;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->queue->Run();
		this->listener = new CalxCoordEventListener(this);
		this->ctrl->addEventListener(this->listener);
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);
		
		wxPanel *generalPanel = new wxPanel(this, wxID_ANY);
		wxStaticBox *generalBox = new wxStaticBox(generalPanel, wxID_ANY, "General info");
		wxStaticBoxSizer *generalSizer = new wxStaticBoxSizer(generalBox, wxVERTICAL);
		this->generalInfoText = new wxStaticText(generalPanel, wxID_ANY, "");
		generalSizer->Add(this->generalInfoText, 0, wxALL | wxEXPAND, 5);
		generalPanel->SetSizer(generalSizer);
		sizer->Add(generalPanel, 0, wxALL | wxEXPAND, 10);
		
		
		wxPanel *linearPanel = new wxPanel(this, wxID_ANY);
		wxStaticBox *linearBox = new wxStaticBox(linearPanel, wxID_ANY, "Linear movement");
		wxStaticBoxSizer *linearSizer = new wxStaticBoxSizer(linearBox, wxHORIZONTAL);	
		linearPanel->SetSizer(linearSizer);
		this->linear = new CalxCoordLinearCtrl(this, linearPanel, wxID_ANY);
		linearSizer->Add(linear, 0, wxALL | wxEXPAND);
		sizer->Add(linearPanel, 0, wxALL | wxEXPAND, 10);
		
		wxPanel *arcPanel = new wxPanel(this, wxID_ANY);
		wxStaticBox *arcBox = new wxStaticBox(arcPanel, wxID_ANY, "Arc movement");
		wxStaticBoxSizer *arcSizer = new wxStaticBoxSizer(arcBox, wxHORIZONTAL);	
		arcPanel->SetSizer(arcSizer);
		this->arc = new CalxCoordArcCtrl(this, arcPanel, wxID_ANY);
		arcSizer->Add(arc, 0, wxALL | wxEXPAND);
		sizer->Add(arcPanel, 0, wxALL | wxEXPAND, 10);
		
		Bind(wxEVT_COMMAND_QUEUE_UPDATE, &CalxCoordCtrl::OnQueueUpdate, this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxCoordCtrl::OnExit, this);
		updateUI();
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
		Layout();
	}
	
	void CalxCoordCtrl::OnLinearMoveClick(wxCommandEvent &evt) {
		motor_point_t dest = {linear->getCoordX(), linear->getCoordY()};
		this->queue->addAction(new CalxCoordMoveAction(ctrl, false, linear->isRelative(), dest, linear->getSpeed(), linear->getDivisor()));
	}
	
	void CalxCoordCtrl::OnLinearJumpClick(wxCommandEvent &evt) {
		motor_point_t dest = {linear->getCoordX(), linear->getCoordY()};
		this->queue->addAction(new CalxCoordMoveAction(ctrl, false, linear->isRelative(), dest, linear->getSpeed(), linear->getDivisor()));
	}
	
	void CalxCoordCtrl::OnArcMoveClick(wxCommandEvent &evt) {
		motor_point_t dest = {arc->getCoordX(), arc->getCoordY()};
		motor_point_t cen = {arc->getCenterCoordX(), arc->getCenterCoordY()};
		this->queue->addAction(new CalxCoordArcAction(ctrl, arc->isRelative(), dest, cen, arc->getSplitter(), arc->getSpeed(), arc->getDivisor(), arc->isClockwise()));
	}
	
	void CalxCoordCtrl::OnExit(wxCloseEvent &evt) {
		this->ctrl->removeEventListener(this->listener);
		this->queue->stop();
		this->queue->Kill();
		wxGetApp().getSystemManager()->removeCoord(ctrl->getID());
	}
	
	void CalxCoordCtrl::OnQueueUpdate(wxThreadEvent &evt) {
		
	}
}