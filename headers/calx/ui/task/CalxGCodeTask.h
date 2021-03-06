/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_UI_TASK_CALX_GCODE_TASK_H_
#define CALX_UI_TASK_CALX_GCODE_TASK_H_

#include "calx/ui/task/CalxTaskFactory.h"

namespace CalX::UI {

	class CalxGCodeTaskFactory : public CalxTaskFactory {
	 public:
		CalxTaskHandle *newTask(wxWindow *) override;
	};

	class CalxGcodeHandle : public CalxTaskHandle {
	 public:
		CalxGcodeHandle(wxWindow *, wxWindowID, const std::string &, std::istream &,
		                std::shared_ptr<ComplexCoordTranslator>);
		std::string getName() const override;
		std::shared_ptr<CoordTask> getTask() override;
		void update() override;
		std::shared_ptr<CoordTranslator> getTranslator() override;

	 protected:
		void dump(std::ostream &) const override;

	 private:
		void OnExit(wxCloseEvent &);

		std::string id;
		CalxCoordFilterCtrl *filter;
		std::shared_ptr<ComplexCoordTranslator> translator;

		std::shared_ptr<GCodeCoordTask> task;
	};
}  // namespace CalX::UI

#endif