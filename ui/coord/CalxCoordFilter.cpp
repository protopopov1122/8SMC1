/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#include "ui/coord/CalxCoordFilter.h"
#include "ui/CalxApp.h"
#include "ui/CalxFrame.h"
#include "ui/CalxPanel.h"
#include <string>

namespace CalXUI {

	CalxCoordFilter::CalxCoordFilter(wxWindow *win, wxWindowID id)
	    : wxPanel::wxPanel(win, id) {}

	CalxCoordBasicFilter::CalxCoordBasicFilter(
	    wxWindow *win, wxWindowID id, std::shared_ptr<BasicCoordTranslator> tr)
	    : CalxCoordFilter::CalxCoordFilter(win, id) {
		this->translator = tr;

		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);

		this->xoffset =
		    new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,
		                   (int) this->translator->getCenter().x);
		this->yoffset =
		    new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,
		                   (int) this->translator->getCenter().y);
		this->xscale =
		    new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,
		                   (int) this->translator->getScale().w);
		this->yscale =
		    new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
		                   wxDefaultSize, wxSP_ARROW_KEYS, INT_MIN, INT_MAX,
		                   (int) this->translator->getScale().h);

		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("X offset") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xoffset, 0, wxALL | wxEXPAND);
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Y offset") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yoffset, 0, wxALL | wxEXPAND);
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("X scale") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xscale, 0, wxALL | wxEXPAND);
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Y scale") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yscale, 0, wxALL | wxEXPAND);

		Layout();
	}

	std::shared_ptr<CoordTranslator> CalxCoordBasicFilter::getTranslator() {
		updateData();
		return this->translator;
	}

	void CalxCoordBasicFilter::updateData() {
		motor_point_t offset = { this->xoffset->GetValue(),
			                       this->yoffset->GetValue() };
		motor_size_t scale = { this->xscale->GetValue(), this->yscale->GetValue() };
		this->translator->setCenter(offset);
		this->translator->setScale(scale);
	}

	CalxCoordLinearFilter::CalxCoordLinearFilter(
	    wxWindow *win, wxWindowID id, std::shared_ptr<LinearCoordTranslator> ctrl)
	    : CalxCoordFilter::CalxCoordFilter(win, id) {
		this->translator = ctrl;

		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);

		this->xoffset = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, ctrl->getOffset().x,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->yoffset = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, ctrl->getOffset().y,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->xscale = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, ctrl->getScale().x,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->yscale = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, ctrl->getScale().y,
		    wxGetApp().getUnitProcessor().getUnitPrecision());

		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("X offset") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xoffset, 0, wxALL | wxEXPAND);
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Y offset") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yoffset, 0, wxALL | wxEXPAND);
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("X scale") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xscale, 0, wxALL | wxEXPAND);
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Y scale") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yscale, 0, wxALL | wxEXPAND);

		xoffset->Bind(wxEVT_SPINCTRL, &CalxCoordLinearFilter::OnFieldChange, this);
		yoffset->Bind(wxEVT_SPINCTRL, &CalxCoordLinearFilter::OnFieldChange, this);
		xscale->Bind(wxEVT_SPINCTRL, &CalxCoordLinearFilter::OnFieldChange, this);
		yscale->Bind(wxEVT_SPINCTRL, &CalxCoordLinearFilter::OnFieldChange, this);

		Layout();
	}

	std::shared_ptr<CoordTranslator> CalxCoordLinearFilter::getTranslator() {
		updateData();
		return this->translator;
	}

	void CalxCoordLinearFilter::updateData() {
		double ox, oy, sx, sy;
		ox = this->xoffset->GetValue();
		oy = this->yoffset->GetValue();
		sx = this->xscale->GetValue();
		sy = this->yscale->GetValue();
		coord_point_t offset = { ox, oy };
		coord_scale_t scale = { sx, sy };
		this->translator->setOffset(offset);
		this->translator->setScale(scale);
	}

	void CalxCoordLinearFilter::OnFieldChange(wxCommandEvent &evt) {
		updateData();
	}

	CalxCoordLogarithmicFilter::CalxCoordLogarithmicFilter(
	    wxWindow *win, wxWindowID id,
	    std::shared_ptr<LogarithmicCoordTranslator> trans)
	    : CalxCoordFilter::CalxCoordFilter(win, id) {
		this->translator = trans;

		wxFlexGridSizer *sizer = new wxFlexGridSizer(2);
		SetSizer(sizer);

		this->xscale = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, trans->getScale().x,
		    wxGetApp().getUnitProcessor().getUnitPrecision());
		this->yscale = new wxSpinCtrlDouble(
		    this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
		    wxSP_ARROW_KEYS, INT_MIN, INT_MAX, trans->getScale().x,
		    wxGetApp().getUnitProcessor().getUnitPrecision());

		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("X scale") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(xscale, 0, wxALL | wxEXPAND);
		sizer->Add(
		    new wxStaticText(this, wxID_ANY, __("Y scale") + std::string(":")), 0,
		    wxRIGHT | wxALIGN_RIGHT, 5);
		sizer->Add(yscale, 0, wxALL | wxEXPAND);

		xscale->Bind(wxEVT_SPINCTRL, &CalxCoordLogarithmicFilter::OnFieldChange,
		             this);
		yscale->Bind(wxEVT_SPINCTRL, &CalxCoordLogarithmicFilter::OnFieldChange,
		             this);

		Layout();
	}

	std::shared_ptr<CoordTranslator> CalxCoordLogarithmicFilter::getTranslator() {
		updateData();
		return this->translator;
	}

	void CalxCoordLogarithmicFilter::updateData() {
		double sx, sy;
		sx = this->xscale->GetValue();
		sy = this->yscale->GetValue();
		coord_scale_t scale = { sx, sy };
		this->translator->setScale(scale);
	}

	void CalxCoordLogarithmicFilter::OnFieldChange(wxCommandEvent &evt) {
		updateData();
	}

	CalxCoordPolarFilter::CalxCoordPolarFilter(
	    wxWindow *win, wxWindowID id, std::shared_ptr<PolarCoordTranslator> polar)
	    : CalxCoordFilter::CalxCoordFilter(win, id) {
		this->translator = polar;

		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		SetSizer(sizer);
		sizer->Add(new wxStaticText(this, wxID_ANY,
		                            __("Polar coordinate filter have not any "
		                               "parameters.\n Add other filters.")));
	}

	std::shared_ptr<CoordTranslator> CalxCoordPolarFilter::getTranslator() {
		return this->translator;
	}

	CalxCoordFilterCtrl::CalxCoordFilterCtrl(
	    wxWindow *win, wxWindowID id, std::shared_ptr<ComplexCoordTranslator> tr)
	    : CalxCoordFilter::CalxCoordFilter(win, id) {
		wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
		SetSizer(sizer);

		wxPanel *listPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(listPanel, 0, wxRIGHT | wxEXPAND, 5);
		wxBoxSizer *listSizer = new wxBoxSizer(wxVERTICAL);
		listPanel->SetSizer(listSizer);
		this->filterList = new wxListBox(listPanel, wxID_ANY);
		listSizer->Add(this->filterList, 1, wxBOTTOM | wxEXPAND);
		this->filterList->Bind(wxEVT_LISTBOX, &CalxCoordFilterCtrl::OnListClick,
		                       this);
		wxButton *addLinearButton =
		    new wxButton(listPanel, wxID_ANY, __("Add Linear"));
		wxButton *addLogarithmicButton =
		    new wxButton(listPanel, wxID_ANY, __("Add Logarithmic"));
		wxButton *addPolarButton =
		    new wxButton(listPanel, wxID_ANY, __("Add Polar"));
		this->removeButton = new wxButton(listPanel, wxID_ANY, __("Remove"));
		listSizer->Add(addLinearButton, 0, wxEXPAND);
		listSizer->Add(addLogarithmicButton, 0, wxEXPAND);
		listSizer->Add(addPolarButton, 0, wxEXPAND);
		listSizer->Add(removeButton, 0, wxEXPAND);
		addLinearButton->Bind(wxEVT_BUTTON, &CalxCoordFilterCtrl::OnAddLinearClick,
		                      this);
		addLogarithmicButton->Bind(
		    wxEVT_BUTTON, &CalxCoordFilterCtrl::OnAddLogarithmicClick, this);
		addPolarButton->Bind(wxEVT_BUTTON, &CalxCoordFilterCtrl::OnAddPolarClick,
		                     this);
		;
		removeButton->Bind(wxEVT_BUTTON, &CalxCoordFilterCtrl::OnRemoveClick, this);

		this->mainPanel = new wxPanel(this, wxID_ANY);
		sizer->Add(mainPanel, 1, wxALL | wxEXPAND);
		wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
		mainPanel->SetSizer(mainSizer);

		this->trans = tr;

		if (this->trans == nullptr) {
			coord_point_t cen = wxGetApp().getUnitProcessor().getUnitOffset();
			coord_scale_t scl = { wxGetApp().getUnitProcessor().getUnitScale(),
				                    wxGetApp().getUnitProcessor().getUnitScale() };
			std::shared_ptr<LinearCoordTranslator> basic =
			    std::make_shared<LinearCoordTranslator>(cen, scl);
			this->trans = std::make_shared<ComplexCoordTranslator>(basic);
			addFilter(basic);
		} else {
			for (size_t i = 0; i < trans->getSize(); i++) {
				addFilter(trans->getTranslator(i));
			}
		}

		updateUI();
		Fit();
	}

	std::shared_ptr<CoordTranslator> CalxCoordFilterCtrl::getTranslator() {
		for (const auto &f : this->filter) {
			f->getTranslator();
		}
		return this->trans;
	}

	void CalxCoordFilterCtrl::updateUI() {
		for (const auto &f : this->filter) {
			f->Show(false);
		}
		this->removeButton->Enable(filterList->GetSelection() != 0 &&
		                           filterList->GetSelection() != wxNOT_FOUND);
		if (filterList->GetSelection() != wxNOT_FOUND) {
			this->filter.at((size_t) filterList->GetSelection())->Show(true);
		}
		Layout();
		wxGetApp().getMainFrame()->getPanel()->updateUI();
	}

	void CalxCoordFilterCtrl::OnListClick(wxCommandEvent &evt) {
		updateUI();
	}

	void CalxCoordFilterCtrl::OnAddLinearClick(wxCommandEvent &evt) {
		std::shared_ptr<CoordTranslator> base =
		    this->trans->getTranslator(this->trans->getSize() - 1);
		coord_point_t offset = { 0, 0 };
		coord_scale_t scale = { 1, 1 };
		std::shared_ptr<LinearCoordTranslator> linear =
		    std::make_shared<LinearCoordTranslator>(offset, scale, base);
		this->trans->add(linear);
		addFilter(linear);
	}

	void CalxCoordFilterCtrl::OnAddLogarithmicClick(wxCommandEvent &evt) {
		std::shared_ptr<CoordTranslator> base =
		    this->trans->getTranslator(this->trans->getSize() - 1);
		coord_scale_t sc = { 0, 0 };
		std::shared_ptr<LogarithmicCoordTranslator> logt =
		    std::make_shared<LogarithmicCoordTranslator>(sc, base);
		this->trans->add(logt);
		addFilter(logt);
	}

	void CalxCoordFilterCtrl::OnAddPolarClick(wxCommandEvent &evt) {
		std::shared_ptr<CoordTranslator> base =
		    this->trans->getTranslator(this->trans->getSize() - 1);
		std::shared_ptr<PolarCoordTranslator> polar =
		    std::make_shared<PolarCoordTranslator>(base);
		this->trans->add(polar);
		addFilter(polar);
	}

	void CalxCoordFilterCtrl::OnRemoveClick(wxCommandEvent &evt) {
		if (filterList->GetSelection() != wxNOT_FOUND) {
			size_t i = (size_t) filterList->GetSelection();
			filter.at(i)->Destroy();
			filter.erase(filter.begin() + (std::ptrdiff_t) i);
			filterList->Delete((unsigned int) i);
			filterList->SetSelection((int) (filter.size() > 0
			                                    ? (int) filter.size() - 1
			                                    : (int) wxNOT_FOUND));
			trans->remove(i);
		}
		updateUI();
	}

	void CalxCoordFilterCtrl::addFilter(std::shared_ptr<CoordTranslator> tr) {
		CalxCoordFilter *fil = nullptr;
		switch (tr->getType()) {
			case CoordType::BasicCoord:
				fil = new CalxCoordBasicFilter(
				    mainPanel, wxID_ANY,
				    std::static_pointer_cast<BasicCoordTranslator>(tr));
				filterList->Append(__("Basic"));
				break;
			case CoordType::LinearCoord:
				fil = new CalxCoordLinearFilter(
				    mainPanel, wxID_ANY,
				    std::static_pointer_cast<LinearCoordTranslator>(tr));
				filterList->Append(__("Linear"));
				break;
			case CoordType::LogarithmicCoord:
				fil = new CalxCoordLogarithmicFilter(
				    mainPanel, wxID_ANY,
				    std::static_pointer_cast<LogarithmicCoordTranslator>(tr));
				filterList->Append(__("Logarithmic"));
				break;
			case CoordType::PolarCoord:
				fil = new CalxCoordPolarFilter(
				    mainPanel, wxID_ANY,
				    std::static_pointer_cast<PolarCoordTranslator>(tr));
				filterList->Append(__("Polar"));
				break;
			default:
				break;
		}
		if (fil == nullptr) {
			return;
		}
		this->filter.push_back(fil);
		mainPanel->GetSizer()->Add(fil, 1, wxALL | wxEXPAND);
		filterList->SetSelection((int) (filter.size() - 1));
		updateUI();
	}
}  // namespace CalXUI
