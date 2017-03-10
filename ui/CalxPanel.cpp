#include "CalxPanel.h"
#include "dev/CalxDevicePanel.h"
#include "coord/CalxCoordPanel.h"
#include "task/CalxTaskPanel.h"

namespace CalXUI {
	
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