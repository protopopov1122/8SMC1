#include <sstream>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include "CalxTaskHandle.h"
#include "ctrl-lib/misc/GCodeParser.h"

namespace CalXUI {
	
	CalxGcodeHandle::CalxGcodeHandle(wxWindow *win, wxWindowID id, std::string sid, std::istream *is, CoordTranslator *tr)
		: CalxTaskHandle::CalxTaskHandle(win, id, CalxTaskType::CalxGcode) {
		this->id = sid;
		this->translator = tr;
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		wxTextCtrl *codeText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY);
		sizer->Add(codeText, 1, wxALL | wxEXPAND, 0);
		
		std::string code;
		int chr;
		while ((chr = is->get()) != EOF) {
			code += chr;
		}
		codeText->SetValue(code);
		
		std::stringstream ss(code);
		GCodeLexer lex(&ss);
		GCodeParser parser(&lex);
		this->task = gcode_translate(&parser, this->translator, wxGetApp().getSystemManager());
		
		Layout();
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxGcodeHandle::OnExit, this);
	}
	
	CoordTask *CalxGcodeHandle::getTask() {
		return this->task;
	}
	
	void CalxGcodeHandle::OnExit(wxCloseEvent &evt) {
		delete this->translator;
	}
}