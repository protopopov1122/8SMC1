#ifndef _8SMC1_UI_CONSOLE_WIDGET_H_
#define _8SMC1_UI_CONSOLE_WIDGET_H_

#include "base.h"
#include "app.h"

namespace _8SMC1UI {
	
	class ConsoleWidget : public wxTextCtrl {
		public:
			ConsoleWidget(wxWindow*, _8SMC1App*);
			virtual ~ConsoleWidget();
		private:
			_8SMC1App *app;
	};
}

#endif