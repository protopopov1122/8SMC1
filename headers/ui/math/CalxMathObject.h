#ifndef CALX_UI_MATH_CALX_MATH_OBJECT_H_
#define CALX_UI_MATH_CALX_MATH_OBJECT_H_

#include "ui/CalxApp.h"
#include "ctrl-lib/graph/Formula.h"

namespace CalXUI {

  class CalxMathObject : public wxPanel {
   public:
    using wxPanel::wxPanel;
    virtual const std::string &getTitle() const = 0;
  };

  class CalxMathFormulaPanel : public CalxMathObject {
   public:
    CalxMathFormulaPanel(wxWindow *, wxWindowID, const std::string &, const std::string &, const std::map<std::string, std::string> &);
    const std::string &getTitle() const override;
   private:
    void OnCalculateClick(wxCommandEvent &);

    std::string title;
    std::string formulaText;
    std::unique_ptr<MathFormula> formula;

    wxStaticText *resultText;
    std::map<std::string, double> variableValues;
  };
}

#endif