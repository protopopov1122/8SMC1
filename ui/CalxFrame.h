#ifndef CALX_UI_CALX_FRAME_H_
#define CALX_UI_CALX_FRAME_H_

#include <string>
#include <iostream>
#include "calx.h"
#include "CalxPanel.h"

namespace CalXUI {
	
	class CalxFrame : public wxFrame {
		public:
			CalxFrame(std::string);
			std::ostream *getConsole();
			CalxPanel *getPanel();
		private:
			void OnClose(wxCloseEvent&);
			
			std::ostream *console;
			CalxPanel *panel;
	};
}


#endif