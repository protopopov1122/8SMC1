#ifndef _8SMC1_UI_CALX_COORD_PANEL_H_
#define _8SMC1_UI_CALX_COORD_PANEL_H_

#include "calx.h"
#include "CalxApp.h"
#include "CalxCoordCtrl.h"
#include <cinttypes>
#include <vector>
#include <wx/listbox.h>

namespace CalX {
	
	class CalxCoordPanel : public wxPanel {
		public:
			CalxCoordPanel(wxWindow*, wxWindowID);
			
			void updateUI();
		private:
			void addPlane(CoordHandle*);
			void removePlane(size_t);
		
			void OnExit(wxCloseEvent&);
			void OnAddButtonClick(wxCommandEvent&);
			void OnRemoveButtonClick(wxCommandEvent&);
			void OnListClick(wxCommandEvent&);
			
			wxListBox *coordList;
			std::vector<CalxCoordCtrl*> coords;
			wxPanel *mainPanel;
			int nextId;
	};
}

#endif