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

#ifndef CALX_UI_COORD_CALX_COORD_COMPONENT_H_
#define CALX_UI_COORD_CALX_COORD_COMPONENT_H_

#include "ui/CalxApp.h"
#include "ui/coord/CalxCoordController.h"

namespace CalXUI {

	class CalxCoordComponent : public wxPanel {
	 public:
		CalxCoordComponent(wxWindow *win, wxWindowID id)
		    : wxPanel::wxPanel(win, id) {}
		virtual void setEnabled(bool en) {
			Enable(en);
		}
	};

	class CalxCoordComponentFactory {
	 public:
		virtual ~CalxCoordComponentFactory() = default;
		virtual CalxCoordComponent *newComponent(wxWindow *,
		                                         CalxCoordController *) = 0;
	};
}  // namespace CalXUI

#endif