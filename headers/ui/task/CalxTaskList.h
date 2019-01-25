/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_UI_TASK_CALX_TASK_LIST_H_
#define CALX_UI_TASK_CALX_TASK_LIST_H_

#include "ui/CalxApp.h"
#include "ui/task/CalxTaskFactory.h"

namespace CalXUI {

	class CalxTaskList {
	 public:
		virtual ~CalxTaskList() = default;
		virtual void stop() = 0;
                virtual std::size_t getTaskCount() = 0;
                virtual void removeTask(std::size_t) = 0;
                virtual void attachTask(const std::string &, std::shared_ptr<CalxTaskFactory>) = 0;
	};
}  // namespace CalXUI

#endif