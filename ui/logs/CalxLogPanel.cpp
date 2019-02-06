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

#include "calx/ui/logs/CalxLogPanel.h"
#include "calx/ctrl-lib/logger/Sink.h"
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <iostream>

namespace CalX::UI {

	wxDECLARE_EVENT(wxEVT_FLUSH_TEXT, wxThreadEvent);
	wxDEFINE_EVENT(wxEVT_FLUSH_TEXT, wxThreadEvent);

	class CalxLogPanel::UIJournalSink : public JournalAbstractSink {
	 public:
		UIJournalSink(wxWindow *win, const std::string &name)
		    : JournalAbstractSink::JournalAbstractSink(name), win(win) {}

		void reset() {
			this->win = nullptr;
		}

	 protected:
		void log(const std::string &text) const override {
			if (win) {
				wxThreadEvent evt(wxEVT_FLUSH_TEXT);
				evt.SetPayload(text);
				wxPostEvent(this->win, evt);
			} else {
				std::cout << text;
			}
		}

	 private:
		wxWindow *win;
	};

	CalxLogPanel::CalxLogPanel(wxWindow *parent, wxWindowID id)
	    : CalxPanelPane::CalxPanelPane(parent, id) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);

		this->logPane =
		    new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
		                   wxTE_MULTILINE | wxTE_READONLY);
		sizer->Add(this->logPane, 1, wxALL | wxEXPAND);

		this->Bind(wxEVT_FLUSH_TEXT, &CalxLogPanel::OnFlushText, this);
		this->Bind(wxEVT_CLOSE_WINDOW, &CalxLogPanel::OnClose, this);
	}

	bool CalxLogPanel::isBusy() {
		return false;
	}

	void CalxLogPanel::shutdown() {}

	void CalxLogPanel::updateUI() {
		this->Refresh();
	}

	std::shared_ptr<JournalSink> CalxLogPanel::getSink(const std::string &name) {
		auto sink = std::make_shared<CalxLogPanel::UIJournalSink>(this, name);
		this->sinks.push_back(sink);
		return sink;
	}

	void CalxLogPanel::OnFlushText(wxThreadEvent &evt) {
		std::ostream os(this->logPane);
		os << evt.GetPayload<std::string>();
	}

	void CalxLogPanel::OnClose(wxCloseEvent &evt) {
		for (auto &sink : this->sinks) {
			if (std::shared_ptr<UIJournalSink> uiSink = sink.lock()) {
				uiSink->reset();
			}
		}
		this->Destroy();
	}
}  // namespace CalX::UI