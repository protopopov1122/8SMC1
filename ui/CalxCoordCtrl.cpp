#include "CalxCoordCtrl.h"
#include "CalxApp.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/choice.h>

namespace CalX {
	
	CalxCoordEventListener::CalxCoordEventListener(CalxCoordCtrl *ctrl) {
		this->ctrl = ctrl;
	}
	
	CalxCoordEventListener::~CalxCoordEventListener() {
	}
	
	void CalxCoordEventListener::moving(CoordMoveEvent &evt) {
		this->ctrl->updateUI();
	}
	
	void CalxCoordEventListener::moved(CoordMoveEvent &evt) {
		this->ctrl->updateUI();
	}
	
	void CalxCoordEventListener::stopped(CoordErrorEvent &evt) {
		this->ctrl->updateUI();
	}
	
	void CalxCoordEventListener::calibrating(CoordCalibrateEvent &evt) {
		this->ctrl->updateUI();
	}
	
	void CalxCoordEventListener::calibrated(CoordCalibrateEvent &evt) {
		this->ctrl->updateUI();
	}
	
	CalxCoordCtrl::CalxCoordCtrl(wxWindow *win, wxWindowID id, CoordHandle *ctrl)
		: wxPanel::wxPanel(win, id) {
		this->ctrl = ctrl;
		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->listener = new CalxCoordEventListener(this);
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		
		wxPanel *generalPanel = new wxPanel(this, wxID_ANY);
		wxStaticBox *generalBox = new wxStaticBox(generalPanel, wxID_ANY, "General info");
		wxStaticBoxSizer *generalSizer = new wxStaticBoxSizer(generalBox, wxVERTICAL);
		this->generalInfoText = new wxStaticText(generalPanel, wxID_ANY, "");
		generalSizer->Add(this->generalInfoText, 0, wxALL | wxEXPAND, 5);
		generalPanel->SetSizer(generalSizer);
		sizer->Add(generalPanel, 0, wxALL | wxEXPAND, 10);
		
		
		wxPanel *movementPanel = new wxPanel(this, wxID_ANY);
		wxStaticBox *movementBox = new wxStaticBox(movementPanel, wxID_ANY, "Simple movement");
		wxStaticBoxSizer *movementSizer = new wxStaticBoxSizer(movementBox, wxHORIZONTAL);
		
		
		wxPanel *movPrmsPanel = new wxPanel(movementPanel, wxID_ANY);
		wxFlexGridSizer *movPrmsSizer = new wxFlexGridSizer(4);
		wxStaticText *coordLabel = new wxStaticText(movPrmsPanel, wxID_ANY, "Coordinate: ");
		wxStaticText *centerLabel = new wxStaticText(movPrmsPanel, wxID_ANY, "Center: ");
		wxStaticText *speedLabel = new wxStaticText(movPrmsPanel, wxID_ANY, "Speed: ");
		wxStaticText *divLabel = new wxStaticText(movPrmsPanel, wxID_ANY, "Divisor: ");
		wxTextCtrl *coordX = new wxTextCtrl(movPrmsPanel, wxID_ANY, "0");
		wxTextCtrl *coordY = new wxTextCtrl(movPrmsPanel, wxID_ANY, "0");
		wxTextCtrl *centerX = new wxTextCtrl(movPrmsPanel, wxID_ANY, "0");
		wxTextCtrl *centerY = new wxTextCtrl(movPrmsPanel, wxID_ANY, "0");
		wxTextCtrl *speedText = new wxTextCtrl(movPrmsPanel, wxID_ANY, "0");
		wxChoice *divCh = new wxChoice(movPrmsPanel, wxID_ANY);
		divCh->Append("1");
		divCh->Append("2");
		divCh->Append("4");
		divCh->Append("8");
		divCh->SetSelection(3);
		wxStaticText *x1Label = new wxStaticText(movPrmsPanel, wxID_ANY, "x");
		wxStaticText *x2Label = new wxStaticText(movPrmsPanel, wxID_ANY, "x");
		wxStaticText *speed2Label = new wxStaticText(movPrmsPanel, wxID_ANY, "");
		wxStaticText *speed3Label = new wxStaticText(movPrmsPanel, wxID_ANY, "");
		wxStaticText *div2Label = new wxStaticText(movPrmsPanel, wxID_ANY, "");
		wxStaticText *div3Label = new wxStaticText(movPrmsPanel, wxID_ANY, "");
		movPrmsSizer->Add(coordLabel);
		movPrmsSizer->Add(coordX);
		movPrmsSizer->Add(x1Label);
		movPrmsSizer->Add(coordY);
		movPrmsSizer->Add(centerLabel);
		movPrmsSizer->Add(centerX);
		movPrmsSizer->Add(x2Label);
		movPrmsSizer->Add(centerY);
		movPrmsSizer->Add(speedLabel);
		movPrmsSizer->Add(speedText);
		movPrmsSizer->Add(speed2Label);
		movPrmsSizer->Add(speed3Label);
		movPrmsSizer->Add(divLabel);
		movPrmsSizer->Add(divCh);
		movPrmsSizer->Add(div2Label);
		movPrmsSizer->Add(div3Label);
		movPrmsPanel->SetSizer(movPrmsSizer);
		movementSizer->Add(movPrmsPanel, 0, wxALL, 5);
		movementPanel->SetSizer(movementSizer);
				
		wxPanel *movCmdPanel = new wxPanel(movementPanel, wxID_ANY);
		wxFlexGridSizer *movCmdSizer = new wxFlexGridSizer(2);
		wxButton *moveButton = new wxButton(movCmdPanel, wxID_ANY, "Move");
		wxButton *rmoveButton = new wxButton(movCmdPanel, wxID_ANY, "Relational Move");
		wxButton *jumpButton = new wxButton(movCmdPanel, wxID_ANY, "Jump");
		wxButton *rjumpButton = new wxButton(movCmdPanel, wxID_ANY, "Relational Jump");
		wxButton *arcButton = new wxButton(movCmdPanel, wxID_ANY, "Arc");
		wxButton *rarcButton = new wxButton(movCmdPanel, wxID_ANY, "Relational Arc");
		movCmdSizer->Add(moveButton);
		movCmdSizer->Add(rmoveButton);
		movCmdSizer->Add(jumpButton);
		movCmdSizer->Add(rjumpButton);
		movCmdSizer->Add(arcButton);
		movCmdSizer->Add(rarcButton);
		movCmdPanel->SetSizer(movCmdSizer);
		movementSizer->Add(movCmdPanel, 0, wxALL, 5);
		
		
		sizer->Add(movementPanel, 0, wxALL | wxEXPAND, 10);
		this->SetSizer(sizer);
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
	
	void CalxCoordCtrl::OnExit(wxCloseEvent &evt) {
		this->ctrl->removeEventListener(this->listener);
		this->queue->stop();
		this->queue->Kill();
		wxGetApp().getSystemManager()->removeCoord(ctrl->getID());
	}
	
	void CalxCoordCtrl::OnQueueUpdate(wxThreadEvent &evt) {
		
	}
}