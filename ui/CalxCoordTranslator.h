#ifndef _8SMC1_UI_CALX_COORD_TRANSLATOR_H_
#define _8SMC1_UI_CALX_COORD_TRANSLATOR_H_

#include "CalxApp.h"
#include "ctrl-lib/SystemManager.h"
#include <wx/spinctrl.h>

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