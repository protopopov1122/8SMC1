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

#ifndef CALX_CTRL_LIB_EXT_ENGINE_H_
#define CALX_CTRL_LIB_EXT_ENGINE_H_

#include "calx/ctrl-lib/CtrlCore.h"

/* CalX supports basic extensions. Extensions are attached to System Manager
   and System Manager calls extension object on most significant events. */

namespace CalX {

	class MotorController;       // Forward referencing
	class InstrumentController;  // Forward referencing
	class CoordTask;             // Forward referencing
	class CoordHandle;           // Forward referencing
	class SystemManager;         // Forward referencing

	class ExtEngine {
	 public:
		virtual ~ExtEngine() = default;
		virtual bool init(SystemManager &) = 0;
		virtual bool destroy() = 0;

		virtual void onTaskAdded(std::shared_ptr<CoordTask> task) {}
		virtual void onTaskRemoving(std::size_t id) {}
		virtual void onPlaneAdded(std::shared_ptr<CoordHandle> handle) {}
		virtual void onPlaneRemoving(std::size_t id) {}
		virtual void onMotorConnected(std::shared_ptr<MotorController> ctrl) {}
		virtual void onInstrumentConnected(
		    std::shared_ptr<InstrumentController> ctrl) {}
	};

	typedef ExtEngine *(*ExtEngine_getter)();
	extern "C" LIBEXPORT ExtEngine *getExtEngine();
}  // namespace CalX

#endif
