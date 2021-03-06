/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_UI_TASK_CALX_PROGRAMMED_TASK_H_
#define CALX_UI_TASK_CALX_PROGRAMMED_TASK_H_

#include "calx/ui/task/CalxProgrammedTaskStep.h"
#include "calx/ui/task/CalxTaskFactory.h"

namespace CalX::UI {

	class CalxProgrammedTaskFactory : public CalxTaskFactory {
	 public:
		CalxTaskHandle *newTask(wxWindow *) override;

	 private:
		std::size_t next_id = 0;
	};

	class CalxProgrammedTaskHandle : public CalxTaskHandle {
	 public:
		CalxProgrammedTaskHandle(wxWindow *, wxWindowID, std::size_t);
		std::string getName() const override;
		std::shared_ptr<CoordTask> getTask() override;
		std::shared_ptr<CoordTranslator> getTranslator() override;
		void update() override;
		void updateUI();

	 protected:
		void dump(std::ostream &) const override;

	 private:
		void OnListClick(wxCommandEvent &);
		void OnMoveAddClick(wxCommandEvent &);
		void OnJumpAddClick(wxCommandEvent &);
		void OnArcAddClick(wxCommandEvent &);
		void OnMoveUpClick(wxCommandEvent &);
		void OnMoveDownClick(wxCommandEvent &);
		void OnRemoveClick(wxCommandEvent &);
		void OnExit(wxCloseEvent &);

		std::shared_ptr<ProgrammedCoordTask> task;
		std::size_t id;
		std::vector<CalxTaskStepHandle *> steps;
		wxPanel *mainPanel;
		wxListBox *stepList;
		wxButton *moveUpButton;
		wxButton *moveDownButton;
		std::shared_ptr<CoordTranslator> trans;
	};
}  // namespace CalX::UI

#endif