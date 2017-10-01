/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_LUA_CALX_LUA_CALX_ENVIRONMENT_H_
#define CALX_LUA_CALX_LUA_CALX_ENVIRONMENT_H_

#include "ctrl-lib/script/ScriptEngine.h"
#include <exception>

using namespace CalX;
using namespace CalXUI;

namespace CalXLua {

	class CalXException : public std::exception {
	 public:
		CalXException(ErrorCode);
		ErrorCode getErrorCode();
		virtual const char *what() const throw();

	 private:
		ErrorCode errcode;
	};

	class LuaCalXEnvironment {
	 public:
		LuaCalXEnvironment(CalXScriptEnvironment &);

		bool connectSerialMotor(int, int, int);
		bool connectSerialInstrument(int, int, int);
		int getMotorCount();
		int getInstrumentCount();
		int getMotorPower(int);
		int enableMotorPower(int, bool);
		int motorMove(int, int, double);
		int motorRelativeMove(int, int, double);
		int motorStop(int);
		int motorPosition(int);
		int motorMoveToTrailer(int, int);
		bool motorCheckTrailers(int);
		int motorWaitWhileRunning(int);

		int instrumentOpenSession(int);
		int instrumentCloseSession(int);
		int instrumentEnable(int, bool);
		bool instrumentIsEnabled(int);
		int instrumentSetRunnable(int, bool);
		bool instrumentIsRunnable(int);
		int instrumentGetMode(int);
		bool instrumentSetMode(int, int);
		bool instrumentIsSessionOpened(int);
		std::string instrumentGetInfo(int);

	 private:
		CalXScriptEnvironment &env;
	};
}  // namespace CalXLua

#endif
