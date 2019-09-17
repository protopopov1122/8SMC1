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

#ifndef CALX_UI_CALX_ACTION_QUEUE_H_
#define CALX_UI_CALX_ACTION_QUEUE_H_

#include "calx/ui/calx.h"
#include <wx/event.h>

namespace CalX::UI {

	wxDECLARE_EVENT(wxEVT_COMMAND_QUEUE_UPDATE, wxThreadEvent);
}

#endif
