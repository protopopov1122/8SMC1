/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


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