/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_UI_TASK_CALX_PROGRAMMED_TASK_H_
#define CALX_UI_TASK_CALX_PROGRAMMED_TASK_H_

#include "ui/task/CalxProgrammedTaskStep.h"
#include "ui/task/CalxTaskFactory.h"

namespace CalXUI {

	class CalxProgrammedTaskFactory : public CalxTaskFactory {
	 public:
		virtual CalxTaskHandle *newTask(wxWindow *);

	 private:
		size_t next_id = 0;
	};

	class CalxProgrammedTaskHandle : public CalxTaskHandle {
	 public:
		CalxProgrammedTaskHandle(wxWindow *, wxWindowID, size_t);
		virtual std::string getName();
		virtual CoordTask *getTask();
		virtual std::shared_ptr<CoordTranslator> getTranslator();
		virtual void update();
		void updateUI();

	 private:
		void OnListClick(wxCommandEvent &);
		void OnMoveAddClick(wxCommandEvent &);
		void OnJumpAddClick(wxCommandEvent &);
		void OnArcAddClick(wxCommandEvent &);
		void OnMoveUpClick(wxCommandEvent &);
		void OnMoveDownClick(wxCommandEvent &);
		void OnRemoveClick(wxCommandEvent &);
		void OnExit(wxCloseEvent &);

		ProgrammedCoordTask *task;
		size_t id;
		std::vector<CalxTaskStepHandle *> steps;
		wxPanel *mainPanel;
		wxListBox *stepList;
		wxButton *moveUpButton;
		wxButton *moveDownButton;
		std::shared_ptr<CoordTranslator> trans;
	};
}

#endif