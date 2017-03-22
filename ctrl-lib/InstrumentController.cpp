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


#include "InstrumentController.h"

namespace CalX {
	
	InstrumentController::InstrumentController(Instrument *instr) {
		this->instr = instr;
		this->state = true;
	}
	
	InstrumentController::~InstrumentController() {
		
	}
	
	Instrument *InstrumentController::getInstrument() {
		return this->instr;
	}
	
	device_id_t InstrumentController::getID() {
		return this->instr->getID();
	}
	
	bool InstrumentController::isEnabled() {
		return this->instr->enabled();
	}
	
	void InstrumentController::enable(bool e) {
		this->instr->enable(e && this->state);
	}
	
	void InstrumentController::flipState() {
		enable(!isEnabled());
	}
	
	bool InstrumentController::isRunnable() {
		return this->state;
	}
	
	void InstrumentController::setRunnable(bool r) {
		this->state = r;
		if (!this->state) {
			enable(false);
		}
	}
}