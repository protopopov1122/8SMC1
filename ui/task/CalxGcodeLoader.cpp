#include "CalxGcodeLoader.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>

namespace CalXUI {
	
	CalxGcodeLoader::CalxGcodeLoader(wxWindow *win, wxWindowID id)
		: wxDialog::wxDialog(win, id, "Load GCode") {
		
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		this->state = false;
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Choose GCode file:"));
		wxPanel *gcodePanel = new wxPanel(this, wxID_ANY);
		sizer->Add(gcodePanel, 0, wxALL | wxEXPAND, 5);
		wxBoxSizer *gcodeSizer = new wxBoxSizer(wxHORIZONTAL);
		gcodePanel->SetSizer(gcodeSizer);
		this->gcodePath = new wxTextCtrl(gcodePanel, wxID_ANY, "");
		gcodeSizer->Add(gcodePath, 0, wxEXPAND);
		this->gcodePath->SetMinSize(wxSize(200, -1));
		wxButton *chooseButton = new wxButton(gcodePanel, wxID_ANY, "Choose");
		gcodeSizer->Add(chooseButton, 0, wxALL);
		chooseButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnChooseClick, this);
		
		this->translator = new CalxCoordTranslator(this, wxID_ANY);
		sizer->Add(translator, 0, wxALL | wxEXPAND, 10);
				
		wxPanel *buttonPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(buttonPanel, 1, wxALL | wxALIGN_CENTER, 5);
		wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		buttonPanel->SetSizer(buttonSizer);
		wxButton *okButton = new wxButton(buttonPanel, wxID_ANY, "OK");
		wxButton *cancelButton = new wxButton(buttonPanel, wxID_ANY, "Cancel");
		buttonSizer->Add(okButton, 0, wxALIGN_CENTER);
		buttonSizer->Add(cancelButton, 0, wxALIGN_CENTER);
		okButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnOkClick, this);
		cancelButton->Bind(wxEVT_BUTTON, &CalxGcodeLoader::OnCancelClick, this);
		
		Layout();
		Fit();
		
		wxCommandEvent evt;
		OnChooseClick(evt);
	}
	
	bool CalxGcodeLoader::isLoaded() {
		return this->state;
	}
	
	std::string CalxGcodeLoader::getPath() {
		return this->gcodePath->GetValue().ToStdString();
	}
	
	CoordTranslator *CalxGcodeLoader::getTranslator() {
		return this->translator->getTranslator();
	}
	
	void CalxGcodeLoader::OnOkClick(wxCommandEvent &evt) {
		this->state = true;
		Hide();
	}
	
	void CalxGcodeLoader::OnCancelClick(wxCommandEvent &evt) {
		delete this->translator->getTranslator();
		Hide();
	}
	
	void CalxGcodeLoader::OnChooseClick(wxCommandEvent &evt) {
		wxFileDialog *dialog = new wxFileDialog(this);
		if (dialog->ShowModal() == wxID_OK) {
			this->gcodePath->SetValue(dialog->GetPath());
		}
		dialog->Destroy();
	}
	
}