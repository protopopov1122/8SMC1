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


#ifndef CALX_NL300_INSTRUMENT_H_
#define CALX_NL300_INSTRUMENT_H_

#include <string>
#include <vector>
#include <cinttypes>
#include "ctrl-lib/device/Device.h"
#include "ctrl-lib/device/DeviceManager.h"
#include "NL300Message.h"
#include "NL300Command.h"
#include "NL300Config.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace CalX {
	
	class StandartDeviceManager; // Forward referencing
	
	enum class NL300InstrumentMode {
		Adjustment = 0, FullPower = 1
	};
	
	class NL300Instrument : public Instrument {
		public:
			NL300Instrument(device_id_t, StandartDeviceManager*);
			virtual ~NL300Instrument();
			bool connect(DeviceSerialPortConnectionPrms*);
			virtual DeviceManager *getDeviceManager();
			virtual bool enable(bool);
			virtual bool enabled();
			virtual std::string getDeviceInfo();
			virtual std::string getRuntimeInfo();
			virtual bool setMode(InstrumentMode);
			virtual InstrumentMode getMode();
			
			bool writeMessage(NL300Message&);
			NL300Message *readMessage();
		private:
			bool writeSerial(std::string);
			int readSerial();
			bool start();
			bool stop();
			
			std::pair<std::string, std::string> getSystemCommandResponse(NL300SystemCommand&);
			std::string getSystemCommandResponse(std::string, std::string);
			NL300GeneralCommand *inquireGeneralParameter(char, uint16_t);
			int64_t inquireIntegerParameter(char, uint16_t, int64_t);
			ConfigEntry *getCoreEntry();
		
			StandartDeviceManager *devman;
			bool state;
			InstrumentMode mode;
			HANDLE handle;
			DeviceSerialPortConnectionPrms prms;
			NL300ConfigEventListener *listener;
			
			std::string hardwareInfo;
			std::string softwareInfo;
	};
}


#endif
