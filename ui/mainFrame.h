#ifndef _8SMC1_UI_MAIN_FRAME_H_
#define _8SMC1_UI_MAIN_FRAME_H_

#include "base.h"
#include "ctrl-lib/SystemManager.h"
#include <string>

namespace _8SMC1::UI {
	
	class _8SMC1MainFrame : public wxFrame {
		public:
			_8SMC1MainFrame(std::string);
			virtual ~_8SMC1MainFrame();
		private:
			SystemManager *sysman;
			DeviceManager *devman;
	};
}


#endif