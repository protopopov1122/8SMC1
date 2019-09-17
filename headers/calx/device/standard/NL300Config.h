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

#ifndef CALX_DEVICE_STANDARD_NL300_CONFIG_H_
#define CALX_DEVICE_STANDARD_NL300_CONFIG_H_

#include "calx/ctrl-lib/device/Device.h"
#include "calx/ctrl-lib/device/DeviceManager.h"
#include <cinttypes>
#include <string>
#include <vector>

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

	class NL300Instrument;        // Forward referencing
	class StandardDeviceManager;  // Forward referencing

	class NL300ConfigEventListener : public CatalogueListener {
	 public:
		NL300ConfigEventListener(NL300Instrument *);
		void entryAdd(ConfigurationCatalogue *, const std::string &) override;
		void entryRemove(ConfigurationCatalogue *, const std::string &) override;
		void keyAdd(ConfigurationCatalogue *, const std::string &,
		            const std::string &) override;
		void keyRemove(ConfigurationCatalogue *, const std::string &,
		               const std::string &) override;
		void keyChange(ConfigurationCatalogue *, const std::string &,
		               const std::string &) override;

	 private:
		void process(ConfigurationCatalogue *, const std::string &,
		             const std::string &);

		NL300Instrument *instr;
	};
}  // namespace CalX

#endif