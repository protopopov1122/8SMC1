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

#include <exception>
#include "lua-calx/LuaScriptEngine.h"
#include <iostream>

namespace CalXLua {

	std::unique_ptr<CalXScript> LuaCalXScriptFactory::newScript(
	    CalXScriptUIEnvironment &env) {
		return std::make_unique<LuaCalXScript>(env);
	}

	LuaCalXScript::LuaCalXScript(CalXScriptUIEnvironment &env)
	    : CalXScript(env), lua(true), env(env) {
		this->initBindings();
	}

	bool LuaCalXScript::loadScript(const std::string &path) {
		try {
			return this->lua.load(path) == lcb::LuaStatusCode::Ok;
		} catch (CalXException &ex) {
			std::cout << "Caught CalX error " << static_cast<int>(ex.getErrorCode())
			          << std::endl;
			return false;
		} catch (...) {
			std::cout << "Caught error" << std::endl;
			return false;
		}
	}

	bool LuaCalXScript::executeScript(const std::string &code) {
		try {
			return this->lua.execute(code.c_str()) == lcb::LuaStatusCode::Ok;
		} catch (CalXException &ex) {
			std::cout << "Caught CalX error " << static_cast<int>(ex.getErrorCode())
			          << std::endl;
			return false;
		} catch (...) {
			std::cout << "Caught error" << std::endl;
			return false;
		}
	}

	bool LuaCalXScript::invokeFunction(const std::string &hook) {
		try {
			return this->lua[hook.c_str()]() != lcb::LuaStatusCode::Ok;
		} catch (CalXException &ex) {
			std::cout << "Caught CalX error " << static_cast<int>(ex.getErrorCode())
			          << std::endl;
			return false;
		} catch (...) {
			std::cout << "Caught error" << std::endl;
			return false;
		}
	}

	void LuaCalXScript::initBindings() {
		auto parity = lcb::LuaFactory::newTable(lua);
		parity["No"] = SerialPortParity::No;
		parity["Odd"] = SerialPortParity::Odd;
		parity["Even"] = SerialPortParity::Even;
		parity["Mark"] = SerialPortParity::Mark;
		parity["Space"] = SerialPortParity::Space;

		auto power = lcb::LuaFactory::newTable(lua);
		power["No"] = Power::NoPower;
		power["Half"] = Power::HalfPower;
		power["Full"] = Power::FullPower;

		auto trailer = lcb::LuaFactory::newTable(lua);
		trailer["Top"] = TrailerId::Trailer1;
		trailer["Bottom"] = TrailerId::Trailer2;

		auto mode = lcb::LuaFactory::newTable(lua);
		mode["Off"] = InstrumentMode::Off;
		mode["Prepare"] = InstrumentMode::Prepare;
		mode["Full"] = InstrumentMode::Full;

		lcb::ClassBinder<LuaCalXMotor>::bind(
		    lua, "id", &LuaCalXMotor::getDeviceID, "power", &LuaCalXMotor::getPower,
		    "enable", &LuaCalXMotor::enablePower, "move", &LuaCalXMotor::move,
		    "relMove", &LuaCalXMotor::relativeMove, "stop", &LuaCalXMotor::stop,
		    "position", &LuaCalXMotor::getPosition, "toTrailer",
		    &LuaCalXMotor::moveToTrailer, "checkTrailers",
		    &LuaCalXMotor::checkTrailers, "wait", &LuaCalXMotor::waitWhileRunning);

		lcb::ClassBinder<LuaCalXMotors>::bind(
		    lua, "get", &LuaCalXMotors::getMotor, "count", &LuaCalXMotors::getCount,
		    "connect", &LuaCalXMotors::connectSerialMotor, "power",
		    lcb::LuaFactory::wrap(lua, *power), "trailer",
		    lcb::LuaFactory::wrap(lua, *trailer));

		lcb::ClassBinder<LuaCalXInstrument>::bind(
		    lua, "id", &LuaCalXInstrument::getDeviceID, "open",
		    &LuaCalXInstrument::open_session, "close",
		    &LuaCalXInstrument::close_session, "opened",
		    &LuaCalXInstrument::isSessionOpened, "enable",
		    &LuaCalXInstrument::enable, "enabled", &LuaCalXInstrument::isEnabled,
		    "runnable", &LuaCalXInstrument::isRunnable, "setRunnable",
		    &LuaCalXInstrument::setRunnable, "mode", &LuaCalXInstrument::getMode,
		    "setMode", &LuaCalXInstrument::setMode, "info",
		    &LuaCalXInstrument::getInfo);

		lcb::ClassBinder<LuaCalXInstruments>::bind(
		    lua, "get", &LuaCalXInstruments::getInstrument, "count",
		    &LuaCalXInstruments::getCount, "connect",
		    &LuaCalXInstruments::connectSerialInstrument, "mode",
		    lcb::LuaFactory::wrap(lua, *mode));

		auto coordPoint = lcb::ClassBinder<coord_point_t>::bind(
		    lua, "x", &coord_point_t::x, "y", &coord_point_t::y, "new",
		    &lcb::LuaCppConstructor<coord_point_t, double, double>);

		auto coordRect = lcb::ClassBinder<coord_rect_t>::bind(
		    lua, "x", &coord_rect_t::x, "y", &coord_rect_t::y, "w",
		    &coord_rect_t::w, "h", &coord_rect_t::h, "new",
		    &lcb::LuaCppConstructor<coord_rect_t, double, double, double, double>);

		lcb::ClassBinder<LuaCalXPlane>::bind(
		    lua, "id", &LuaCalXPlane::getPlaneID, "move",
		    static_cast<ErrorCode (LuaCalXPlane::*)(coord_point_t, double, bool,
		                                            bool)>(&LuaCalXPlane::move),
		    "arc", &LuaCalXPlane::arc, "calibrate", &LuaCalXPlane::calibrate,
		    "measure", &LuaCalXPlane::measure, "fmove",
		    static_cast<ErrorCode (LuaCalXPlane::*)(coord_point_t, double)>(
		        &LuaCalXPlane::move),
		    "configure", &LuaCalXPlane::configure, "position",
		    &LuaCalXPlane::getPosition, "size", &LuaCalXPlane::getSize, "measured",
		    &LuaCalXPlane::isMeasured, "asCenter", &LuaCalXPlane::positionAsCenter,
		    "openWatcher", &LuaCalXPlane::openWatcher);

		lcb::ClassBinder<LuaCalXPlanes>::bind(
		    lua, "create", &LuaCalXPlanes::create, "count",
		    &LuaCalXPlanes::getPlaneCount, "get", &LuaCalXPlanes::getPlane);
		
		lcb::ClassBinder<LuaCalXTasks>::bind(
			lua,
			"count", &LuaCalXTasks::getTaskCount,
			"remove", &LuaCalXTasks::removeTask,
			"gcodeFile", &LuaCalXTasks::newGCodeFile
		);

		lcb::ClassBinder<ConfiguationFlatDictionary>::bind(
		    lua, "has", &ConfiguationFlatDictionary::has, "int",
		    &ConfiguationFlatDictionary::getInt, "float",
		    &ConfiguationFlatDictionary::getReal, "string",
		    &ConfiguationFlatDictionary::getString, "bool",
		    &ConfiguationFlatDictionary::getBool);

		lcb::ClassBinder<LuaCalXConfig>::bind(lua, "get", &LuaCalXConfig::getEntry,
		                                      "has", &LuaCalXConfig::hasEntry);

		lcb::ClassBinder<LuaCalXSettings>::bind(
		    lua, "exists", &LuaCalXSettings::exists, "has",
		    &LuaCalXSettings::hasEntry, "get", &LuaCalXSettings::getEntry);

		lcb::ClassBinder<LuaCalXMath>::bind(
		    lua, "count", &LuaCalXMath::getFormulaCount, "add",
		    &LuaCalXMath::addFormula, "remove", &LuaCalXMath::removeFormula);

		lcb::ClassBinder<LuaCalXJournal>::bind(
		    lua, "debug", &LuaCalXJournal::debug, "info", &LuaCalXJournal::info,
		    "warning", &LuaCalXJournal::warning, "error", &LuaCalXJournal::error,
		    "critical", &LuaCalXJournal::critical);

		this->lua["calx"] = *lcb::LuaFactory::newTable(lua);

		this->lua["calx"]["serial"] = *lcb::LuaFactory::newTable(lua);
		this->lua["calx"]["serial"]["parity"] = *parity;
		this->lua["calx"]["point"] = coordPoint;
		this->lua["calx"]["rect"] = coordRect;
		this->lua["calx"]["motors"] = std::make_unique<LuaCalXMotors>(this->env);
		this->lua["calx"]["instruments"] =
		    std::make_unique<LuaCalXInstruments>(this->env);
		this->lua["calx"]["planes"] = std::make_unique<LuaCalXPlanes>(this->env);
		this->lua["calx"]["tasks"] = std::make_unique<LuaCalXTasks>(this->env);
		this->lua["calx"]["config"] = std::make_unique<LuaCalXConfig>(this->env);
		this->lua["calx"]["settings"] =
		    std::make_unique<LuaCalXSettings>(this->env);
		this->lua["calx"]["math"] = std::make_unique<LuaCalXMath>(this->env);
		this->lua["calx"]["logger"] =
		    std::make_unique<LuaCalXJournal>(this->env.getLogger());
		this->lua["calx"]["journal"] =
		    std::make_unique<LuaCalXJournal>(this->env.getJournal());
	}
}  // namespace CalXLua
