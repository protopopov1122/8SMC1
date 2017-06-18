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


#ifndef CALX_NL300_CONFIG_H_
#define CALX_NL300_CONFIG_H_

#include <string>
#include <vector>
#include <cinttypes>
#include "ctrl-lib/device/Device.h"
#include "ctrl-lib/device/DeviceManager.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace CalX {
	
	
	
	#define NL300_LASER_NAME "NL"
	#define NL300_PC_NAME "PC"
	#define NL300_ENTRY_NAME "instrument"
	#define NL300_CORE_NAME "core"
	#define NL300_PACK_PULSES "pack_pulses"
	#define NL300_MAX_OUTPUT_DELAY "max_output_delay"
	#define NL300_ADJ_OUTPUT_DELAY "adj_output_delay"
	#define NL300_SYNC_OUT_DELAY "sync_out_delay"
	#define NL300_REPETITION_RATE_DIV "repetition_rate_div"

	#define NL300_CORE_MODE "mode"
	#define NL300_MODE_CHANGE_DELAY "mode_change_delay"
	#define NL300_CORE_ENABLE_DELAY "enable_delay"
	#define NL300_CORE_DISABLE_DELAY "disable_delay"
	#define NL300_CORE_OFF_DELAY "off_mode_delay"
	#define NL300_CORE_ADJ_DELAY "adj_mode_delay"
	#define NL300_CORE_MAX_DELAY "max_mode_delay"
	#define NL300_CORE_ENABLE_OFF_DELAY "enable_off_delay"
	#define NL300_CORE_ENABLE_ADJ_DELAY "enable_adj_delay"
	#define NL300_CORE_ENABLE_MAX_DELAY "enable_max_delay"
	#define NL300_CORE_DISABLE_OFF_DELAY "disable_off_delay"
	#define NL300_CORE_DISABLE_ADJ_DELAY "disable_adj_delay"
	#define NL300_CORE_DISABLE_MAX_DELAY "disable_max_delay"

	
	class NL300Instrument; // Forward referencing
	class StandartDeviceManager; // Forward referencing

	class NL300ConfigEventListener : public ConfigEventListener {
		public:
			NL300ConfigEventListener(NL300Instrument*);
			virtual ~NL300ConfigEventListener();
			virtual void entryAdded(ConfigManager*, std::string);
			virtual void entryRemoved(ConfigManager*, std::string);
			virtual void keyAdded(ConfigManager*, ConfigEntry*, std::string);
			virtual void keyRemoved(ConfigManager*, ConfigEntry*, std::string);
			virtual void keyChanged(ConfigManager*, ConfigEntry*, std::string);
		private:
			void process(ConfigManager*, ConfigEntry*, std::string);
		
			NL300Instrument *instr;
	};
	
}

#endif