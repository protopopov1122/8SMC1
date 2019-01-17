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

#include "ui/script/CalxScriptHandle.h"
#include <wx/splitter.h>
#include <iostream>

namespace CalXUI {

	class CalxScriptHookCallAction : public CalxAction {
	 public:
		CalxScriptHookCallAction(CalXScript &script, std::string hook)
		    : script(script), hook(std::move(hook)) {}

		virtual void perform(SystemManager *sysman) {
			script.invokeFunction(hook);
		}

		virtual void stop() {}

	 private:
		CalXScript &script;
		std::string hook;
	};

	class CalxScriptExecuteAction : public CalxAction {
	 public:
		CalxScriptExecuteAction(CalXScript &script, std::string code)
		    : script(script), code(code) {}

		virtual void perform(SystemManager *sysman) {
			script.executeScript(code);
		}

		virtual void stop() {}

	 private:
		CalXScript &script;
		std::string code;
	};

	CalxScriptHandle::CalxScriptHandle(wxWindow *win, wxWindowID id)
	    : wxPanel::wxPanel(win, id) {}

	CalxFileScriptHandle::CalxFileScriptHandle(wxWindow *win, wxWindowID id,
	                                           CalxActionReceiver *recv,
	                                           std::string code,
	                                           std::unique_ptr<CalXScript> script)
	    : CalxScriptHandle::CalxScriptHandle(win, id),
	      recv(recv),
	      script(std::move(script)) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		wxPanel *callPanel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *callSizer = new wxBoxSizer(wxHORIZONTAL);
		callPanel->SetSizer(callSizer);
		sizer->Add(callPanel, 0, wxALL | wxEXPAND);

		wxStaticText *hookLabel =
		    new wxStaticText(this, wxID_ANY, __("Call a hook") + std::string(": "));
		this->hookText = new wxTextCtrl(callPanel, wxID_ANY, "");
		wxButton *callButton = new wxButton(callPanel, wxID_ANY, __("Call"));
		callButton->Bind(wxEVT_BUTTON, &CalxFileScriptHandle::OnCallClick, this);
		callSizer->Add(hookLabel, 0, wxALIGN_CENTER);
		callSizer->Add(hookText, 1, wxALL | wxALIGN_CENTER);
		callSizer->Add(callButton, 0, wxALIGN_CENTER);

		wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
		sizer->Add(splitter, 1, wxALL | wxEXPAND);

		wxTextCtrl *codeText =
		    new wxTextCtrl(splitter, wxID_ANY, code, wxDefaultPosition,
		                   wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

		wxPanel *shellPanel = new wxPanel(splitter, wxID_ANY);
		wxBoxSizer *shellSizer = new wxBoxSizer(wxHORIZONTAL);
		shellPanel->SetSizer(shellSizer);
		this->shellText =
		    new wxTextCtrl(shellPanel, wxID_ANY, "", wxDefaultPosition,
		                   wxDefaultSize, wxTE_MULTILINE);
		shellSizer->Add(shellText, 1, wxALL | wxEXPAND);
		wxButton *executeButton = new wxButton(shellPanel, wxID_ANY, __("Execute"));
		shellSizer->Add(executeButton);
		executeButton->Bind(wxEVT_BUTTON, &CalxFileScriptHandle::OnExecuteClick,
		                    this);

		splitter->Initialize(codeText);
		splitter->SplitHorizontally(codeText, shellPanel);
		splitter->SetSashGravity(0.8f);

		this->Bind(wxEVT_CLOSE_WINDOW, &CalxFileScriptHandle::OnExit, this);
	}

	void CalxFileScriptHandle::OnCallClick(wxCommandEvent &evt) {
		std::string hook = this->hookText->GetValue().ToStdString();
		this->recv->sendAction(
		    std::make_unique<CalxScriptHookCallAction>(*this->script, hook));
	}

	void CalxFileScriptHandle::OnExecuteClick(wxCommandEvent &evt) {
		std::string code = this->shellText->GetValue().ToStdString();
		this->recv->sendAction(
		    std::make_unique<CalxScriptExecuteAction>(*this->script, code));
	}

	void CalxFileScriptHandle::OnExit(wxCloseEvent &evt) {
		Destroy();
	}

	CalxScriptShellHandle::CalxScriptShellHandle(
	    wxWindow *win, wxWindowID id, CalxActionReceiver *recv,
	    std::unique_ptr<CalXScript> script)
	    : CalxScriptHandle::CalxScriptHandle(win, id),
	      recv(recv),
	      script(std::move(script)) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		this->shellText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
		                                 wxDefaultSize, wxTE_MULTILINE);
		sizer->Add(shellText, 1, wxALL | wxEXPAND);
		wxButton *executeButton = new wxButton(this, wxID_ANY, __("Execute"));
		sizer->Add(executeButton);
		executeButton->Bind(wxEVT_BUTTON, &CalxScriptShellHandle::OnExecuteClick,
		                    this);

		this->Bind(wxEVT_CLOSE_WINDOW, &CalxScriptShellHandle::OnExit, this);
	}

	void CalxScriptShellHandle::OnExecuteClick(wxCommandEvent &evt) {
		std::string code = this->shellText->GetValue().ToStdString();
		this->recv->sendAction(
		    std::make_unique<CalxScriptExecuteAction>(*this->script, code));
	}

	void CalxScriptShellHandle::OnExit(wxCloseEvent &evt) {
		Destroy();
	}
}  // namespace CalXUI