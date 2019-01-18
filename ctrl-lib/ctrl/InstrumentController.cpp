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

#include "ctrl-lib/ctrl/InstrumentController.h"
#include <algorithm>

namespace CalX {

	InstrumentController::InstrumentController(ConfigurationCatalogue &conf,
	                                           Instrument &instr)
	    : DeviceController::DeviceController(conf, instr), instr(instr) {
		this->state = true;
	}

	Instrument &InstrumentController::getInstrument() const {
		return this->instr;
	}

	bool InstrumentController::isSessionOpened() const {
		return SessionableResource::isSessionOpened();
	}

	ErrorCode InstrumentController::open_session() {
		if (!this->isSessionOpened()) {
			SessionableResource::open_session();
			ErrorCode errcode = this->instr.open_session() ? ErrorCode::NoError
			                                               : ErrorCode::LowLevelError;
			return errcode;
		} else {
			return ErrorCode::NoError;
		}
	}

	ErrorCode InstrumentController::close_session() {
		if (this->isSessionOpened()) {
			SessionableResource::close_session();
			if (this->isEnabled()) {
				this->enable(false);
			}
			ErrorCode errcode = this->instr.close_session()
			                        ? ErrorCode::NoError
			                        : ErrorCode::LowLevelError;
			return errcode;
		} else {
			return ErrorCode::NoError;
		}
	}

	bool InstrumentController::isEnabled() const {
		return this->instr.enabled();
	}

	ErrorCode InstrumentController::enable(bool e) {
		if ((e && this->state) != isEnabled()) {
			if (e && this->state && !this->isSessionOpened()) {
				ErrorCode errcode = this->open_session();
				if (errcode != ErrorCode::NoError) {
					return errcode;
				}
			}
			if (!this->instr.enable(e && this->state)) {
				return ErrorCode::LowLevelError;
			}
			sendStateChanged();
		}
		return ErrorCode::NoError;
	}

	ErrorCode InstrumentController::flipState() {
		return enable(!isEnabled());
	}

	bool InstrumentController::isRunnable() const {
		return this->state;
	}

	void InstrumentController::setRunnable(bool r) {
		this->state = r;
		if (!this->state) {
			enable(false);
		}
		sendStateChanged();
	}

	std::string InstrumentController::getInfo() const {
		return this->instr.getDeviceInfo();
	}

	InstrumentMode InstrumentController::getMode() const {
		return this->instr.getWorkingMode();
	}

	bool InstrumentController::setMode(InstrumentMode m) {
		return this->instr.setWorkingMode(m);
	}

	void InstrumentController::use() {
		UsableResource::use();
		this->submitEvent(&InstrumentEventListener::onUse);
	}

	void InstrumentController::unuse() {
		UsableResource::unuse();
		this->submitEvent(&InstrumentEventListener::onUnuse);
	}

	void InstrumentController::sendStateChanged() {
		this->submitEvent(&InstrumentEventListener::stateChanged,
		                  this->isRunnable(), this->isEnabled());
	}
}  // namespace CalX
