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

#ifndef CALX_CTRL_LIB_SCRIPT_SCRIPT_ENGINE_H_
#define CALX_CTRL_LIB_SCRIPT_SCRIPT_ENGINE_H_

#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/conf/ConfigManager.h"

using namespace CalX;

namespace CalXUI {

	class CalXScriptEnvironment {
	 public:
		CalXScriptEnvironment(std::shared_ptr<ConfigManager>);
		virtual ~CalXScriptEnvironment() = default;

		std::shared_ptr<ConfigManager> getConfiguration();

		virtual bool connectSerialMotor(uint8_t, uint32_t, uint8_t) = 0;
		virtual bool connectSerialInstrument(uint8_t, uint32_t, uint8_t) = 0;
		virtual size_t getMotorCount() = 0;
		virtual size_t getInstrumentCount() = 0;
		virtual Power getMotorPower(device_id_t) = 0;
		virtual ErrorCode enableMotorPower(device_id_t, bool) = 0;
		virtual ErrorCode motorMove(device_id_t, motor_coord_t, float) = 0;
		virtual ErrorCode motorRelativeMove(device_id_t, motor_coord_t, float) = 0;
		virtual ErrorCode motorStop(device_id_t) = 0;
		virtual motor_coord_t getMotorPosition(device_id_t) = 0;
		virtual ErrorCode motorMoveToTrailer(device_id_t, TrailerId) = 0;
		virtual bool motorCheckTrailers(device_id_t) = 0;
		virtual ErrorCode motorWaitWhileRunning(device_id_t) = 0;

	 private:
		std::shared_ptr<ConfigManager> config;
	};

	class CalXScript {
	 public:
		CalXScript(CalXScriptEnvironment &);
		virtual ~CalXScript() = default;

		virtual void call(std::string) = 0;

	 protected:
		CalXScriptEnvironment &env;
	};

	typedef CalXScript *(*ScriptEngine_getter)(CalXScriptEnvironment &);
	extern "C" LIBEXPORT CalXScript *getScriptEngine(CalXScriptEnvironment &);

}  // namespace CalXUI

#endif
