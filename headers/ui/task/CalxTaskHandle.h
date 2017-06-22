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


#ifndef CALX_UI_CALX_TASK_HANDLE_H_
#define CALX_UI_CALX_TASK_HANDLE_H_

#include <iostream>
#include <vector>
#include "ui/CalxApp.h"
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include "ui/coord/CalxCoordFilter.h"

using namespace CalX;

namespace CalXUI {

	enum class CalxTaskType {
		CalxGcode, CalxProgrammed, CalxLinear
	};

	class CalxTaskHandle : public wxScrolledWindow {
		public:
			CalxTaskHandle(wxWindow *win, wxWindowID id, CalxTaskType type)
				: wxScrolledWindow::wxScrolledWindow(win, id) {
				this->SetScrollRate(5, 5);
				this->type = type;
			}
			CalxTaskType getType() {return this->type;}
			virtual CoordTask *getTask() = 0;
			virtual ComplexCoordTranslator *getTranslator() = 0;
			virtual void update() = 0;
		private:
			CalxTaskType type;
	};

	class CalxGcodeHandle : public CalxTaskHandle {
		public:
			CalxGcodeHandle(wxWindow*, wxWindowID, std::string, std::istream*, ComplexCoordTranslator*);
			virtual CoordTask *getTask();
			virtual void update();
			std::string getId() {
				return this->id;
			}
			virtual ComplexCoordTranslator *getTranslator() {
				return this->translator;
			}
		private:
			void OnExit(wxCloseEvent&);

			std::string id;
			CalxCoordFilterCtrl *filter;
			ComplexCoordTranslator *translator;


			CoordTask *task;
	};

	class CalxTaskStepHandle : public wxPanel {
		public:
			CalxTaskStepHandle(wxWindow *win, wxWindowID id)
				: wxPanel::wxPanel(win, id) {}
			virtual void update() = 0;
	};

	class CalxTaskLinearStepHandle : public CalxTaskStepHandle {
		public:
			CalxTaskLinearStepHandle(wxWindow*, wxWindowID);
			MoveTaskStep *getTaskStep();
			virtual void update();
		private:
			void OnFieldChange(wxCommandEvent&);

			MoveTaskStep *step;

			wxSpinCtrlDouble *xCoord;
			wxSpinCtrlDouble *yCoord;
			wxSpinCtrlDouble *speed;
			wxCheckBox *relative;
	};

	class CalxTaskLinearJumpStepHandle : public CalxTaskStepHandle {
		public:
			CalxTaskLinearJumpStepHandle(wxWindow*, wxWindowID);
			JumpTaskStep *getTaskStep();
			virtual void update();
		private:
			void OnFieldChange(wxCommandEvent&);

			JumpTaskStep *step;

			wxSpinCtrlDouble *xCoord;
			wxSpinCtrlDouble *yCoord;
			wxSpinCtrlDouble *speed;
			wxCheckBox *relative;
	};

	class CalxTaskArcStepHandle : public CalxTaskStepHandle {
		public:
			CalxTaskArcStepHandle(wxWindow*, wxWindowID);
			ArcTaskStep *getTaskStep();
			virtual void update();
		private:
			void OnFieldChange(wxCommandEvent&);

			ArcTaskStep *step;

			wxSpinCtrlDouble *destXCoord;
			wxSpinCtrlDouble *destYCoord;
			wxSpinCtrlDouble *cenXCoord;
			wxSpinCtrlDouble *cenYCoord;
			wxSpinCtrlDouble *speed;
			wxSpinCtrl *splitter;
			wxCheckBox *clockwise;
			wxCheckBox *relative;
	};

	class CalxProgrammedTaskHandle : public CalxTaskHandle {
		public:
			CalxProgrammedTaskHandle(wxWindow*, wxWindowID);
			virtual CoordTask *getTask();
			virtual ComplexCoordTranslator *getTranslator();
			virtual void update();
			void updateUI();
		private:
			void OnListClick(wxCommandEvent&);
			void OnMoveAddClick(wxCommandEvent&);
			void OnJumpAddClick(wxCommandEvent&);
			void OnArcAddClick(wxCommandEvent&);
			void OnMoveUpClick(wxCommandEvent&);
			void OnMoveDownClick(wxCommandEvent&);
			void OnRemoveClick(wxCommandEvent&);
			void OnExit(wxCloseEvent&);

			ProgrammedCoordTask *task;
			std::vector<CalxTaskStepHandle*> steps;
			wxPanel *mainPanel;
			wxListBox *stepList;
			wxButton *moveUpButton;
			wxButton *moveDownButton;
			ComplexCoordTranslator *trans;
	};

	class CalxLinearTaskHandle : public CalxTaskHandle {
		public:
			CalxLinearTaskHandle(wxWindow*, wxWindowID);
			virtual CoordTask *getTask();
			virtual ComplexCoordTranslator *getTranslator();
			virtual void update();
			void setRectangle(coord_rect_t);
		private:
			void OnFieldChange(wxCommandEvent&);
			void OnExit(wxCloseEvent&);
			LinearCoordTask *task;
			ComplexCoordTranslator *trans;
			wxSpinCtrlDouble *xCoord;
			wxSpinCtrlDouble *yCoord;
			wxSpinCtrlDouble *wDim;
			wxSpinCtrlDouble *hDim;
			wxSpinCtrlDouble *spacing;
			wxCheckBox *vertical;
	};
}

#endif
