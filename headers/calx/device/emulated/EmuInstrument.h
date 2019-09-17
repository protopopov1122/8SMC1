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

#ifndef CALX_DEVICE_EMULATED_EMU_INSTRUMENT_H_
#define CALX_DEVICE_EMULATED_EMU_INSTRUMENT_H_

#include "calx/ctrl-lib/device/Device.h"
#include "calx/ctrl-lib/device/DeviceManager.h"
#include <cinttypes>
#include <string>
#include <vector>

namespace CalX {

	class EmuDeviceManager;  // Forward referencing

	class EmuInstrument : public Instrument {
	 public:
		EmuInstrument(device_id_t, EmuDeviceManager &);
		virtual DeviceManager &getDeviceManager();
		virtual bool open_session();
		virtual bool close_session();
		virtual bool enable(bool);
		virtual bool enabled();
		virtual void terminate();
		virtual std::string getDeviceInfo();
		virtual std::string getRuntimeInfo();
		virtual bool setWorkingMode(InstrumentMode);
		virtual InstrumentMode getWorkingMode();

	 private:
		EmuDeviceManager &devman;
		bool en;
		InstrumentMode mode;
	};
}  // namespace CalX

#endif
