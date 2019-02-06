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

#include "calx/ui/task/CalxGcodeLoader.h"
#include <wx/gdicmn.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalX::UI {

	CalxGcodeLoader::CalxGcodeLoader(wxWindow *win, wxWindowID id)
	    : wxDialog::wxDialog(win, id, __("Load GCode")) {
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);

		wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxALL | wxALIGN_CENTER);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		mainPanel->SetSizer(mainSizer);
		this->state = false;

		mainSizer->Add(new wxStaticText(
		    mainPanel, wxID_ANY, __("Choose GCode file") + std::string(":")));
		wxPanel *gcodePanel = new wxPanel(mainPanel, wxID_ANY);
		mainSizer->Add(gcodePanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *gcodeSizer = new wxBoxSizer(wxHORIZONTAL);
		gcodePanel->SetSizer(gcodeSizer);
		this->gcodePath = new wxTextCtrl(gcodePanel, wxID_ANY, "");
		gcodeSizer->Add(gcodePath, 0, wxEXPAND);
		this->gcodePath->SetMinSize(wxSize(200, -1));
		wxButton *chooseButton = new wxButton(gcodePanel, wxID_ANY, __("Choose"));
		gcodeSizer->Add(chooseButton, 0, wxALL);
		chooseButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnChooseClick, this);

		ConfigurationCatalogue &conf =
		    wxGetApp().getSystemManager().getConfiguration();
		coord_point_t trans_offset = {
			static_cast<double>(conf.getEntry(CalxConfiguration::Coordinates)
			                        ->getReal(CalxCoordConfiguration::OffsetX, 0.0)),
			static_cast<double>(conf.getEntry(CalxConfiguration::Coordinates)
			                        ->getReal(CalxCoordConfiguration::OffsetY, 0.0))
		};
		coord_scale_t trans_scale = {
			static_cast<double>(conf.getEntry(CalxConfiguration::Coordinates)
			                        ->getReal(CalxCoordConfiguration::ScaleX, 1.0)),
			static_cast<double>(conf.getEntry(CalxConfiguration::Coordinates)
			                        ->getReal(CalxCoordConfiguration::ScaleY, 1.0))
		};
		coord_point_t unit_offset = wxGetApp().getUnitProcessor().getUnitOffset();
		coord_scale_t unit_scale = { wxGetApp().getUnitProcessor().getUnitScale(),
			                           wxGetApp().getUnitProcessor().getUnitScale() };
		std::shared_ptr<LinearCoordTranslator> unit_trans =
		    std::make_shared<LinearCoordTranslator>(unit_offset, unit_scale);
		std::shared_ptr<LinearCoordTranslator> trans =
		    std::make_shared<LinearCoordTranslator>(trans_offset, trans_scale,
		                                            unit_trans);
		std::shared_ptr<ComplexCoordTranslator> ctrans =
		    std::make_shared<ComplexCoordTranslator>(unit_trans);
		ctrans->add(trans);

		this->translator = new CalxCoordFilterCtrl(mainPanel, wxID_ANY, ctrans);
		mainSizer->Add(translator, 0, wxALL | wxEXPAND, 10);

		wxPanel *buttonPanel = new wxPanel(mainPanel, wxID_ANY);
		mainSizer->Add(buttonPanel, 1, wxALL | wxALIGN_CENTER, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_OK, __("OK"));
		wxButton *cancelButton =
		    new wxButton(buttonPanel, wxID_CANCEL, __("Cancel"));
		buttonSizer->Add(okButton);
		buttonSizer->Add(cancelButton);
		okButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnOkClick, this);
		cancelButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnCancelClick, this);
		Bind(wxEVT_SHOW, &CalxGcodeLoader::OnShow, this);

		Layout();
		Fit();
#ifdef __WXGTK__
		wxSize size = GetSize();
		size.x *= 1.1;
		size.y *= 1.1;
		SetSize(size);
#endif
	}

	bool CalxGcodeLoader::isLoaded() {
		return this->state;
	}

	std::string CalxGcodeLoader::getPath() {
		return this->gcodePath->GetValue().ToStdString();
	}

	std::shared_ptr<ComplexCoordTranslator> CalxGcodeLoader::getTranslator() {
		return std::static_pointer_cast<ComplexCoordTranslator>(
		    this->translator->getTranslator());
	}

	void CalxGcodeLoader::OnOkClick(wxCommandEvent &evt) {
		this->state = true;
		Hide();
	}

	void CalxGcodeLoader::OnCancelClick(wxCommandEvent &evt) {
		Hide();
	}

	void CalxGcodeLoader::OnChooseClick(wxCommandEvent &evt) {
		wxFileDialog *dialog = new wxFileDialog(this);
		if (dialog->ShowModal() == wxID_OK) {
			this->gcodePath->SetValue(dialog->GetPath());
		}
		dialog->Destroy();
	}

	void CalxGcodeLoader::OnShow(wxShowEvent &evt) {
		if (evt.IsShown()) {
			wxFileDialog *dialog = new wxFileDialog(this);
			if (dialog->ShowModal() == wxID_OK) {
				this->gcodePath->SetValue(dialog->GetPath());
			}
			dialog->Destroy();
		}
	}
}  // namespace CalX::UI
