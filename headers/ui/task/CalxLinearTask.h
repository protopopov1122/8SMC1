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

#include "ui/task/CalxTaskFactory.h"

namespace CalXUI {

  class CalxLinearTaskFactory : public CalxTaskFactory {
   public:
	virtual CalxTaskHandle *newTask(wxWindow *);

   private:
	size_t next_id = 0;
  };

  class CalxLinearTaskHandle : public CalxTaskHandle {
   public:
	CalxLinearTaskHandle(wxWindow *, wxWindowID, size_t);
	virtual std::string getName();
	virtual CoordTask *getTask();
	virtual CoordTranslator *getTranslator();
	virtual void update();
	void setRectangle(coord_rect_t);

   private:
	void OnFieldChange(wxCommandEvent &);
	void OnExit(wxCloseEvent &);
	LinearCoordTask *task;
	size_t id;
	CoordTranslator *trans;
	wxSpinCtrlDouble *xCoord;
	wxSpinCtrlDouble *yCoord;
	wxSpinCtrlDouble *wDim;
	wxSpinCtrlDouble *hDim;
	wxSpinCtrlDouble *spacing;
	wxCheckBox *vertical;
  };
}