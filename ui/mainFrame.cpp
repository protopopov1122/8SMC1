#include "mainFrame.h"

namespace _8SMC1::UI {
	
	
	_8SMC1MainFrame::_8SMC1MainFrame(std::string title)
		: wxFrame::wxFrame(NULL, wxID_ANY, wxString(title)) {
		this->devman = new DeviceManager();
		this->sysman = new SystemManager(this->devman);
	}
	
	_8SMC1MainFrame::~_8SMC1MainFrame() {
		delete this->sysman;
		delete this->devman;
	}
	
}