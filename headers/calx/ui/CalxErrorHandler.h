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

#ifndef CALX_UI_ERROR_HANDLER_H_
#define CALX_UI_ERROR_HANDLER_H_

#include "calx/ui/CalxApp.h"

namespace CalX::UI {

	class CalxErrorHandler {
	 public:
		CalxErrorHandler(SystemManager &);
		void handle(ErrorCode);
		void display(ErrorCode);
		void alert(const std::string &, const std::string &, long);

	 private:
		SystemManager &sysman;
	};
}  // namespace CalX::UI

#endif
