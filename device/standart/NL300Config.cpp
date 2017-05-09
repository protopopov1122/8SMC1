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


#include <string.h>
#include <string>
#include "NL300Instrument.h"
#include "StandartDeviceManager.h"

namespace CalX {

	
	NL300ConfigEventListener::NL300ConfigEventListener(NL300Instrument *i) {
		this->instr = i;
	}
	
	NL300ConfigEventListener::~NL300ConfigEventListener() {
		
	}
	
	void NL300ConfigEventListener::entryAdded(ConfigManager *conf, std::string id) {
		
	}
	
	void NL300ConfigEventListener::entryRemoved(ConfigManager *conf, std::string id) {
		
	}
	
	void NL300ConfigEventListener::keyAdded(ConfigManager *conf, ConfigEntry *entry, std::string key) {
		process(conf, entry, key);
	}
	
	void NL300ConfigEventListener::keyRemoved(ConfigManager *conf, ConfigEntry *entry, std::string key) {
		
	}
	
	void NL300ConfigEventListener::keyChanged(ConfigManager *conf, ConfigEntry *entry, std::string key) {
		process(conf, entry, key);
	}
	
	void NL300ConfigEventListener::process(ConfigManager *conf, ConfigEntry *entry, std::string key) {
		if (entry->getEntryName().compare(NL300_ENTRY_NAME) != 0) {
			return;
		}
		if (key.compare(NL300_PACK_PULSES) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'P', 0, NL300GeneralAction::Set, new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		} else if (key.compare(NL300_MAX_OUTPUT_DELAY) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'D', 0, NL300GeneralAction::Set, new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		} else if (key.compare(NL300_ADJ_OUTPUT_DELAY) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'D', 1, NL300GeneralAction::Set, new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		} else if (key.compare(NL300_SYNC_OUT_DELAY) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'D', 2, NL300GeneralAction::Set, new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		} else if (key.compare(NL300_REPETITION_RATE_DIV) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'F', 0, NL300GeneralAction::Set, new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		}
	}
	
}