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


#ifndef CALX_UI_CALX_COORD_TRANSLATOR_H_
#define CALX_UI_CALX_COORD_TRANSLATOR_H_

#include <wx/spinctrl.h>
#include "CalxApp.h"

namespace CalXUI {
	
	class CalxCoordTranslator : public wxPanel {
		public:
			CalxCoordTranslator(wxWindow*, wxWindowID);
			CoordTranslator *getTranslator();
		private:
			void updateData();
			BasicCoordTranslator *translator;
			wxSpinCtrl *xoffset;
			wxSpinCtrl *yoffset;
			wxSpinCtrl *xscale;
			wxSpinCtrl *yscale;
	};
}

#endif