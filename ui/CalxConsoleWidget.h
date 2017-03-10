#ifndef CALX_UI_CALX_CONSOLE_WIDGET_H_
#define CALX_UI_CALX_CONSOLE_WIDGET_H_

#include <wx/textctrl.h>
#include "calx.h"


namespace CalXUI {
	
	class CalxConsoleWidget : public wxTextCtrl {
		public:
			CalxConsoleWidget(wxWindow*, wxWindowID);
	};
}

#endif