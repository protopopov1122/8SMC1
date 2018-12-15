/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#include <fstream>
#include "ui/script/CalxScriptPanel.h"
#include <wx/sizer.h>
#include <wx/splitter.h>

namespace CalXUI {

	wxDEFINE_EVENT(wxEVT_ENABLE_EVENT, wxThreadEvent);

	class CalxScriptActionWrapper : public CalxAction {
	 public:
		CalxScriptActionWrapper(CalxScriptPanel *panel,
		                        std::unique_ptr<CalxAction> action)
		    : panel(panel), action(std::move(action)) {}

		virtual void perform(SystemManager *sysman) {
			panel->setEnabled(false);
			action->perform(sysman);
			panel->setEnabled(true);
		}

		virtual void stop() {
			action->stop();
		}

	 private:
		CalxScriptPanel *panel;
		std::unique_ptr<CalxAction> action;
	};

	CalxScriptPanel::CalxScriptPanel(wxWindow *win, wxWindowID id,
	                                 CalXScriptEnvironment &env,
	                                 CalXScriptFactory &fact)
	    : CalxPanelPane::CalxPanelPane(win, id), env(env), scriptFactory(fact) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
		sizer->Add(splitter, 1, wxALL | wxEXPAND);

		wxPanel *scriptListPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *scriptListSizer = new wxBoxSizer(wxVERTICAL);
		scriptListPanel->SetSizer(scriptListSizer);
		this->scriptList = new wxListBox(scriptListPanel, wxID_ANY);
		scriptListSizer->Add(this->scriptList, 1, wxALL | wxEXPAND);
		this->scriptList->Bind(wxEVT_LISTBOX, &CalxScriptPanel::OnListClick, this);
		wxButton *loadButton =
		    new wxButton(scriptListPanel, wxID_ANY, __("Load script"));
		scriptListSizer->Add(loadButton, 0, wxALL | wxEXPAND);
		loadButton->Bind(wxEVT_BUTTON, &CalxScriptPanel::OnLoadScriptClick, this);
		wxButton *shellButton =
		    new wxButton(scriptListPanel, wxID_ANY, __("Open shell"));
		scriptListSizer->Add(shellButton, 0, wxALL | wxEXPAND);
		shellButton->Bind(wxEVT_BUTTON, &CalxScriptPanel::OnOpenShellClick, this);
		wxButton *removeButton =
		    new wxButton(scriptListPanel, wxID_ANY, __("Remove"));
		scriptListSizer->Add(removeButton, 0, wxALL | wxEXPAND);
		removeButton->Bind(wxEVT_BUTTON, &CalxScriptPanel::OnRemoveScriptClick,
		                   this);

		this->scriptPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *scriptSizer = new wxBoxSizer(wxVERTICAL);
		scriptPanel->SetSizer(scriptSizer);

		splitter->Initialize(scriptListPanel);
		splitter->SplitVertically(scriptListPanel, scriptPanel);
		splitter->SetSashGravity(0.1f);

		Layout();

		this->queue = new CalxActionQueue(wxGetApp().getSystemManager(), this);
		this->queue->Run();

		this->Bind(wxEVT_CLOSE_WINDOW, &CalxScriptPanel::OnExit, this);
		this->Bind(wxEVT_ENABLE_EVENT, &CalxScriptPanel::OnEnableEvent, this);
	}

	bool CalxScriptPanel::isBusy() {
		return this->queue->isBusy();
	}

	void CalxScriptPanel::shutdown() {
		this->queue->stop();
	}

	void CalxScriptPanel::updateUI() {
		for (const auto &t : this->scripts) {
			t->Show(false);
		}
		if (this->scriptList->GetSelection() != wxNOT_FOUND) {
			this->scripts.at((size_t) scriptList->GetSelection())->Show(true);
		}
		this->scriptPanel->Layout();
		this->Layout();
	}

	void CalxScriptPanel::setEnabled(bool en) {
		wxThreadEvent evt(wxEVT_ENABLE_EVENT);
		evt.SetPayload(en);
		wxPostEvent(this, evt);
	}

	void CalxScriptPanel::sendAction(std::unique_ptr<CalxAction> action) {
		this->queue->addAction(
		    std::make_unique<CalxScriptActionWrapper>(this, std::move(action)));
	}

	void CalxScriptPanel::OnListClick(wxCommandEvent &evt) {
		this->updateUI();
	}

	void CalxScriptPanel::OnLoadScriptClick(wxCommandEvent &evt) {
		wxFileDialog *dialog =
		    new wxFileDialog(this, __("Import script file"), "", "", "", wxFD_OPEN);
		if (dialog->ShowModal() == wxID_OK) {
			std::string path = dialog->GetPath().ToStdString();

			std::ifstream file(path);
			std::string code;
			file.seekg(0, std::ios::end);
			code.reserve(file.tellg());
			file.seekg(0, std::ios::beg);
			code.assign((std::istreambuf_iterator<char>(file)),
			            std::istreambuf_iterator<char>());
			file.close();

			std::unique_ptr<CalXScript> script =
			    this->scriptFactory.openFile(this->env, path);
			this->addHandle(
			    path, new CalxFileScriptHandle(this->scriptPanel, wxID_ANY, this,
			                                   code, std::move(script)));
		}
	}

	void CalxScriptPanel::OnOpenShellClick(wxCommandEvent &evt) {
		std::unique_ptr<CalXScript> script =
		    this->scriptFactory.createShell(this->env);
		this->addHandle(
		    FORMAT(__("Script shell #%s"), std::to_string(this->scripts.size())),
		    new CalxScriptShellHandle(this->scriptPanel, wxID_ANY, this,
		                              std::move(script)));
	}

	void CalxScriptPanel::OnRemoveScriptClick(wxCommandEvent &evt) {
		if (this->scriptList->GetSelection() != wxNOT_FOUND) {
			size_t sel = (size_t) this->scriptList->GetSelection();
			this->scriptList->Delete((unsigned int) sel);
			this->scripts.at(sel)->Close(true);
			this->scripts.erase(this->scripts.begin() + (std::ptrdiff_t) sel);
			updateUI();
		} else {
			std::string message = __("Select script to remove");
			wxMessageDialog *dialog = new wxMessageDialog(
			    this, message, __("Warning"), wxOK | wxICON_WARNING);
			dialog->ShowModal();
			dialog->Destroy();
		}
	}

	void CalxScriptPanel::OnExit(wxCloseEvent &evt) {
		Destroy();
	}

	void CalxScriptPanel::OnEnableEvent(wxThreadEvent &evt) {
		this->Enable(evt.GetPayload<bool>());
	}

	void CalxScriptPanel::addHandle(std::string title, CalxScriptHandle *handle) {
		this->scriptList->Append(title);
		this->scriptPanel->GetSizer()->Add(handle, 1, wxALL | wxEXPAND);
		this->scripts.push_back(handle);
		this->scriptList->SetSelection(this->scripts.size() - 1);
		this->updateUI();
	}
}  // namespace CalXUI