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
#include "device/Device.h"
#include "device/DeviceManager.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace CalX {
		
	enum class NL300InstrumentMode {
		Adjustment = 0, FullPower = 1
	};
	
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
	
	#define NL300_LASER_NAME "NL"
	#define NL300_PC_NAME "PC"
	#define NL300_ENTRY_NAME "core"
	#define NL300_MODE "mode"
	#define NL300_PACK_PULSES "pack_pulses"
	#define NL300_MAX_OUTPUT_DELAY "max_output_delay"
	#define NL300_ADJ_OUTPUT_DELAY "adj_output_delay"
	#define NL300_SYNC_OUT_DELAY "sync_out_delay"
	#define NL300_REPETITION_RATE_DIV "repetition_rate_div"
	
	class NL300Instrument; // Forward referencing

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
	
	class NL300Instrument : public Instrument {
		public:
			NL300Instrument(device_id_t, DeviceSerialPortConnectionPrms*, DeviceManager*);
			virtual ~NL300Instrument();
			virtual bool enable(bool);
			virtual bool enabled();
			virtual std::string getInfo();
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