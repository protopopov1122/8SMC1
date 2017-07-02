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


#ifndef CALX_NL300_COMMAND_H_
#define CALX_NL300_COMMAND_H_

#include <string>
#include <vector>
#include <cinttypes>
#include "ctrl-lib/device/Device.h"
#include "ctrl-lib/device/DeviceManager.h"
#include "device/standart/NL300Message.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace CalX {
	
	class StandartDeviceManager; // Forward referencing

	class NL300SystemCommand : public NL300Message {
		public:
			// Arguments: receiver, command, parameter, sender
			NL300SystemCommand(std::string, std::string, std::string, std::string);
			virtual ~NL300SystemCommand();
			std::string getCommand();
			std::string getParameter();
		private:
			std::string command;
			std::string parameter;
	};
	
	enum class NL300GeneralAction {
		Set = 'S', Add = 'A', Program = 'P', Inquiry = '?'
	};
	
	class NL300GeneralCommand : public NL300Message {
		public:
			// Arguments: receiver, array, index, action, parameter, sender
			NL300GeneralCommand(std::string, char, uint16_t, NL300GeneralAction, NL300Parameter*, std::string);
			virtual ~NL300GeneralCommand();
			char getArray();
			uint16_t getIndex();
			NL300GeneralAction getAction();
			NL300Parameter *getParameter();
		private:
			char array;
			uint16_t index;
			NL300GeneralAction action;
			NL300Parameter *parameter;
	};
}

#endif