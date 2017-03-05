#ifndef _8SMC1_UI_CALX_GCODE_LOADER_H_
#define _8SMC1_UI_CALX_GCODE_LOADER_H_

#include "CalxApp.h"
#include "coord/CalxCoordTranslator.h"
#include <string>
#include <wx/textctrl.h>

namespace CalX {
	
	class CalxGcodeLoader : public wxDialog {
		public:
			CalxGcodeLoader(wxWindow*, wxWindowID);
			bool isLoaded();
			std::string getPath();
			CoordTranslator *getTranslator();
		private:
			void OnOkClick(wxCommandEvent&);
			void OnCancelClick(wxCommandEvent&);
			void OnChooseClick(wxCommandEvent&);
			
			wxTextCtrl *gcodePath;			
			CalxCoordTranslator *translator;
			bool state;
	};
}

#endif