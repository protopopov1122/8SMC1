#include "CalxCoordCtrl.h"
#include "CalxApp.h"
#include "ctrl-lib/graph/FunctionParser.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include <sstream>
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
		sizer->Add(moveButton);
		
		
	}
	
	void CalxCoordGraphCtrl::init() {
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);
		
		
		wxPanel *graphPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(graphPanel, 0, wxRIGHT, 10);
		wxFlexGridSizer *graphSizer = new wxFlexGridSizer(2);
		graphPanel->SetSizer(graphSizer);
		
		this->expr = new wxTextCtrl(graphPanel, wxID_ANY, "x");
		this->xmin = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, -10);
		this->xmax = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 10);
		this->ymin = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, -10);
		this->ymax = new wxSpinCtrl(graphPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 10);
		this->step = new wxTextCtrl(graphPanel, wxID_ANY, "1");
		wxButton *buildButton = new wxButton(graphPanel, wxID_ANY, "Build");
		buildButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnGraphBuildClick, ctrl);
		
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "Function "), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(expr, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "X axis range:"));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "min"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "max"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(xmax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "Y axis range:"));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, ""));
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "min"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymin, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "max"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(ymax, 0, wxEXPAND);
		graphSizer->Add(new wxStaticText(graphPanel, wxID_ANY, "X axis step"), 0, wxALIGN_RIGHT | wxRIGHT, 10);
		graphSizer->Add(step, 0, wxEXPAND);
		graphSizer->Add(buildButton);
		
		wxPanel *coordPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(coordPanel, 0, wxLEFT, 10);
		wxFlexGridSizer *coordSizer = new wxFlexGridSizer(2);
		coordPanel->SetSizer(coordSizer);
		
		this->xoffset = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yoffset = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->xscale = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 1);
		this->yscale = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 1);
		this->speed = new wxSpinCtrl(coordPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 4000, 4000, 4000);
		
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "X axis offset"), 0, wxRIGHT, 10);
		coordSizer->Add(xoffset, 0, wxEXPAND);
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "Y axis offset"), 0, wxRIGHT, 10);
		coordSizer->Add(yoffset, 0, wxEXPAND);
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "X axis scale"), 0, wxRIGHT, 10);
		coordSizer->Add(xscale, 0, wxEXPAND);
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "Y axis scale"), 0, wxRIGHT, 10);
		coordSizer->Add(yscale, 0, wxEXPAND);
		coordSizer->Add(new wxStaticText(coordPanel, wxID_ANY, "Build speed"), 0, wxRIGHT, 10);
		coordSizer->Add(speed, 0, wxEXPAND);
	}
	
	void CalxCoordOtherCtrl::init() {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		
		wxPanel *calibratePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(calibratePanel, 0, wxALL, 5);
		wxBoxSizer *calibrateSizer = new wxBoxSizer(wxHORIZONTAL);
		calibratePanel->SetSizer(calibrateSizer);
		wxButton *calibrateButton = new wxButton(calibratePanel, wxID_ANY, "Calibrate");
		calibrateButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnCalibrateClick, ctrl);
		calibrateSizer->Add(calibrateButton);
		calibrateSizer->Add(new wxStaticText(calibratePanel, wxID_ANY, " to "), wxALL | wxALIGN_CENTER, 5);
		trailer = new wxChoice(calibratePanel, wxID_ANY);
		trailer->Append("trailer 1");
		trailer->Append("trailer 2");
		trailer->SetSelection(0);
		calibrateSizer->Add(trailer, 0, wxALL);
		
		wxPanel *logPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(logPanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *logSizer = new wxBoxSizer(wxHORIZONTAL);
		logPanel->SetSizer(logSizer);
		this->logActions = new wxCheckBox(logPanel, wxID_ANY, "Log actions");		
		this->logErrors = new wxCheckBox(logPanel, wxID_ANY, "Log errors");
		logSizer->Add(logActions);
		logSizer->Add(logErrors);
		
		wxPanel *mapPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mapPanel, 0, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *mapSizer = new wxFlexGridSizer(2);
		mapPanel->SetSizer(mapSizer);
		this->xOffset = new wxSpinCtrl(mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yOffset = new wxSpinCtrl(mapPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->xScale = new wxTextCtrl(mapPanel, wxID_ANY, "1");
		this->yScale = new wxTextCtrl(mapPanel, wxID_ANY, "1");
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "Coordinate offset:"));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, ""));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "x:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(xOffset, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "y:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(yOffset, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "Coordinate scale:"));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, ""));
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "x:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(xScale, 0, wxEXPAND);
		mapSizer->Add(new wxStaticText(mapPanel, wxID_ANY, "y:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		mapSizer->Add(yScale, 0, wxEXPAND);
		
		wxPanel *validatePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(validatePanel, 0, wxALL | wxEXPAND, 5);
		wxFlexGridSizer *validateSizer = new wxFlexGridSizer(2);
		validatePanel->SetSizer(validateSizer);
		this->minx = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->miny = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->maxx = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->maxy = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->speed = new wxSpinCtrl(validatePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 4000, 4000);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "Coordinate validation:"));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, ""));
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "min x:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(minx, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "min y:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(miny, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "max x:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(maxx, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "max y:"), 0, wxRIGHT | wxALIGN_RIGHT, 5);
		validateSizer->Add(maxy, 0, wxEXPAND);
		validateSizer->Add(new wxStaticText(validatePanel, wxID_ANY, "Maximum speed"));
		validateSizer->Add(speed, 0, wxEXPAND);
		
		logActions->SetValue(ctrl->getPlaneLog()->isLoggingActions());
		logErrors->SetValue(ctrl->getPlaneLog()->isLoggingErrors());
		xOffset->SetValue(ctrl->getPlaneMap()->getOffset().x);
		yOffset->SetValue(ctrl->getPlaneMap()->getOffset().y);
		xScale->SetValue(std::to_string((float) ctrl->getPlaneMap()->getScale().x));
		yScale->SetValue(std::to_string((float) ctrl->getPlaneMap()->getScale().y));
		minx->SetValue(ctrl->getPlaneValidator()->getMinimum().x);
		miny->SetValue(ctrl->getPlaneValidator()->getMinimum().y);
		maxx->SetValue(ctrl->getPlaneValidator()->getMaximum().x);
		maxy->SetValue(ctrl->getPlaneValidator()->getMaximum().y);
		speed->SetValue(ctrl->getPlaneValidator()->getMaxSpeed());
		
		wxButton *updateButton = new wxButton(this, wxID_ANY, "Update filters");
		sizer->Add(updateButton);
		updateButton->Bind(wxEVT_BUTTON, &CalxCoordCtrl::OnUpdateFiltersClick, ctrl);
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
	
	class CalxCoordGraphAction : public CalxAction {
		public:
			CalxCoordGraphAction(CoordHandle *handle, CoordTranslator *trans, GraphBuilder *builder, float speed) {
				this->handle = handle;
				this->translator = trans;
				this->builder = builder;
				this->speed = speed;
			}
			
			virtual ~CalxCoordGraphAction() {
				delete this->translator;
				delete this->builder;
			}
			
			virtual void perform(SystemManager *sysman) {
				builder->build(sysman, handle, translator, speed);
			}
			
		private:
			CoordHandle *handle;
			CoordTranslator *translator;
			GraphBuilder *builder;
			float speed;
	};
	
	class CalxCoordCalibrateAction : public CalxAction {
		public:
			CalxCoordCalibrateAction(CoordHandle *handle, TrailerId tr) {
				this->handle = handle;
				this->trailer = tr;
			}
			
			virtual void perform(SystemManager *sysman) {
				handle->calibrate(trailer);
			}
		private:
			CoordHandle *handle;
			TrailerId trailer;
	};
	
	CalxCoordCtrl::CalxCoordCtrl(wxWindow *win, wxWindowID id, CoordHandle *ctrl)
		: wxScrolledWindow::wxScrolledWindow(win, id) {
		this->ctrl = ctrl;
		
		motor_point_t mapOffset = {0, 0};
		motor_scale_t mapScale = {1.0f, 1.0f};
		this->map = new CoordPlaneMap(mapOffset, mapScale, ctrl->peekPlane());
		ctrl->pushPlane(this->map);
		motor_point_t validateMin = {INT_MIN, INT_MIN};
		motor_point_t validateMax = {INT_MAX, INT_MAX};
		this->validator = new CoordPlaneValidator(validateMin, validateMax, 4000, ctrl->peekPlane());
		ctrl->pushPlane(this->validator);
		this->log = new CoordPlaneLog(ctrl->peekPlane(), wxGetApp().getMainFrame()->getConsole(), "Plane #" + std::to_string(ctrl->getID()) + ": ");
		ctrl->pushPlane(this->log);
		
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
		sizer->Add(generalPanel, 0, wxALL | wxEXPAND, 0);
		
		wxPanel *actionPanel = new wxPanel(this, wxID_ANY);
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
		actionSizer->Add(actionSub2Panel, 0, wxLEFT | wxEXPAND, 5);
		wxStaticBox *otherBox = new wxStaticBox(actionSub2Panel, wxID_ANY, "Other");
		wxStaticBoxSizer *otherSizer = new wxStaticBoxSizer(otherBox, wxVERTICAL);
		actionSub2Panel->SetSizer(otherSizer);
		this->otherCtrl = new CalxCoordOtherCtrl(this, actionSub2Panel, wxID_ANY);
		otherSizer->Add(otherCtrl, 0, wxALL | wxEXPAND);
		
		sizer->Add(actionPanel, 0, wxALL | wxEXPAND, 0);
		Bind(wxEVT_COMMAND_QUEUE_UPDATE, &CalxCoordCtrl::OnQueueUpdate, this);
		Bind(wxEVT_CLOSE_WINDOW, &CalxCoordCtrl::OnExit, this);
		updateUI();
		
		this->Layout();
		this->FitInside();
        this->SetScrollRate(5, 5);
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
	
	void CalxCoordCtrl::OnGraphBuildClick(wxCommandEvent &evt) {
		std::stringstream ss(graphCtrl->getExpression());
		FunctionLexer lexer(ss);
		FunctionParser parser(&lexer);
		Node *node = parser.parse();
		motor_point_t toffset = {graphCtrl->getXOffset(), graphCtrl->getYOffset()};
		motor_size_t tsize = {graphCtrl->getXScale(), graphCtrl->getYScale()};
		long double minx = graphCtrl->getXMin();
		long double maxx = graphCtrl->getXMax();
		long double miny = graphCtrl->getYMin();
		long double maxy = graphCtrl->getYMax();
		long double step = graphCtrl->getStep();
		float speed = graphCtrl->getSpeed();
		BasicCoordTranslator *trans = new BasicCoordTranslator(toffset, tsize);
		coord_point_t min = {minx, miny};
		coord_point_t max = {maxx, maxy};
		GraphBuilder *graph = new GraphBuilder(node, min, max, step);
		this->queue->addAction(new CalxCoordGraphAction(ctrl, trans, graph, speed));
	}
	
	void CalxCoordCtrl::OnCalibrateClick(wxCommandEvent &evt) {
		TrailerId tr = otherCtrl->getTrailer();
		this->queue->addAction(new CalxCoordCalibrateAction(ctrl, tr));
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
		this->queue->stop();
		this->queue->Kill();
		wxGetApp().getSystemManager()->removeCoord(ctrl->getID());
	}
	
	void CalxCoordCtrl::OnQueueUpdate(wxThreadEvent &evt) {
		
	}
}