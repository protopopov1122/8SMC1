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

#include "device/standart/NL300Instrument.h"
#include "device/standart/StandartDeviceManager.h"
#include <string.h>
#include <string>

namespace CalX {

	NL300SystemCommand::NL300SystemCommand(std::string recv, std::string com,
	                                       std::string parm, std::string send)
	    : NL300Message::NL300Message(NL300MessageType::System, recv,
	                                 com + (parm.empty() ? "" : "=" + parm),
	                                 send) {
		this->command = com;
		this->parameter = parm;
	}

	NL300SystemCommand::~NL300SystemCommand() {}

	std::string NL300SystemCommand::getCommand() {
		return this->command;
	}

	std::string NL300SystemCommand::getParameter() {
		return this->parameter;
	}

	NL300GeneralCommand::NL300GeneralCommand(std::string recv, char array,
	                                         uint16_t index,
	                                         NL300GeneralAction action,
	                                         NL300Parameter *parm,
	                                         std::string send)
	    : NL300Message::NL300Message(
	          NL300MessageType::General, recv,
	          array + std::to_string(index) + '/' + static_cast<char>(action) +
	              (parm != nullptr ? parm->getString() : ""),
	          send) {
		this->array = array;
		this->index = index;
		this->action = action;
		this->parameter = parm;
	}

	NL300GeneralCommand::~NL300GeneralCommand() {
		delete this->parameter;
	}

	char NL300GeneralCommand::getArray() {
		return this->array;
	}

	uint16_t NL300GeneralCommand::getIndex() {
		return this->index;
	}

	NL300GeneralAction NL300GeneralCommand::getAction() {
		return this->action;
	}

	NL300Parameter *NL300GeneralCommand::getParameter() {
		return this->parameter;
	}
}  // namespace CalX
