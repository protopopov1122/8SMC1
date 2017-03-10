#ifndef CALX_UI_CALX_COORD_DIALOG_H_
#define CALX_UI_CALX_COORD_DIALOG_H_

#include "calx.h"
#include <wx/choice.h>

using namespace CalX;

namespace CalXUI {
	
	class CalxCoordDialog : public wxDialog {
		public:
			CalxCoordDialog(wxWindow*, wxWindowID, SystemManager*);
			CoordHandle *getCoordPlane();
		private:
			void OnOkButtonClick(wxCommandEvent&);
			void OnCancelButtonClick(wxCommandEvent&);
		
			wxChoice *xChoice;
			wxChoice *yChoice;
		
			SystemManager *sysman;
			CoordHandle *ctrl;
	};
}

#endif