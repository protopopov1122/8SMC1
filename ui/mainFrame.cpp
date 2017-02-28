#include "app.h"
#include "mainFrame.h"
#include "ConsoleWidget.h"

namespace _8SMC1UI {
	
	
	_8SMC1MainFrame::_8SMC1MainFrame(std::string title, _8SMC1App *app)
		: wxFrame::wxFrame(NULL, wxID_ANY, wxString(title)) {
		this->app = app;
		initFrame();
	}
	
	_8SMC1MainFrame::~_8SMC1MainFrame() {
	}
	
	void _8SMC1MainFrame::initFrame() {
		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);

		wxBoxSizer *mainPanel_sizer = new wxBoxSizer(wxVERTICAL);
		wxPanel *tabbedPanel = new wxPanel(mainPanel);
		wxPanel *consolePanel = new wxPanel(mainPanel);
		mainPanel_sizer->Add(tabbedPanel, 1, wxEXPAND | wxALL, 5);
		mainPanel_sizer->Add(consolePanel, 1, wxEXPAND | wxALL, 5);
		mainPanel->SetSizer(mainPanel_sizer);
		
		wxScrolledWindow *scrolledPanel = new wxScrolledWindow(consolePanel,  wxID_ANY, wxDefaultPosition, 			wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS | wxVSCROLL | wxHSCROLL);
		
		ConsoleWidget *w1 = new ConsoleWidget(tabbedPanel, app);
		ConsoleWidget *w2 = new ConsoleWidget(scrolledPanel, app);
	}
	
}