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

#ifndef CALX_UI_TASK_CALX_PROGRAMMED_TASK_STEP_H_
#define CALX_UI_TASK_CALX_PROGRAMMED_TASK_STEP_H_

#include "calx/ui/CalxApp.h"
#include <wx/spinctrl.h>

namespace CalX::UI {

	class CalxTaskStepHandle : public wxPanel {
	 public:
		CalxTaskStepHandle(wxWindow *win, wxWindowID id)
		    : wxPanel::wxPanel(win, id) {}
		virtual void update() = 0;
	};

	class CalxTaskLinearStepHandle : public CalxTaskStepHandle {
	 public:
		CalxTaskLinearStepHandle(wxWindow *, wxWindowID);
		std::shared_ptr<MoveTaskStep> getTaskStep();
		void update() override;

	 private:
		void OnFieldChange(wxCommandEvent &);

		std::shared_ptr<MoveTaskStep> step;

		wxSpinCtrlDouble *xCoord;
		wxSpinCtrlDouble *yCoord;
		wxSpinCtrlDouble *speed;
		wxCheckBox *relative;
	};

	class CalxTaskLinearJumpStepHandle : public CalxTaskStepHandle {
	 public:
		CalxTaskLinearJumpStepHandle(wxWindow *, wxWindowID);
		std::shared_ptr<JumpTaskStep> getTaskStep();
		void update() override;

	 private:
		void OnFieldChange(wxCommandEvent &);

		std::shared_ptr<JumpTaskStep> step;

		wxSpinCtrlDouble *xCoord;
		wxSpinCtrlDouble *yCoord;
		wxSpinCtrlDouble *speed;
		wxCheckBox *relative;
	};

	class CalxTaskArcStepHandle : public CalxTaskStepHandle {
	 public:
		CalxTaskArcStepHandle(wxWindow *, wxWindowID);
		std::shared_ptr<ArcTaskStep> getTaskStep();
		void update() override;

	 private:
		void OnFieldChange(wxCommandEvent &);

		std::shared_ptr<ArcTaskStep> step;

		wxSpinCtrlDouble *destXCoord;
		wxSpinCtrlDouble *destYCoord;
		wxSpinCtrlDouble *cenXCoord;
		wxSpinCtrlDouble *cenYCoord;
		wxSpinCtrlDouble *speed;
		wxSpinCtrl *splitter;
		wxCheckBox *clockwise;
		wxCheckBox *relative;
	};
}  // namespace CalX::UI

#endif
