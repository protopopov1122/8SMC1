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

#ifndef CALX_UI_TASK_CALX_GCODE_TASK_H_
#define CALX_UI_TASK_CALX_GCODE_TASK_H_

#include "ui/task/CalxTaskFactory.h"

namespace CalXUI {

  class CalxGCodeTaskFactory : public CalxTaskFactory {
   public:
	virtual CalxTaskHandle *newTask(wxWindow*);
  };
	
	

  class CalxGcodeHandle : public CalxTaskHandle {
   public:
	CalxGcodeHandle(wxWindow *, wxWindowID, std::string, std::istream *,
					ComplexCoordTranslator *);
	virtual CoordTask *getTask();
	virtual void update();
	virtual ComplexCoordTranslator *getTranslator();

   private:
	void OnExit(wxCloseEvent &);

	std::string id;
	CalxCoordFilterCtrl *filter;
	ComplexCoordTranslator *translator;

	CoordTask *task;
  };
}

#endif