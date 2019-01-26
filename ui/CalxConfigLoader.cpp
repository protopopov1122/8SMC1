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

#include "ui/CalxConfigLoader.h"
#include "ctrl-lib/conf/INI.h"
#include <wx/sizer.h>

namespace CalX::UI {

	CalxConfigLoader::CalxConfigLoader(wxWindow *win, wxWindowID id,
	                                   std::string path)
	    : wxDialog::wxDialog(win, id, __("CalX Configuration Loader"),
	                         wxDefaultPosition, wxDefaultSize,
	                         wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
		this->filename = "";
		if (!path.empty()) {
			this->filesys.ChangePathTo(path);
		}
		this->exiting = false;

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		this->configList = new wxDataViewListCtrl(this, wxID_ANY);
		this->configList->AppendTextColumn(__("Name"));
		this->configList->AppendTextColumn(__("Path"));
		this->configList->AppendTextColumn(__("Description"));
		sizer->Add(this->configList, 1, wxALL | wxEXPAND, 10);

		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALL | wxALIGN_CENTER, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *loadButton = new wxButton(buttonPanel, wxID_ANY, __("Load"));
		wxButton *exitButton = new wxButton(buttonPanel, wxID_ANY, __("Exit"));
		buttonSizer->Add(loadButton);
		buttonSizer->Add(exitButton);
		loadButton->Bind(wxEVT_BUTTON, &CalxConfigLoader::OnLoadClick, this);
		exitButton->Bind(wxEVT_BUTTON, &CalxConfigLoader::OnExitClick, this);

		this->Bind(wxEVT_CLOSE_WINDOW, &CalxConfigLoader::OnClose, this);
		Layout();
	}

	void CalxConfigLoader::load() {
		std::string fname =
		    this->filesys.FindFirst("*.conf.ini", wxFILE).ToStdString();
		while (!fname.empty()) {
			std::string path =
			    this->filesys.URLToFileName(fname).GetFullPath().ToStdString();
			std::string name, descr;
			std::shared_ptr<ConfigurationCatalogue> conf = nullptr;
			std::ifstream cnf(path);
			if (!cnf.good()) {
				fname = this->filesys.FindNext().ToStdString();
				continue;
			} else {
				conf = INIConfiguration::load(cnf, std::cout);
				name = conf->getEntry(CalxConfiguration::Configuration)
				           ->getString(CalxConfigConfiguration::Name,
				                       this->filesys.URLToFileName(fname)
				                           .GetFullName()
				                           .ToStdString());
				descr =
				    conf->getEntry(CalxConfiguration::Configuration)
				        ->getString(CalxConfigConfiguration::Description, __("None"));
			}
			cnf.close();

			CalxConfigFile cf;
			cf.path = path;
			cf.name = name;
			cf.descr = descr;
			this->found.push_back(cf);
			fname = this->filesys.FindNext().ToStdString();
		}
		if (this->found.size() == 0) {
			// Configuration not found
			// Configuration file path will be empty
		} else if (this->found.size() == 1) {
			this->filename = this->found.at(0).path;
		} else {
			this->filename = this->found.at(0).path;
			for (const CalxConfigFile &cfile : this->found) {
				wxVector<wxVariant> data;
				data.push_back(wxVariant(cfile.name));
				data.push_back(wxVariant(cfile.path));
				data.push_back(wxVariant(cfile.descr));
				this->configList->AppendItem(data);
			}
			this->ShowModal();
		}
	}

	std::string CalxConfigLoader::getFileName() {
		return this->filename;
	}

	bool CalxConfigLoader::isExiting() {
		return this->exiting;
	}

	void CalxConfigLoader::OnLoadClick(wxCommandEvent &evt) {
		this->filename = this->found
		                     .at(this->configList->GetSelectedRow() == wxNOT_FOUND
		                             ? 0
		                             : this->configList->GetSelectedRow())
		                     .path;
		Hide();
	}

	void CalxConfigLoader::OnExitClick(wxCommandEvent &evt) {
		this->exiting = true;
		Hide();
	}

	void CalxConfigLoader::OnClose(wxCloseEvent &evt) {
		this->exiting = true;
		Hide();
	}
}  // namespace CalX::UI
