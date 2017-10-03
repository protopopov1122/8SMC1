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

#ifndef CALX_UI_SCRIPT_CALX_SCRIPT_ENGINE_H_
#define CALX_UI_SCRIPT_CALX_SCRIPT_ENGINE_H_

#include <wx/thread.h>
#include "ctrl-lib/script/ScriptEngine.h"
#include "ui/CalxApp.h"
#include "ui/dev/CalxDeviceHandle.h"
#include "ui/coord/CalxPlaneList.h"

namespace CalXUI {

	class CalXAppScriptEnvironment : public CalXScriptEnvironment {
	 public:
		CalXAppScriptEnvironment(CalxApp &);

		virtual bool connectSerialMotor(uint8_t, uint32_t, uint8_t);
		virtual bool connectSerialInstrument(uint8_t, uint32_t, uint8_t);
		virtual size_t getMotorCount();
		virtual size_t getInstrumentCount();
		virtual std::pair<Power, ErrorCode> getMotorPower(device_id_t);
		virtual ErrorCode enableMotorPower(device_id_t, bool);
		virtual ErrorCode motorMove(device_id_t, motor_coord_t, float);
		virtual ErrorCode motorRelativeMove(device_id_t, motor_coord_t, float);
		virtual ErrorCode motorStop(device_id_t);
		virtual std::pair<motor_coord_t, ErrorCode> getMotorPosition(device_id_t);
		virtual ErrorCode motorMoveToTrailer(device_id_t, TrailerId);
		virtual std::pair<bool, ErrorCode> motorCheckTrailers(device_id_t);
		virtual ErrorCode motorWaitWhileRunning(device_id_t);

		virtual ErrorCode instrumentOpenSession(device_id_t);
		virtual ErrorCode instrumentCloseSession(device_id_t);
		virtual ErrorCode instrumentEnable(device_id_t, bool);
		virtual std::pair<bool, ErrorCode> instrumentIsEnabled(device_id_t);
		virtual std::pair<bool, ErrorCode> instrumentIsRunnable(device_id_t);
		virtual ErrorCode instrumentSetRunnable(device_id_t, bool);
		virtual std::pair<InstrumentMode, ErrorCode> instrumentGetMode(device_id_t);
		virtual std::pair<bool, ErrorCode> instrumentSetMode(device_id_t,
		                                                     InstrumentMode);
		virtual std::pair<bool, ErrorCode> instrumentIsSessionOpened(device_id_t);
		virtual std::pair<std::string, ErrorCode> instrumentGetInfo(device_id_t);

		virtual bool createCoordPlane(device_id_t, device_id_t, device_id_t);
		virtual ErrorCode planeMove(size_t, coord_point_t, double, bool, bool);
		virtual ErrorCode planeArc(size_t, coord_point_t, coord_point_t, int,
		                           double, bool, bool);
		virtual ErrorCode planeCalibrate(size_t, TrailerId);
		virtual ErrorCode planeMeasure(size_t, TrailerId);
		virtual ErrorCode planeMove(size_t, coord_point_t, double);
		virtual ErrorCode planeConfigure(size_t, coord_point_t, double);

	 private:
		CalxApp &app;
	};

	class CalXScriptHookThread : public wxThread {
	 public:
		CalXScriptHookThread(std::string);

	 protected:
		virtual wxThread::ExitCode Entry();

	 private:
		std::string hook;
	};
}  // namespace CalXUI

#endif
