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


#ifndef CALX_NL300_MESSAGE_H_
#define CALX_NL300_MESSAGE_H_

#include <string>
#include <vector>
#include <cinttypes>
#include "ctrl-lib/device/Device.h"
#include "ctrl-lib/device/DeviceManager.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace CalX {

	class StandartDeviceManager; // Forward referencing
	
	enum class NL300MessageType {
		System, General
	};
	
	class NL300Message {
		public:
			// Arguments: receiver, message, sender
			NL300Message(NL300MessageType, std::string, std::string, std::string);
			virtual ~NL300Message();
			NL300MessageType getType();
			std::string getReceiver();
			std::string getMessage();
			std::string getSender();
			virtual std::string toCommand();
		private:
			NL300MessageType type;
			std::string receiver;
			std::string message;
			std::string sender;
	};
	
	enum class NL300ParameterType {
		None, Integer, Real, String
	};
	
	class NL300Parameter {
		public:
			NL300Parameter(NL300ParameterType);
			virtual ~NL300Parameter();
			NL300ParameterType getType();
			virtual std::string getString() = 0;
			int64_t getInt(int64_t);
			double getReal(double);
			std::string getString(std::string);
		private:
			NL300ParameterType type;
	};
	
	class NL300IntegerParameter : public NL300Parameter {
		public:
			NL300IntegerParameter(int64_t);
			virtual ~NL300IntegerParameter();
			int64_t getValue();
			virtual std::string getString();
		private:
			int64_t value;
	};
	
	class NL300RealParameter : public NL300Parameter {
		public:
			NL300RealParameter(double);
			virtual ~NL300RealParameter();
			double getValue();
			virtual std::string getString();
		private:
			double value;
	};
	
	class NL300StringParameter : public NL300Parameter {
		public:
			NL300StringParameter(std::string);
			virtual ~NL300StringParameter();
			std::string getValue();
			virtual std::string getString();
		private:
			std::string value;
	};
	
	class NL300NoneParameter : public NL300Parameter {
		public:
			NL300NoneParameter();
			virtual ~NL300NoneParameter();
			virtual std::string getString();
	};
}


#endif