/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <sstream>
#include "ui/task/CalxTaskHandle.h"
#include <wx/sizer.h>
#include <wx/textctrl.h>

namespace CalXUI {

	CalxGcodeHandle::CalxGcodeHandle(wxWindow *win, wxWindowID id, std::string sid, std::istream *is, ComplexCoordTranslator *tr)
		: CalxTaskHandle::CalxTaskHandle(win, id, CalxTaskType::CalxGcode) {
		this->id = sid;
		this->translator = tr;

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		wxTextCtrl *codeText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY);
		sizer->Add(codeText, 1, wxALL | wxEXPAND, 0);
		this->filter = new CalxCoordFilterCtrl(this, wxID_ANY, tr);
		sizer->Add(filter, 0, wxALL, 0);


		std::string code;
		int chr;
		while ((chr = is->get()) != EOF) {
            code += (char) chr;
		}
		codeText->SetValue(code);

		std::stringstream ss(code);
		this->task = new GCodeCoordTask(&ss, tr);

		Layout();
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxGcodeHandle::OnExit, this);
	}

	CoordTask *CalxGcodeHandle::getTask() {
		return this->task;
	}

	void CalxGcodeHandle::OnExit(wxCloseEvent &evt) {
		delete this->translator;
		Destroy();
	}

	void CalxGcodeHandle::update() {
		this->filter->getTranslator();
	}
}
