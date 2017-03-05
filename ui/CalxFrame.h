#ifndef _8SMC1_UI_CALX_FRAME_H_
#define _8SMC1_UI_CALX_FRAME_H_

#include "calx.h"
#include <string>
#include <iostream>

namespace CalX {
	
	class CalxPanel; // Forward referencing
	
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