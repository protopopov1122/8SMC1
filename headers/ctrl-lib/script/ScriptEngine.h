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

#ifndef CALX_CTRL_LIB_SCRIPT_SCRIPT_ENGINE_H_
#define CALX_CTRL_LIB_SCRIPT_SCRIPT_ENGINE_H_

#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/conf/Dictionary.h"
#include <optional>

/* Script engine is used to integrate different scripting languages into the
   system, it consists of several abstract objects:
     * Script environment - adapter between system manager and scripting
   language engine, converts hierarchy of real objects into plain interface that
   uses only scalars and pairs of them. It simplifies script-system interaction
     * Script object - represents scripting engine instance (its scope,
   environment, etc.) with a couple of common methods
     * Script factory - object used to produce scripts in two "modes" - script
   from file, script shell (allowing simple interactive step-by-step script
   execution)
*/

namespace CalX {

	class CalXScriptMotor {
	 public:
		virtual ~CalXScriptMotor() = default;
		virtual bool isValid() = 0;
		virtual std::optional<Power> getPower() = 0;
		virtual ErrorCode enablePower(bool) = 0;
		virtual ErrorCode move(motor_coord_t, float) = 0;
		virtual ErrorCode relativeMove(motor_coord_t, float) = 0;
		virtual ErrorCode stop() = 0;
		virtual std::optional<motor_coord_t> getPosition() = 0;
		virtual ErrorCode moveToTrailer(TrailerId) = 0;
		virtual ErrorCode checkTrailers() = 0;
		virtual ErrorCode waitWhileRunning() = 0;
	};

	class CalXScriptInstrument {
	 public:
		virtual ~CalXScriptInstrument() = default;
		virtual bool isValid() = 0;
		virtual ErrorCode open_session() = 0;
		virtual ErrorCode close_session() = 0;
		virtual ErrorCode enable(bool) = 0;
		virtual std::optional<bool> isEnabled() = 0;
		virtual std::optional<bool> isRunnable() = 0;
		virtual ErrorCode setRunnable(bool) = 0;
		virtual std::optional<InstrumentMode> getMode() = 0;
		virtual ErrorCode setMode(InstrumentMode) = 0;
		virtual std::optional<bool> isSessionOpened() = 0;
		virtual std::optional<std::string> getInfo() = 0;
	};

	class CalXScriptDevices {
	 public:
		virtual ~CalXScriptDevices() = default;
		virtual device_id_t connectMotor(DeviceConnectionPrms *) = 0;
		virtual device_id_t connectInstrument(DeviceConnectionPrms *) = 0;
		virtual std::size_t getMotorCount() = 0;
		virtual std::size_t getInstrumentCount() = 0;
		virtual std::unique_ptr<CalXScriptMotor> getMotor(device_id_t) = 0;
		virtual std::unique_ptr<CalXScriptInstrument> getInstrument(
		    device_id_t) = 0;
	};

	class CalXScriptPlane {
	 public:
		virtual ~CalXScriptPlane() = default;
		virtual ErrorCode move(coord_point_t, double, bool, bool) = 0;
		virtual ErrorCode arc(coord_point_t, coord_point_t, int, double, bool,
		                      bool) = 0;
		virtual ErrorCode calibrate(TrailerId) = 0;
		virtual ErrorCode measure(TrailerId) = 0;
		virtual ErrorCode move(coord_point_t, double) = 0;
		virtual ErrorCode configure(coord_point_t, double) = 0;
		virtual std::optional<coord_point_t> getPosition() = 0;
		virtual std::pair<coord_rect_t, ErrorCode> getSize() = 0;
		virtual std::optional<bool> isMeasured() = 0;
		virtual bool positionAsCenter() = 0;
	};

	class CalXScriptPlanes {
	 public:
		virtual ~CalXScriptPlanes() = default;
		virtual int32_t createPlane(device_id_t, device_id_t, device_id_t = -1) = 0;
		virtual std::size_t getPlaneCount() = 0;
		virtual std::unique_ptr<CalXScriptPlane> getPlane(std::size_t) = 0;
	};

	class CalXScriptEnvironment {
	 public:
		CalXScriptEnvironment(ConfigurationCatalogue &, SettingsRepository *);
		virtual ~CalXScriptEnvironment() = default;

		ConfigurationCatalogue &getConfiguration();
		SettingsRepository *getSettings();

		virtual CalXScriptDevices &getDevices() = 0;
		virtual CalXScriptPlanes &getPlanes() = 0;
		virtual JournalLogger &getLogger() = 0;

	 private:
		ConfigurationCatalogue &config;
		SettingsRepository *settings;
	};

	class CalXScript {
	 public:
		CalXScript(CalXScriptEnvironment &);
		virtual ~CalXScript() = default;

		virtual bool loadScript(const std::string &) = 0;
		virtual bool invokeFunction(const std::string &) = 0;
		virtual bool executeScript(const std::string &) = 0;

	 protected:
		CalXScriptEnvironment &env;
	};

	class CalXScriptFactory {
	 public:
		virtual ~CalXScriptFactory() = default;
		virtual std::unique_ptr<CalXScript> newScript(CalXScriptEnvironment &) = 0;
	};

	typedef CalXScriptFactory *(*ScriptEngine_getter)();
	extern "C" LIBEXPORT CalXScriptFactory *getScriptEngine();

}  // namespace CalX

#endif
