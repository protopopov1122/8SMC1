#include "app.h"
#include "mainFrame.h"

namespace _8SMC1::UI {

	bool _8SMC1App::OnInit() {
		_8SMC1MainFrame *frame = new _8SMC1MainFrame("8SMC1 System");
		frame->Show(true);
		return true;
	}
}


wxIMPLEMENT_APP(_8SMC1::UI::_8SMC1App);