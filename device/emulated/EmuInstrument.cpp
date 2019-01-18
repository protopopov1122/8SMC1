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

#include "device/emulated/EmuInstrument.h"
#include "device/emulated/EmuDeviceManager.h"

namespace CalX {

	EmuInstrument::EmuInstrument(device_id_t id, EmuDeviceManager &devman)
	    : Instrument::Instrument(id), devman(devman) {
		this->en = false;
		this->mode = InstrumentMode::Off;
	}

	DeviceManager &EmuInstrument::getDeviceManager() {
		return this->devman;
	}

	std::string EmuInstrument::getDeviceInfo() {
		return "";
	}

	std::string EmuInstrument::getRuntimeInfo() {
		return "";
	}

	bool EmuInstrument::open_session() {
		return true;
	}

	bool EmuInstrument::close_session() {
		return true;
	}

	bool EmuInstrument::enable(bool e) {
		this->en = e;
		return true;
	}

	bool EmuInstrument::enabled() {
		return this->en;
	}

	bool EmuInstrument::setWorkingMode(InstrumentMode m) {
		this->mode = m;
		return true;
	}

	InstrumentMode EmuInstrument::getWorkingMode() {
		return this->mode;
	}

	void EmuInstrument::terminate() {}
}  // namespace CalX
