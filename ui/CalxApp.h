#ifndef _8SMC1_UI_CALX_APP_H_
#define _8SMC1_UI_CALX_APP_H_

#include "calx.h"

using namespace _8SMC1;

namespace CalX {
	
	class CalxFrame; // Forward referencing
	
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
wxDECLARE_APP(CalX::CalxApp);


#endif