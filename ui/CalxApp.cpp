#include "CalxApp.h"
#include "CalxFrame.h"

namespace CalX {
	
	bool CalxApp::OnInit() {
		this->devman = new DeviceManager();
		this->sysman = new SystemManager(this->devman);
		this->frame = new CalxFrame("8SMC1 CalX");
		this->frame->Show(true);
		this->frame->Maximize(true);
		
		return true;
	}
	
	int CalxApp::OnExit() {
		delete this->sysman;
		delete this->devman;
		return 0;
	}
	
	SystemManager *CalxApp::getSystemManager() {
		return this->sysman;
	}
	
	CalxFrame *CalxApp::getMainFrame() {
		return this->frame;
	}
}

wxIMPLEMENT_APP(CalX::CalxApp);