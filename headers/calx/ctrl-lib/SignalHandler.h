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

#ifndef CALX_CTRL_LIB_SIGNAL_HANDLER_H_
#define CALX_CTRL_LIB_SIGNAL_HANDLER_H_

#include "calx/ctrl-lib/CtrlCore.h"
#include "calx/ctrl-lib/SystemManager.h"

/* This header contains OS-oriented signal
   hanlers that catch fatal errors and try
   to prevent hardware damage. */

extern CalX::SystemManager *SYSMAN;

extern "C" void calx_terminate();

namespace CalX {

	void setup_signals(SystemManager &);
}

#endif
