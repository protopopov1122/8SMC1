#ifndef CALX_UI_MATH_CALX_MATH_PANEL_H_
#define CALX_UI_MATH_CALX_MATH_PANEL_H_

#include "ui/CalxApp.h"
#include "ui/CalxPanelPane.h"
#include "ui/math/CalxMathObject.h"

namespace CalXUI {

  class CalxMathPanel : public CalxPanelPane {
   public:
    CalxMathPanel(wxWindow *, wxWindowID);
		bool isBusy() override;
		void shutdown() override;
		void updateUI() override;
   private:
    void addMathObject(CalxMathObject *);
    void removeMathObject(std::size_t);
    void OnListClick(wxCommandEvent &);

    wxPanel *mathObjectPanel;
    wxListBox *mathObjectList;
    std::vector<CalxMathObject *> contents;
  };
}

#endif