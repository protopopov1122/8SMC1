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

#ifndef CALX_UI_SCRIPT_CALX_SCRIPT_ENGINE_H_
#define CALX_UI_SCRIPT_CALX_SCRIPT_ENGINE_H_

#include <wx/thread.h>
#include "ui/calx.h"
#include "ui/script/ScriptEngine.h"
#include "ui/CalxApp.h"
#include "ui/dev/CalxDeviceHandle.h"
#include "ui/coord/CalxPlaneList.h"

namespace CalXUI {

	class CalXAppScriptMotor : public CalXScriptMotor {
	 public:
		CalXAppScriptMotor(CalxApp &, device_id_t);

		bool isValid() override;
		std::optional<Power> getPower() override;
		ErrorCode enablePower(bool) override;
		ErrorCode move(motor_coord_t, float) override;
		ErrorCode relativeMove(motor_coord_t, float) override;
		ErrorCode stop() override;
		std::optional<motor_coord_t> getPosition() override;
		ErrorCode moveToTrailer(TrailerId) override;
		ErrorCode checkTrailers() override;
		ErrorCode waitWhileRunning() override;

	 private:
		CalxApp &app;
		device_id_t motor_id;
	};

	class CalXAppScriptInstrument : public CalXScriptInstrument {
	 public:
		CalXAppScriptInstrument(CalxApp &, device_id_t);

		bool isValid() override;
		ErrorCode open_session() override;
		ErrorCode close_session() override;
		ErrorCode enable(bool) override;
		std::optional<bool> isEnabled() override;
		std::optional<bool> isRunnable() override;
		ErrorCode setRunnable(bool) override;
		std::optional<InstrumentMode> getMode() override;
		ErrorCode setMode(InstrumentMode) override;
		std::optional<bool> isSessionOpened() override;
		std::optional<std::string> getInfo() override;

	 private:
		CalxApp &app;
		device_id_t instrument_id;
	};

	class CalXAppScriptDevices : public CalXScriptDevices {
	 public:
		CalXAppScriptDevices(CalxApp &);

		device_id_t connectMotor(DeviceConnectionPrms *) override;
		device_id_t connectInstrument(DeviceConnectionPrms *) override;
		std::size_t getMotorCount() override;
		std::size_t getInstrumentCount() override;
		std::unique_ptr<CalXScriptMotor> getMotor(device_id_t) override;
		std::unique_ptr<CalXScriptInstrument> getInstrument(device_id_t) override;

	 private:
		CalxApp &app;
	};

	class CalXAppScriptPlane : public CalXScriptPlane {
	 public:
		CalXAppScriptPlane(CalxApp &, std::size_t);
		ErrorCode move(coord_point_t, double, bool, bool) override;
		ErrorCode arc(coord_point_t, coord_point_t, int, double, bool,
		              bool) override;
		ErrorCode calibrate(TrailerId) override;
		ErrorCode measure(TrailerId) override;
		ErrorCode move(coord_point_t, double) override;
		ErrorCode configure(coord_point_t, double) override;
		std::optional<coord_point_t> getPosition() override;
		std::pair<coord_rect_t, ErrorCode> getSize() override;
		std::optional<bool> isMeasured() override;
		bool positionAsCenter() override;

	 private:
		CalxApp &app;
		std::size_t plane_id;
	};

	class CalXAppScriptPlanes : public CalXScriptPlanes {
	 public:
		CalXAppScriptPlanes(CalxApp &);

		int32_t createPlane(device_id_t, device_id_t, device_id_t) override;
		std::size_t getPlaneCount() override;
		std::unique_ptr<CalXScriptPlane> getPlane(std::size_t) override;

	 private:
		CalxApp &app;
	};

	class CalXAppScriptUI : public CalXScriptUI {
	 public:
		CalXAppScriptUI(CalxApp &);
		ErrorCode openWatcher(std::size_t) override;

	 private:
		CalxApp &app;
	};

	class CalXAppScriptEnvironment : public CalXScriptUIEnvironment {
	 public:
		CalXAppScriptEnvironment(CalxApp &);

		CalXScriptDevices &getDevices() override;
		CalXScriptPlanes &getPlanes() override;
		CalXScriptUI &getUI() override;

	 private:
		CalxApp &app;
		CalXAppScriptDevices devices;
		CalXAppScriptPlanes planes;
		CalXAppScriptUI ui;
	};

	class CalXScriptHookThread : public wxThread {
	 public:
		CalXScriptHookThread(std::string, std::string);

	 protected:
		virtual wxThread::ExitCode Entry();

	 private:
		std::string path;
		std::string hook;
	};
}  // namespace CalXUI

#endif
