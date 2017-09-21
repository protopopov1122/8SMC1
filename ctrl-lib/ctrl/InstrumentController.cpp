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

#include "ctrl-lib/ctrl/InstrumentController.h"
#include <algorithm>

namespace CalX {

	InstrumentController::InstrumentController(Instrument *instr) {
		this->instr = instr;
		this->state = true;
		this->session_state = false;
	}

	Instrument *InstrumentController::getInstrument() {
		return this->instr;
	}

	device_id_t InstrumentController::getID() {
		return this->instr->getID();
	}

	std::shared_ptr<ConfigManager> InstrumentController::getConfiguration() {
		return this->instr->getConfiguration();
	}

	bool InstrumentController::isSessionOpened() {
		return this->session_state;
	}

	ErrorCode InstrumentController::open_session() {
		if (!this->session_state) {
			ErrorCode errcode = this->instr->open_session()
			                        ? ErrorCode::NoError
			                        : ErrorCode::LowLevelError;
			if (errcode == ErrorCode::NoError) {
				this->session_state = true;
			}
			return errcode;
		} else {
			return ErrorCode::NoError;
		}
	}

	ErrorCode InstrumentController::close_session() {
		if (this->session_state) {
			if (this->isEnabled()) {
				this->enable(false);
			}
			ErrorCode errcode = this->instr->close_session()
			                        ? ErrorCode::NoError
			                        : ErrorCode::LowLevelError;
			if (errcode == ErrorCode::NoError) {
				this->session_state = false;
			}
			return errcode;
		} else {
			return ErrorCode::NoError;
		}
	}

	bool InstrumentController::isEnabled() {
		return this->instr->enabled();
	}

	ErrorCode InstrumentController::enable(bool e) {
		if ((e && this->state) != isEnabled()) {
			if (e && this->state && !this->session_state) {
				ErrorCode errcode = this->open_session();
				if (errcode != ErrorCode::NoError) {
					return errcode;
				}
			}
			if (!this->instr->enable(e && this->state)) {
				return ErrorCode::LowLevelError;
			}
			sendStateChanged();
		}
		return ErrorCode::NoError;
	}

	ErrorCode InstrumentController::flipState() {
		return enable(!isEnabled());
	}

	bool InstrumentController::isRunnable() {
		return this->state;
	}

	void InstrumentController::setRunnable(bool r) {
		this->state = r;
		if (!this->state) {
			enable(false);
		}
		sendStateChanged();
	}

	std::string InstrumentController::getInfo() {
		return this->instr->getDeviceInfo();
	}

	InstrumentMode InstrumentController::getMode() {
		return this->instr->getWorkingMode();
	}

	bool InstrumentController::setMode(InstrumentMode m) {
		return this->instr->setWorkingMode(m);
	}

	void InstrumentController::addEventListener(
	    std::shared_ptr<InstrumentEventListener> l) {
		this->listeners.push_back(l);
	}

	void InstrumentController::removeEventListener(
	    std::shared_ptr<InstrumentEventListener> l) {
		this->listeners.erase(
		    std::remove(this->listeners.begin(), this->listeners.end(), l),
		    this->listeners.end());
	}

	void InstrumentController::use() {
		for (const auto &l : this->listeners) {
			l->use();
		}
	}

	void InstrumentController::unuse() {
		for (const auto &l : this->listeners) {
			l->unuse();
		}
	}

	void InstrumentController::sendStateChanged() {
		for (const auto &l : this->listeners) {
			l->stateChanged(this->isRunnable(), this->isEnabled());
		}
	}
}  // namespace CalX
