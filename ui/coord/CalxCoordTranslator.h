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