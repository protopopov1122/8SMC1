#ifndef CALX_UI_MATH_CALX_MATH_PANEL_H_
#define CALX_UI_MATH_CALX_MATH_PANEL_H_

#include "ui/CalxApp.h"
#include "ui/CalxPanelPane.h"
#include "ui/math/CalxMathObject.h"

wxDECLARE_EVENT(wxEVT_ADD_FORMULA, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_REMOVE_FORMULA, wxThreadEvent);

namespace CalXUI {

	class CalxMathEngine {
	 public:
		virtual ~CalxMathEngine() = default;
		virtual std::size_t getFormulaCount() = 0;
		virtual void addFormula(const std::string &, const std::string &, const std::map<std::string, std::string> &) = 0;
		virtual bool removeFormula(std::size_t) = 0;
	};

	class CalxMathPanel : public CalxPanelPane, public CalxMathEngine {
	 public:
		CalxMathPanel(wxWindow *, wxWindowID);
		bool isBusy() override;
		void shutdown() override;
		void updateUI() override;

		std::size_t getFormulaCount() override;
		void addFormula(const std::string &, const std::string &, const std::map<std::string, std::string> &) override;
		bool removeFormula(std::size_t) override;

	 private:
		void addMathObject(CalxMathObject *);
		void removeMathObject(std::size_t);
		void OnListClick(wxCommandEvent &);
		void OnAddFormula(wxThreadEvent &);
		void OnRemoveFormula(wxThreadEvent &);

		wxPanel *mathObjectPanel;
		wxListBox *mathObjectList;
		wxPanel *mathObjectListPanel;
		std::vector<CalxMathObject *> contents;
	};
}  // namespace CalXUI

#endif