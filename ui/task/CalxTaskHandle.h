#ifndef CALX_UI_CALX_TASK_HANDLE_H_
#define CALX_UI_CALX_TASK_HANDLE_H_

#include <iostream>
#include "CalxApp.h"

using namespace CalX;

namespace CalXUI {
	
	enum CalxTaskType {
		CalxGcode
	};
	
	class CalxTaskHandle : public wxPanel {
		public:
			CalxTaskHandle(wxWindow *win, wxWindowID id, CalxTaskType type)
				: wxPanel::wxPanel(win, id) {
				this->type = type;
			}
			CalxTaskType getType() {return this->type;}
			virtual CoordTask *getTask() = 0;
		private:
			CalxTaskType type;
	};
	
	class CalxGcodeHandle : public CalxTaskHandle {
		public:
			CalxGcodeHandle(wxWindow*, wxWindowID, std::string, std::istream*, CoordTranslator*);
			virtual CoordTask *getTask();
			std::string getId() {
				return this->id;
			}
		private:
			void OnExit(wxCloseEvent&);
		
			std::string id;
			CoordTranslator *translator;
			CoordTask *task;
	};
}

#endif