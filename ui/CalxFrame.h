#ifndef _8SMC1_UI_CALX_FRAME_H_
#define _8SMC1_UI_CALX_FRAME_H_

#include "calx.h"
#include <string>
#include <iostream>

namespace CalX {
	
	class CalxFrame : public wxFrame {
		public:
			CalxFrame(std::string);
			std::ostream *getConsole();
			
		private:
			void OnClose(wxCloseEvent&);
			
			std::ostream *console;
	};
}


#endif