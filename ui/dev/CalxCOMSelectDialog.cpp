/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ui/dev/CalxCOMSelectDialog.h"
#include "calx/ui/CalxApp.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalX::UI {

	CalxCOMSelectDialog::CalxCOMSelectDialog(wxWindow *win, wxWindowID id)
	    : wxDialog::wxDialog(win, id, __("Select COM Port")) {
		this->port = -1;
		this->speed = -1;
		this->parity = SerialPortParity::No;

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 0, wxALL, 10);
		wxFlexGridSizer *mainSizer = new wxFlexGridSizer(2);
		mainPanel->SetSizer(mainSizer);
		mainSizer->Add(
		    new wxStaticText(mainPanel, wxID_ANY, __("Port") + std::string(": ")),
		    0, wxALIGN_RIGHT | wxRIGHT, 5);
		this->portSpin =
		    new wxSpinCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, 1, 255,
		                   (int) wxGetApp()
		                       .getSystemManager()
		                       .getConfiguration()
		                       .getEntry(CalxConfiguration::SerialConnection)
		                       ->getInt(CalxDeviceConfiguration::COMPort, 1));
		mainSizer->Add(this->portSpin);
		mainSizer->Add(new wxStaticText(mainPanel, wxID_ANY,
		                                __("Baud rate") + std::string(": ")),
		               0, wxALIGN_RIGHT | wxRIGHT, 5);
		this->speedSpin =
		    new wxSpinCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, 0, INT_MAX,
		                   (int) wxGetApp()
		                       .getSystemManager()
		                       .getConfiguration()
		                       .getEntry(CalxConfiguration::SerialConnection)
		                       ->getInt(CalxDeviceConfiguration::COMSpeed, 9600));
		mainSizer->Add(this->speedSpin);
		mainSizer->Add(
		    new wxStaticText(mainPanel, wxID_ANY, __("Parity") + std::string(": ")),
		    0, wxALIGN_RIGHT | wxRIGHT, 5);
		this->parityCh = new wxChoice(mainPanel, wxID_ANY);
		this->parityCh->Append(__("No"));
		this->parityCh->Append(__("Odd"));
		this->parityCh->Append(__("Even"));
		this->parityCh->Append(__("Mark"));
		this->parityCh->Append(__("Space"));
		this->parityCh->SetSelection(
		    (int) wxGetApp()
		        .getSystemManager()
		        .getConfiguration()
		        .getEntry(CalxConfiguration::SerialConnection)
		        ->getInt(CalxDeviceConfiguration::COMParity, 0));
		mainSizer->Add(this->parityCh);

		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 0, wxALL | wxALIGN_CENTER, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_OK, __("OK"));
		wxButton *cancelButton =
		    new wxButton(buttonPanel, wxID_CANCEL, __("Cancel"));
		buttonSizer->Add(okButton);
		buttonSizer->Add(cancelButton);
		okButton->Bind(wxEVT_BUTTON, &CalxCOMSelectDialog::OnOkClick, this);
		cancelButton->Bind(wxEVT_BUTTON, &CalxCOMSelectDialog::OnCancelClick, this);

		Layout();
		Fit();
#ifdef __WXGTK__
		wxSize size = GetSize();
		size.x *= 1.15;
		size.y *= 1.15;
		SetSize(size);
#endif
	}

	int16_t CalxCOMSelectDialog::getPort() {
		return this->port;
	}

	int32_t CalxCOMSelectDialog::getSpeed() {
		return this->speed;
	}

	SerialPortParity CalxCOMSelectDialog::getParity() {
		return this->parity;
	}

	void CalxCOMSelectDialog::OnOkClick(wxCommandEvent &evt) {
		this->port = (int16_t) this->portSpin->GetValue();
		this->speed = this->speedSpin->GetValue();
		this->parity = SerialPortParity::No;
		switch (this->parityCh->GetSelection()) {
			case 0:
				this->parity = SerialPortParity::No;
				break;
			case 1:
				this->parity = SerialPortParity::Odd;
				break;
			case 2:
				this->parity = SerialPortParity::Even;
				break;
			case 3:
				this->parity = SerialPortParity::Mark;
				break;
			case 4:
				this->parity = SerialPortParity::Space;
				break;
		}
		Hide();
	}

	void CalxCOMSelectDialog::OnCancelClick(wxCommandEvent &evt) {
		Hide();
	}
}  // namespace CalX::UI
