/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/device/standard/NL300Instrument.h"
#include "calx/device/standard/StandardDeviceManager.h"
#include <cstring>
#include <string>

namespace CalX {

	NL300ConfigEventListener::NL300ConfigEventListener(NL300Instrument *i) {
		this->instr = i;
	}

	void NL300ConfigEventListener::entryAdd(ConfigurationCatalogue *conf,
	                                        const std::string &id) {}

	void NL300ConfigEventListener::entryRemove(ConfigurationCatalogue *conf,
	                                           const std::string &id) {}

	void NL300ConfigEventListener::keyAdd(ConfigurationCatalogue *conf,
	                                      const std::string &entry,
	                                      const std::string &key) {
		process(conf, entry, key);
	}

	void NL300ConfigEventListener::keyRemove(ConfigurationCatalogue *conf,
	                                         const std::string &entry,
	                                         const std::string &key) {}

	void NL300ConfigEventListener::keyChange(ConfigurationCatalogue *conf,
	                                         const std::string &entry,
	                                         const std::string &key) {
		process(conf, entry, key);
	}

	void NL300ConfigEventListener::process(ConfigurationCatalogue *conf,
	                                       const std::string &entryName,
	                                       const std::string &key) {
		if (entryName.compare(NL300_ENTRY_NAME) != 0) {
			return;
		}
		ConfiguationFlatDictionary *entry = conf->getEntry(entryName);
		if (key.compare(NL300_PACK_PULSES) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'P', 0, NL300GeneralAction::Set,
			                        new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		} else if (key.compare(NL300_MAX_OUTPUT_DELAY) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'D', 0, NL300GeneralAction::Set,
			                        new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		} else if (key.compare(NL300_ADJ_OUTPUT_DELAY) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'D', 1, NL300GeneralAction::Set,
			                        new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		} else if (key.compare(NL300_SYNC_OUT_DELAY) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'D', 2, NL300GeneralAction::Set,
			                        new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		} else if (key.compare(NL300_REPETITION_RATE_DIV) == 0) {
			int_conf_t value = entry->getInt(key, 1);
			NL300GeneralCommand cmd(NL300_LASER_NAME, 'F', 0, NL300GeneralAction::Set,
			                        new NL300IntegerParameter(value), NL300_PC_NAME);
			this->instr->writeMessage(cmd);
		}
	}
}  // namespace CalX
