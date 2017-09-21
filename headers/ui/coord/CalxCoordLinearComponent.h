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

#ifndef CALX_UI_CALX_COORD_LINEAR_CTRL_H_
#define CALX_UI_CALX_COORD_LINEAR_CTRL_H_

#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/coord/CalxCoordComponent.h"
#include "ui/coord/CalxCoordController.h"
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

using namespace CalX;

namespace CalXUI {

	class CalxCoordLinearComponentFactory : public CalxCoordComponentFactory {
		virtual CalxCoordComponent *newComponent(wxWindow *, CalxCoordController *);
	};

	class CalxCoordLinearComponent : public CalxCoordComponent {
	 public:
		CalxCoordLinearComponent(wxWindow *, wxWindowID, CalxCoordController *);

	 private:
		void OnMoveClick(wxCommandEvent &);
		void OnJumpClick(wxCommandEvent &);

		wxSpinCtrlDouble *xCoord;
		wxSpinCtrlDouble *yCoord;
		wxSpinCtrlDouble *speed;
		wxCheckBox *relative;

		CalxCoordController *controller;
	};
}  // namespace CalXUI

#endif
