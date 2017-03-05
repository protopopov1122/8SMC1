#include <wx/splitter.h>
#include <wx/textctrl.h>
#include "CalxFrame.h"
#include "CalxPanel.h"
#include "CalxConsoleWidget.h"

namespace CalX {
	CalxFrame::CalxFrame(std::string title)
		: wxFrame::wxFrame(nullptr, wxID_ANY, title) {
		
		this->CreateStatusBar(1);
		this->SetStatusText("8SMC1 CalX", 0);
		
		wxSplitterWindow *mainPanel = new wxSplitterWindow(this, wxID_ANY);
		
		this->panel = new CalxPanel(mainPanel, wxID_ANY);
		CalxConsoleWidget *console = new CalxConsoleWidget(mainPanel, wxID_ANY);
		panel->Show(true);
		console->Show(true);
		mainPanel->Initialize(panel);
		mainPanel->SplitHorizontally(panel, console);
		mainPanel->SetSashGravity(1.0f);	
		
		this->console = new std::ostream(console);
		
		Bind(wxEVT_CLOSE_WINDOW, &CalxFrame::OnClose, this);
		Layout();
		Fit();
	}
	
	CalxPanel *CalxFrame::getPanel() {
		return this->panel;
	}
	
	std::ostream *CalxFrame::getConsole() {
		return this->console;
	}

	void CalxFrame::OnClose(wxCloseEvent &evt) {
		Destroy();
	}
}