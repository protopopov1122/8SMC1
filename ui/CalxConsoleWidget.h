#ifndef _8SMC1_UI_CALX_CONSOLE_WIDGET_H_
#define _8SMC1_UI_CALX_CONSOLE_WIDGET_H_

#include <wx/textctrl.h>
#include "calx.h"


namespace CalX {
	
	class CalxConsoleWidget : public wxTextCtrl {
		public:
			CalxConsoleWidget(wxWindow*, wxWindowID);
	};
}

#endif