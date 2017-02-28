#include "ConsoleWidget.h"

namespace _8SMC1UI {
	
	ConsoleWidget::ConsoleWidget(wxWindow *root, _8SMC1App *app)
		: wxTextCtrl::wxTextCtrl(root, wxID_ANY) {
		this->app = app;
	}
	
	ConsoleWidget::~ConsoleWidget() {
		
	}
	
}