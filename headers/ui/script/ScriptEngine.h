/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#ifndef CALX_UI_SCRIPT_SCRIPT_ENGINE_H_
#define CALX_UI_SCRIPT_SCRIPT_ENGINE_H_

#include "ctrl-lib/script/ScriptEngine.h"

namespace CalXUI {

	class CalXScriptUI {
	 public:
		virtual ~CalXScriptUI() = default;
		virtual CalX::ErrorCode openWatcher(std::size_t) = 0;
	};

	class CalXScriptUIEnvironment : public CalX::CalXScriptEnvironment {
	 public:
		using CalX::CalXScriptEnvironment::CalXScriptEnvironment;
		virtual CalXScriptUI &getUI() = 0;
	};

	class CalXScriptUIFactory {
	 public:
		virtual ~CalXScriptUIFactory() = default;
		virtual std::unique_ptr<CalX::CalXScript> newScript(CalXScriptUIEnvironment &) = 0;
	};

	typedef CalXScriptUIFactory *(*ScriptUIEngine_getter)();
	extern "C" LIBEXPORT CalXScriptUIFactory *getScriptUIEngine();
}  // namespace CalXUI

#endif