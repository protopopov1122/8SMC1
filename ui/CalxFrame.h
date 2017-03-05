#ifndef _8SMC1_UI_CALX_FRAME_H_
#define _8SMC1_UI_CALX_FRAME_H_

#include <string>
#include <iostream>
#include "calx.h"
#include "CalxPanel.h"

namespace CalX {
	
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