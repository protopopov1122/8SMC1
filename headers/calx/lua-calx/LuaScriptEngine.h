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

#ifndef CALX_LUA_CALX_LUA_SCRIPT_ENGINE_H_
#define CALX_LUA_CALX_LUA_SCRIPT_ENGINE_H_

#include "calx/lua-calx/LuaCalXEnvironment.h"

namespace CalX::UI::Lua {

	class LuaCalXScript : public CalXScript {
	 public:
		LuaCalXScript(CalXScriptUIEnvironment &);

		bool loadScript(const std::string &) override;
		bool executeScript(const std::string &) override;
		bool invokeFunction(const std::string &) override;

	 private:
		void initBindings();
		void loadLibrary();

		lcb::LuaEnvironment lua;
		CalXScriptUIEnvironment &env;
	};

	class LuaCalXScriptFactory : public CalXScriptUIFactory {
	 public:
		std::unique_ptr<CalXScript> newScript(CalXScriptUIEnvironment &) override;
	};
}  // namespace CalX::UI::Lua

#endif
