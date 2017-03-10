#ifndef CALX_UI_CALX_TASK_PANEL_H_
#define CALX_UI_CALX_TASK_PANEL_H_

#include <vector>
#include <wx/listbox.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include "CalxApp.h"
#include "CalxTaskHandle.h"
#include "CalxActionQueue.h"

namespace CalXUI {
	
	class CalxTaskHandle; // Forward referencing
	
	class CalxTaskPanel : public wxPanel {
		public:
			CalxTaskPanel(wxWindow*, wxWindowID);
			
			void updateUI();
			void stop();
			void setEnabled(bool);
		private:
			void OnExit(wxCloseEvent&);
			void OnNewGcodeClick(wxCommandEvent&);
			void OnRemoveClick(wxCommandEvent&);
			void OnListClick(wxCommandEvent&);
			void OnBuildClick(wxCommandEvent&);
			void OnQueueUpdate(wxThreadEvent&);
			void OnStopClick(wxCommandEvent&);
			
			wxListBox *taskList;
			wxPanel *mainPanel;
			wxPanel *taskPanel;
			std::vector<CalxTaskHandle*> list;
			wxPanel *execPanel;
			wxButton *stopButton;
			wxChoice *plane;
			wxSpinCtrl *speed;
			CalxActionQueue *queue;
	};
}

#endif