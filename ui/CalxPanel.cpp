#include "CalxPanel.h"

namespace CalX {
	
	CalxPanel::CalxPanel(wxWindow *win, wxWindowID id)
		: wxNotebook::wxNotebook(win, id) {
		
		this->dev = new CalxDevicePanel(this, wxID_ANY);
		this->AddPage(this->dev, "Devices");
		this->coord = new CalxCoordPanel(this, wxID_ANY);
		this->AddPage(this->coord, "Coordinate planes");
		this->task = new CalxTaskPanel(this, wxID_ANY);
		this->AddPage(this->task, "Tasks");
	}
	
	void CalxPanel::updateUI() {
		this->coord->updateUI();
		this->task->updateUI();
	}
}