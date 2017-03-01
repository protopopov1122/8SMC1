#include "CalxConsoleWidget.h"

namespace CalX {
	
	CalxConsoleWidget::CalxConsoleWidget(wxWindow *win, wxWindowID id) :
		wxTextCtrl::wxTextCtrl(win, id, "", wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY) {
			
	}
}