#ifndef _8SMC1_UI_CALX_TASK_PANEL_H_
#define _8SMC1_UI_CALX_TASK_PANEL_H_

#include <vector>
#include <wx/listbox.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include "CalxApp.h"
#include "CalxTaskHandle.h"
#include "CalxActionQueue.h"

namespace CalX {
	
	class CalxTaskHandle; // Forward referencing
	
	class CalxTaskPanel : public wxPanel {
		public:
			CalxTaskPanel(wxWindow*, wxWindowID);
			
			void updateUI();
		private:
			void OnExit(wxCloseEvent&);
			void OnNewGcodeClick(wxCommandEvent&);
			void OnRemoveClick(wxCommandEvent&);
			void OnListClick(wxCommandEvent&);
			void OnBuildClick(wxCommandEvent&);
			void OnQueueUpdate(wxThreadEvent&);
			
			wxListBox *taskList;
			wxPanel *mainPanel;
			std::vector<CalxTaskHandle*> list;
			wxPanel *execPanel;
			wxChoice *plane;
			wxSpinCtrl *speed;
			CalxActionQueue *queue;
	};
}

#endif