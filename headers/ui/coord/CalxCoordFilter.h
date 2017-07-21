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

#ifndef CALX_UI_CALX_COORD_TRANSLATOR_H_
#define CALX_UI_CALX_COORD_TRANSLATOR_H_

#include "ui/CalxApp.h"
#include <vector>
#include <wx/listbox.h>
#include <wx/spinctrl.h>

namespace CalXUI {

	class CalxCoordFilter : public wxPanel {
	 public:
		CalxCoordFilter(wxWindow *, wxWindowID);
		virtual std::shared_ptr<CoordTranslator> getTranslator() = 0;
	};

	class CalxCoordBasicFilter : public CalxCoordFilter {
	 public:
		CalxCoordBasicFilter(wxWindow *, wxWindowID,
		                     std::shared_ptr<BasicCoordTranslator>);
		virtual std::shared_ptr<CoordTranslator> getTranslator();

	 private:
		void updateData();
		std::shared_ptr<BasicCoordTranslator> translator;
		wxSpinCtrl *xoffset;
		wxSpinCtrl *yoffset;
		wxSpinCtrl *xscale;
		wxSpinCtrl *yscale;
	};

	class CalxCoordLinearFilter : public CalxCoordFilter {
	 public:
		CalxCoordLinearFilter(wxWindow *, wxWindowID,
		                      std::shared_ptr<LinearCoordTranslator>);
		virtual std::shared_ptr<CoordTranslator> getTranslator();

	 private:
		void updateData();
		void OnFieldChange(wxCommandEvent &);

		std::shared_ptr<LinearCoordTranslator> translator;
		wxSpinCtrlDouble *xoffset;
		wxSpinCtrlDouble *yoffset;
		wxSpinCtrlDouble *xscale;
		wxSpinCtrlDouble *yscale;
	};

	class CalxCoordLogarithmicFilter : public CalxCoordFilter {
	 public:
		CalxCoordLogarithmicFilter(wxWindow *, wxWindowID,
		                           std::shared_ptr<LogarithmicCoordTranslator>);
		virtual std::shared_ptr<CoordTranslator> getTranslator();

	 private:
		void updateData();
		void OnFieldChange(wxCommandEvent &);

		std::shared_ptr<LogarithmicCoordTranslator> translator;
		wxSpinCtrlDouble *xscale;
		wxSpinCtrlDouble *yscale;
	};

	class CalxCoordPolarFilter : public CalxCoordFilter {
	 public:
		CalxCoordPolarFilter(wxWindow *, wxWindowID,
		                     std::shared_ptr<PolarCoordTranslator>);
		virtual std::shared_ptr<CoordTranslator> getTranslator();

	 private:
		std::shared_ptr<PolarCoordTranslator> translator;
	};

	class CalxCoordFilterCtrl : public CalxCoordFilter {
	 public:
		CalxCoordFilterCtrl(wxWindow *, wxWindowID,
		                    std::shared_ptr<ComplexCoordTranslator> = nullptr);
		virtual std::shared_ptr<CoordTranslator> getTranslator();
		void updateUI();

	 private:
		void OnListClick(wxCommandEvent &);
		void OnAddLinearClick(wxCommandEvent &);
		void OnAddLogarithmicClick(wxCommandEvent &);
		void OnAddPolarClick(wxCommandEvent &);
		void OnRemoveClick(wxCommandEvent &);

		void addFilter(std::shared_ptr<CoordTranslator>);

		wxPanel *mainPanel;
		wxListBox *filterList;
		wxButton *removeButton;

		std::shared_ptr<ComplexCoordTranslator> trans;
		std::vector<CalxCoordFilter *> filter;
	};
}

#endif
