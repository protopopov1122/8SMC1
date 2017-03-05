#include "CalxCoordTranslator.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace CalX {
	
	CalxCoordTranslator::CalxCoordTranslator(wxWindow *win, wxWindowID id)
		: wxPanel::wxPanel(win, id) {
		
		motor_point_t cen = {0, 0};
		motor_size_t size = {1, 1};
		this->translator = new BasicCoordTranslator(cen, size);
		
		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);
		
		this->xoffset = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->yoffset = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 0);
		this->xscale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 1);
		this->yscale = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX, 1);
		
		sizer->Add(new wxStaticText(this, wxID_ANY, "Basic coordinate settings:"));
		sizer->Add(new wxStaticText(this, wxID_ANY, ""));
		sizer->Add(new wxStaticText(this, wxID_ANY, "X offset:"), wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xoffset);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Y offset:"), wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yoffset);
		sizer->Add(new wxStaticText(this, wxID_ANY, "X scale:"), wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xscale);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Y scale:"), wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yscale);
				
		Layout();
	}
	
	CoordTranslator *CalxCoordTranslator::getTranslator() {
		updateData();
		return this->translator;
	}
	
	void CalxCoordTranslator::updateData() {
		motor_point_t offset = {this->xoffset->GetValue(), this->yoffset->GetValue()};
		motor_size_t scale = {this->xscale->GetValue(), this->yscale->GetValue()};
		this->translator->setCenter(offset);
		this->translator->setScale(scale);
	}
}