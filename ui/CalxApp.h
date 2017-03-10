#ifndef CALX_UI_CALX_APP_H_
#define CALX_UI_CALX_APP_H_

#include "calx.h"
#include "CalxFrame.h"

using namespace CalX;

namespace CalXUI {
	
	class CalxApp : public wxApp {
		public:
			virtual bool OnInit();
			virtual int OnExit();
			SystemManager *getSystemManager();
			CalxFrame *getMainFrame();
		private:
			CalxFrame *frame;
			DeviceManager *devman;
			SystemManager *sysman;
	};
	
}
wxDECLARE_APP(CalXUI::CalxApp);


#endif