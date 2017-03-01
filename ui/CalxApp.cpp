#include "CalxApp.h"

namespace CalX {
	
	bool CalxApp::OnInit() {
		this->devman = new DeviceManager();
		this->sysman = new SystemManager(this->devman);
		this->frame = new CalxFrame("8SMC1 CalX");
		this->frame->Show(true);
		
		*this->frame->getConsole() << "Hello, world!\n";
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
}

wxIMPLEMENT_APP(CalX::CalxApp);