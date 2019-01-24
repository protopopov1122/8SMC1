/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ui/task/CalxGCodeTask.h"
#include "ui/task/CalxGcodeLoader.h"
#include <iostream>

namespace CalXUI {

	CalxGcodeHandle::CalxGcodeHandle(wxWindow *win, wxWindowID id,
	                                 const std::string &sid, std::istream *is,
	                                 std::shared_ptr<ComplexCoordTranslator> tr)
	    : CalxTaskHandle::CalxTaskHandle(win, id) {
		this->id = sid;
		this->translator = tr;

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		wxTextCtrl *codeText =
		    new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
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
		this->task = std::make_shared<GCodeCoordTask>(ss, tr);

		Layout();
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxGcodeHandle::OnExit, this);
	}

	std::string CalxGcodeHandle::getName() const {
		return this->id;
	}

	std::shared_ptr<CoordTask> CalxGcodeHandle::getTask() {
		return this->task;
	}

	void CalxGcodeHandle::OnExit(wxCloseEvent &evt) {
		Destroy();
	}

	void CalxGcodeHandle::update() {
		this->filter->getTranslator();
	}

	std::shared_ptr<CoordTranslator> CalxGcodeHandle::getTranslator() {
		return this->translator;
	}

	void CalxGcodeHandle::dump(std::ostream &out) const {
		out << "GCode task \'" << this->getName() << '\'';
	}

	CalxTaskHandle *CalxGCodeTaskFactory::newTask(wxWindow *win) {
		CalxGcodeHandle *handle = nullptr;
		CalxGcodeLoader *loader = new CalxGcodeLoader(win, wxID_ANY);
		loader->ShowModal();
		if (loader->isLoaded()) {
			std::fstream is(loader->getPath());
			handle = new CalxGcodeHandle(win, wxID_ANY, loader->getPath(), &is,
			                             loader->getTranslator());
			is.close();
		}
		loader->Destroy();
		return handle;
	}
}  // namespace CalXUI