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


#ifndef CALX_UI_COORD_CALX_COORD_ADJUSTER_H_
#define CALX_UI_COORD_CALX_COORD_ADJUSTER_H_


#include "ui/CalxApp.h"
#include "ui/CalxActionQueue.h"
#include <wx/spinctrl.h>
#include <wx/stattext.h>

namespace CalXUI {
	
	class CalxCoordAdjustDialog; // Forward reference
	
	enum class CalxAdjustDirection {
		Up, Down, Left, Right
	};
	
	class CalxCoordAdjustAction : public CalxAction {
		public:
			CalxCoordAdjustAction(CalxCoordAdjustDialog*, CoordHandle*, CalxAdjustDirection, double, double);
			void perform(SystemManager*);
			void stop();
		private:
			CalxCoordAdjustDialog *dialog;
			CoordHandle *handle;
			CalxAdjustDirection direction;
			double step;
			double speed;
	};
	
	class CalxCoordAdjustMoveAction : public CalxAction {
		public:
			CalxCoordAdjustMoveAction(CalxCoordAdjustDialog*, CoordHandle*, coord_point_t, double);
			void perform(SystemManager*);
			void stop();
		private:
			CalxCoordAdjustDialog *dialog;
			CoordHandle *handle;
			coord_point_t dest;
			double speed;
	};

	class CalxCoordAdjustViewer : public wxWindow {
		public:
			CalxCoordAdjustViewer(wxWindow*, wxWindowID, CoordHandle*, wxSize);
		private:
			void OnPaintEvent(wxPaintEvent&);
			void OnResizeEvent(wxSizeEvent&);
			void render(wxDC&);
			
			CoordHandle *handle;
			wxColour pointer_colour;
	};

	class CalxCoordAdjustRepaintTimer : public wxTimer {
		public:
			CalxCoordAdjustRepaintTimer(CalxCoordAdjustDialog*);
			virtual void Notify();
		private:
			CalxCoordAdjustDialog *dialog;
	};

	class CalxCoordAdjustDialog : public wxDialog {
		public:
			CalxCoordAdjustDialog(wxWindow*, wxWindowID, CoordHandle*);
			
			void adjust(CalxAdjustDirection);
			void setEnabled(bool);
			void updateUI();
		private:
			void OnOkClick(wxCommandEvent&);
			void OnStopClick(wxCommandEvent&);
			void OnLeftClick(wxCommandEvent&);
			void OnRightClick(wxCommandEvent&);
			void OnUpClick(wxCommandEvent&);
			void OnDownClick(wxCommandEvent&);
			void OnMouseMove(wxMouseEvent&);
			void OnMouseClick(wxMouseEvent&);
			void OnExit(wxCloseEvent&);
		
			wxStaticText *mouseCoords;
			wxStaticText *position;
			wxPanel *prmsPanel;
			wxPanel *buttonPanel;
			wxButton *stopButton;
			wxSpinCtrlDouble *stepSpin;
			wxSpinCtrlDouble *speedSpin;
		
			CoordHandle *handle;
			CalxActionQueue *queue;
			CalxCoordAdjustRepaintTimer *timer;
			CalxCoordAdjustViewer *viewer;
	};
}


#endif