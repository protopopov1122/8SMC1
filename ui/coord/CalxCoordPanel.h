#ifndef _8SMC1_UI_CALX_COORD_PANEL_H_
#define _8SMC1_UI_CALX_COORD_PANEL_H_

#include <cinttypes>
#include <vector>
#include <wx/listbox.h>
#include "CalxApp.h"
#include "CalxCoordCtrl.h"

namespace CalX {
	
	class CalxCoordPanel : public wxPanel {
		public:
			CalxCoordPanel(wxWindow*, wxWindowID);
			
			size_t getCoordCount() {return coords.size();}
			CoordHandle *getCoord(size_t sz) {return sz < coords.size() ? coords.at(sz)->getHandle() : nullptr;}
			void updateUI();
			void stop();
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