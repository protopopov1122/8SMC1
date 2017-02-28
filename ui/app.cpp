#include "app.h"
#include "mainFrame.h"

namespace _8SMC1UI {

	_8SMC1App::_8SMC1App() {
		this->devman = new _8SMC1::DeviceManager();
		this->sysman = new _8SMC1::SystemManager(this->devman);
	}
	
	_8SMC1App::~_8SMC1App() {
		delete this->sysman;
		delete this->devman;
	}

	bool _8SMC1App::OnInit() {
		_8SMC1MainFrame *frame = new _8SMC1MainFrame("8SMC1 System", this);
		frame->Show(true);
		return true;
	}
}


wxIMPLEMENT_APP(_8SMC1UI::_8SMC1App);