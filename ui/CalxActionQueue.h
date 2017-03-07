#ifndef _8SMC1_UI_CALX_ACTION_QUEUE_H_
#define _8SMC1_UI_CALX_ACTION_QUEUE_H_

#include <vector>
#include <wx/thread.h>
#include <wx/event.h>
#include "calx.h"

using namespace _8SMC1;

wxDECLARE_EVENT(wxEVT_COMMAND_QUEUE_UPDATE, wxThreadEvent);

namespace CalX {
	
		class CalxAction {
			public:
				virtual ~CalxAction() {};
				virtual void perform(SystemManager*) = 0;
				virtual void stop() = 0;
		};
		
		class CalxActionQueue : public wxThread {
			public:
				CalxActionQueue(SystemManager*, wxEvtHandler*);
				virtual ~CalxActionQueue();
				void addAction(CalxAction*);
				bool isEmpty();
				void stop();
				void stopCurrent();
			protected:
				virtual ExitCode Entry();
			private:
				wxEvtHandler *handle;
				wxMutex *mutex;
				wxCondition *cond;
				wxMutex condMutex;
				std::vector<CalxAction*> queue;
				CalxAction *current;
				bool work;
				SystemManager *sysman;
		};
}

#endif