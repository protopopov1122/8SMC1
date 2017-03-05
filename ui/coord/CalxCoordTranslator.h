#ifndef _8SMC1_UI_CALX_COORD_TRANSLATOR_H_
#define _8SMC1_UI_CALX_COORD_TRANSLATOR_H_

#include <wx/spinctrl.h>
#include "CalxApp.h"

namespace CalX {
	
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