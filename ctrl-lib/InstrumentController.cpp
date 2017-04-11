/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CalX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <algorithm>
#include "InstrumentController.h"

namespace CalX {
	
	InstrumentController::InstrumentController(Instrument *instr) {
		this->instr = instr;
		this->state = true;
		INIT_LOG("InstrumentController");
	}
	
	InstrumentController::~InstrumentController() {
		DESTROY_LOG("InstrumentController");
	}
	
	Instrument *InstrumentController::getInstrument() {
		return this->instr;
	}
	
	device_id_t InstrumentController::getID() {
		return this->instr->getID();
	}
	
	ConfigManager *InstrumentController::getConfiguration() {
		return this->instr->getConfiguration();
	}
	
	bool InstrumentController::isEnabled() {
		return this->instr->enabled();
	}
	
	ErrorCode InstrumentController::enable(bool e) {
		if ((e && this->state) != isEnabled()) {
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
		return this->instr->getInfo();
	}
	
	InstrumentMode InstrumentController::getMode() {
		return this->instr->getMode();
	}
	
	bool InstrumentController::setMode(InstrumentMode m) {
		return this->instr->setMode(m);
	}
	
	void InstrumentController::addEventListener(InstrumentEventListener *l) {
		this->listeners.push_back(l);
	}
	
	void InstrumentController::removeEventListener(InstrumentEventListener *l) {
		this->listeners.erase(
			std::remove(this->listeners.begin(), this->listeners.end(),
				l), this->listeners.end());
		delete l;
	}
	
	void InstrumentController::use() {
		for (const auto& l : this->listeners) {
			l->use();
		}
	}
	
	void InstrumentController::unuse() {
		for (const auto& l : this->listeners) {
			l->unuse();
		}
	}
	
	void InstrumentController::sendStateChanged() {
		for (const auto& l : this->listeners) {
			l->stateChanged(this->isRunnable(), this->isEnabled());
		}
	}
}