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
	
	NL300Message::NL300Message(NL300MessageType type, std::string recv, std::string msg, std::string snd) {
		this->type = type;
		this->receiver = recv;
		this->message = msg;
		this->sender = snd;
	}
	
	NL300Message::~NL300Message() {
		
	}
	
	NL300MessageType NL300Message::getType() {
		return this->type;
	}
	
	std::string NL300Message::getReceiver() {
		return this->receiver;
	}
	
	std::string NL300Message::getMessage() {
		return this->message;
	}
	
	std::string NL300Message::getSender() {
		return this->sender;
	}
	
	std::string NL300Message::toCommand() {
		return "[" + this->receiver + ":" + this->message + "\\" + this->sender + "]";
	}
	
	NL300Parameter::NL300Parameter(NL300ParameterType t) {
		this->type = t;
	}
	
	NL300Parameter::~NL300Parameter() {
		
	}
	
	NL300ParameterType NL300Parameter::getType() {
		return this->type;
	}
	
	int64_t NL300Parameter::getInt(int64_t def) {
		if (this->type == NL300ParameterType::Integer) {
			return ((NL300IntegerParameter*) this)->getValue();
		} else {
			return def;
		}
	}
	
	double NL300Parameter::getReal(double def) {
		if (this->type == NL300ParameterType::Real) {
			return ((NL300RealParameter*) this)->getValue();
		} else {
			return def;
		}
	}
	
	std::string NL300Parameter::getString(std::string def) {
		if (this->type == NL300ParameterType::String) {
			return ((NL300StringParameter*) this)->getValue();
		} else {
			return def;
		}
	}
	
	NL300IntegerParameter::NL300IntegerParameter(int64_t v)
		: NL300Parameter::NL300Parameter(NL300ParameterType::Integer) {
		this->value = v;
	}
	
	NL300IntegerParameter::~NL300IntegerParameter() {
		
	}
	
	int64_t NL300IntegerParameter::getValue() {
		return this->value;
	}
	
	std::string NL300IntegerParameter::getString() {
		return std::to_string(this->value);
	}
	
	NL300RealParameter::NL300RealParameter(double v)
		: NL300Parameter::NL300Parameter(NL300ParameterType::Real) {
		this->value = v;
	}
	
	NL300RealParameter::~NL300RealParameter() {
		
	}
	
	double NL300RealParameter::getValue() {
		return this->value;
	}
	
	std::string NL300RealParameter::getString() {
		return std::to_string(this->value);
	}
	
	NL300StringParameter::NL300StringParameter(std::string v)
		: NL300Parameter::NL300Parameter(NL300ParameterType::String) {
		this->value = v;	
	}
	
	NL300StringParameter::~NL300StringParameter() {
		
	}
	
	std::string NL300StringParameter::getValue() {
		return this->value;
	}
	
	std::string NL300StringParameter::getString() {
		return "=" + this->value;
	}
	
	NL300NoneParameter::NL300NoneParameter()
		: NL300Parameter::NL300Parameter(NL300ParameterType::None) {
			
	}
	
	NL300NoneParameter::~NL300NoneParameter() {
		
	}
	
	std::string NL300NoneParameter::getString() {
		return "";
	}

	NL300SystemCommand::NL300SystemCommand(std::string recv, std::string com, std::string parm, std::string send)
		: NL300Message::NL300Message(NL300MessageType::System, recv, com + (parm.empty() ? "" :  "=" + parm), send) {
		this->command = com;
		this->parameter = parm;
	}
	
	NL300SystemCommand::~NL300SystemCommand() {
		
	}

	std::string NL300SystemCommand::getCommand() {
		return this->command;
	}

	std::string NL300SystemCommand::getParameter() {
		return this->parameter;
	}
	
	NL300GeneralCommand::NL300GeneralCommand(std::string recv, char array, uint16_t index,
		NL300GeneralAction action, NL300Parameter *parm, std::string send)
		: NL300Message::NL300Message(NL300MessageType::General, recv, array + std::to_string(index) + '/' +
			static_cast<char>(action) + parm->getString(), send) {
				
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
	
	#define ILOG(msg) LOG_INSTR(this->getID(), msg)
	
	NL300Instrument::NL300Instrument(device_id_t id, DeviceSerialPortConnectionPrms *prms, StandartDeviceManager *devman)
		: Instrument::Instrument() {
		this->dev = id;
		this->devman = devman;
		this->state = false;
		memcpy(&this->prms, prms, sizeof(DeviceSerialPortConnectionPrms));
		
		int baudrate = prms->speed;
		ILOG("Init on COM" + std::to_string(prms->port) + ", baudrate: " + std::to_string(prms->speed) +
			", parity: " + std::to_string(static_cast<int>(prms->parity)));
		this->handle = CreateFile(("COM" + std::to_string(prms->port)).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (this->handle == INVALID_HANDLE_VALUE) {
			ILOG("COM port opening error");
			this->errors.push_back("Error opening COM" + std::to_string(prms->port));
			this->handle = INVALID_HANDLE_VALUE;
			return;
		}
		
		ILOG("Setting COM port parameters");
		
		SetCommMask(handle, EV_RXCHAR);
		SetupComm(handle, 1500, 1500);

		// TODO Properly setup timeouts
		COMMTIMEOUTS CommTimeOuts;
		CommTimeOuts.ReadIntervalTimeout = 1;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 1;
		CommTimeOuts.ReadTotalTimeoutConstant = 0xFFFFFFFF;
		CommTimeOuts.WriteTotalTimeoutMultiplier = 1;
		CommTimeOuts.WriteTotalTimeoutConstant = 1;

		if(!SetCommTimeouts(handle, &CommTimeOuts)) {
			ILOG("COM port parameters setting error");
			CloseHandle(handle);
			this->errors.push_back("Error configuring COM" + std::to_string(prms->port));
			return;
		}
	
		DCB ComDCM;
	
		memset(&ComDCM,0,sizeof(ComDCM));
		ComDCM.DCBlength = sizeof(DCB);
		GetCommState(handle, &ComDCM);
		ComDCM.BaudRate = DWORD(baudrate);
		ComDCM.Parity = static_cast<BYTE>(prms->parity);

		if(!SetCommState(handle, &ComDCM)) {
			ILOG("COM port parameters setting error");
			CloseHandle(handle);
			this->errors.push_back("Error configuring COM" + std::to_string(prms->port));
		}
		
		
		ILOG("Initializing instrument state");
		NL300GeneralCommand cmd(NL300_LASER_NAME, 'E', 0, NL300GeneralAction::Set, new NL300IntegerParameter(0), NL300_PC_NAME);
		if (!writeMessage(cmd)) {
			ILOG("Instrument state setting error");
			this->errors.push_back("Error configuring COM" + std::to_string(prms->port));
		}
		this->mode = InstrumentMode::Off;

		this->listener = new NL300ConfigEventListener(this);
		this->config.addEventListener(this->listener);
		
		ConfigEntry *core = this->config.getEntry(NL300_ENTRY_NAME);
		core->put(NL300_MODE_CHANGE_DELAY, new BoolConfigValue(true));
		core->put(NL300_ENABLE_DELAY, new BoolConfigValue(true));
		core->put(NL300_PACK_PULSES, new IntegerConfigValue(inquireIntegerParameter('P', 0, 1)));
		core->put(NL300_MAX_OUTPUT_DELAY, new IntegerConfigValue(inquireIntegerParameter('D', 0, 400)));
		core->put(NL300_ADJ_OUTPUT_DELAY, new IntegerConfigValue(inquireIntegerParameter('D', 1, 400)));
		core->put(NL300_SYNC_OUT_DELAY, new IntegerConfigValue(inquireIntegerParameter('D', 2, 0)));
		core->put(NL300_REPETITION_RATE_DIV, new IntegerConfigValue(inquireIntegerParameter('F', 0, 1)));
		
		this->hardwareInfo = getSystemCommandResponse("VER", "");
		this->softwareInfo = getSystemCommandResponse("SN", "");
		
		ILOG("Instrument ready");
	}
	
	NL300Instrument::~NL300Instrument() {
		this->config.removeEventListener(this->listener);
		if (this->handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
		}
		ILOG("Instrument closed");
	}
	
	DeviceManager *NL300Instrument::getDeviceManager() {
		return this->devman;
	}
	
	bool NL300Instrument::enable(bool en) {
		if (handle == INVALID_HANDLE_VALUE) {
			ILOG("Enable error: instrument closed");
			return false;
		}
		if (en == enabled()) {
			ILOG("Enable: state not changed");
			return true;
		}
		
		ILOG("Changing instrument state to " + std::string(en ? "enabled" : "disabled"));
		bool status = en ? start() : stop();
		if (!status) {
			return false;
		}
		
		state = en;
		ILOG("Instrument state changed to " + std::string(en ? "enabled" : "disabled"));
		return true;
	}

	bool NL300Instrument::start() {
		std::string res = getSystemCommandResponse("START", "error");
		if (!res.empty()) {
			if (res.compare("1") == 0) {
				this->errors.push_back("NL300 start error: laser not ready");
			} else if (res.compare("2") == 0) {
				this->errors.push_back("NL300 start error: overheat");
			} else if (res.compare("4") == 0) {
				this->errors.push_back("NL300 start error: flash lamp");
			} else if (res.compare("8") == 0) {
				this->errors.push_back("NL300 start error: interlock");
			} else if (res.compare("16") == 0) {
				this->errors.push_back("NL300 start error: cover");
			} else {
				this->errors.push_back("NL300 start error: unknown");
			}
			getDeviceManager()->saveError();
		}
		if (this->config.getEntry(NL300_ENTRY_NAME)->getBool(NL300_ENABLE_DELAY, true)) {
			int syncDelay = 0;
			if (mode == InstrumentMode::Prepare) {
				syncDelay = this->config.getEntry(NL300_ENTRY_NAME)->getInt(NL300_ADJ_OUTPUT_DELAY, 400);
			} else if (mode == InstrumentMode::Full) {
				syncDelay = this->config.getEntry(NL300_ENTRY_NAME)->getInt(NL300_MAX_OUTPUT_DELAY, 400);
			}
			Sleep(syncDelay);
		}
		return res.empty();
	}

	bool NL300Instrument::stop() {
		NL300SystemCommand msg(NL300_LASER_NAME, "STOP", "", NL300_PC_NAME);
		return writeMessage(msg);
	}
	
	InstrumentMode NL300Instrument::getMode() {
		return this->mode;
	}
	
	bool NL300Instrument::setMode(InstrumentMode mode) {
		if (handle == INVALID_HANDLE_VALUE) {
			ILOG("Set mode error: instrument closed");
			return false;
		}
		if (this->mode == mode) {
			return true;
		}
		
		int imode = mode == InstrumentMode::Off ? 0 : (mode == InstrumentMode::Prepare? 1 : 2);
		ILOG("Changing instrument mode to " + std::to_string(imode));
		NL300GeneralCommand cmd(NL300_LASER_NAME, 'E', 0, NL300GeneralAction::Set, new NL300IntegerParameter(imode), NL300_PC_NAME);
		if (!writeMessage(cmd)) {
			return false;
		}
		if (this->config.getEntry(NL300_ENTRY_NAME)->getBool(NL300_MODE_CHANGE_DELAY, true)) {
			int syncDelay = 0;
			if (mode == InstrumentMode::Prepare) {
				syncDelay = this->config.getEntry(NL300_ENTRY_NAME)->getInt(NL300_ADJ_OUTPUT_DELAY, 400);
			} else if (mode == InstrumentMode::Full) {
				syncDelay = this->config.getEntry(NL300_ENTRY_NAME)->getInt(NL300_MAX_OUTPUT_DELAY, 400);
			}
			Sleep(syncDelay);
		}
		
		this->mode = mode;
		ILOG("Mode changed to " + std::to_string(imode));
		return true;
	}
	
	bool NL300Instrument::writeMessage(NL300Message &msg) {
		return writeSerial(msg.toCommand());
	}

	NL300Message *NL300Instrument::readMessage() {
		NL300Message *out = nullptr;
		
		const int MAX_LEN = 128;
		char mesg[MAX_LEN];
		size_t offset = 0;
		char chr;
		do {
			chr = readSerial();
			if (chr == EOF) {
				return nullptr;
			}
			mesg[offset++] = chr;
		} while (chr != ']' && offset < MAX_LEN - 1);
		mesg[offset] = '\0';
		if (strlen(mesg) < 2 ||
			mesg[0] != '[' ||
			mesg[strlen(mesg) - 1] != ']') {
			return nullptr;
		}

		std::string recv;
		std::string message;
		std::string sender;
		const char *ptr = &mesg[1];
		while (*ptr != ':' &&
			*ptr != ']') {
			recv.push_back(*ptr);
			ptr++;
		}
		if (*ptr != ':') {
			return nullptr;
		}
		ptr++;
		while (*ptr != '\\' &&
			*ptr != ']') {
			message.push_back(*ptr);
			ptr++;
		}
		if (*ptr != '\\') {
			return nullptr;
		}
		ptr++;
		while (*ptr != ']') {
			sender.push_back(*ptr);
			ptr++;
		}

		do { // Do-While used only to provide useful break operation
			if (message.find('/') != std::string::npos &&
				message.length() >= 4) {	// May be General command
				ptr = message.c_str();
				char array = *(ptr++);
				uint16_t index = 0;
				while (isdigit(*ptr) &&
					*ptr != '/' &&
					*ptr != '\0') {
					index *= 10;
					index += (*ptr) - '0';
					ptr++;
				}
				if (*ptr != '/') {
					break;
				}
				ptr++;
				NL300GeneralAction act = NL300GeneralAction::Inquiry;
				char op = *ptr;
				if (op == static_cast<char>(NL300GeneralAction::Set)) {
					act = NL300GeneralAction::Set;
				} else 
				if (op == static_cast<char>(NL300GeneralAction::Add)) {
					act = NL300GeneralAction::Add;
				} else 
				if (op == static_cast<char>(NL300GeneralAction::Program)) {
					act = NL300GeneralAction::Program;
				} else 
				if (op == static_cast<char>(NL300GeneralAction::Inquiry)) {
					act = NL300GeneralAction::Inquiry;
				} else {
					break;
				}
				ptr++;
				std::string parameter;
				while (*ptr != '\0') {
					parameter.push_back(*ptr);
					ptr++;
				}
				NL300Parameter *pr = nullptr;
				if (!parameter.empty()) {
					ptr = parameter.c_str();
					if (*ptr == '=') {
						pr = new NL300StringParameter(std::string(ptr + 1));
					}
					bool integer = true;
					bool fp = false;
					for (size_t i = 0; i < strlen(ptr); i++) {
						char chr = ptr[i];
						if (isdigit(chr)) {
							continue;
						} else if (chr == '.' && !fp) {
							fp = true;
						} else {
							integer = false;
						}
					}
					if (integer) {
						if (fp) {
							pr = new NL300RealParameter(std::stod(parameter));
						} else {
							pr = new NL300IntegerParameter(std::stoll(parameter));
						}
					}
				}
				out = new NL300GeneralCommand(recv, array, index, act, pr, sender);
			}
		} while (false);
		if (out == nullptr) {	// System command
			std::string command;
			std::string parameter;
			ptr = message.c_str();
			while (*ptr != '=' && *ptr != '\0' && *ptr != '\"') {
				command.push_back(*ptr);
				ptr++;
			}
			if (*ptr == '=') {
				ptr++;
				while (*ptr != '\0') {
					parameter.push_back(*ptr);	
					ptr++;
				}
			} else if (*ptr == '\"') {
				ptr++;
				while (*ptr != '\"' && *ptr != '\0') {
					parameter.push_back(*ptr);
					ptr++;
				}
			}
			out = new NL300SystemCommand(recv, command, parameter, sender);
		}

		return out;
	}
	
	bool NL300Instrument::writeSerial(std::string stdStr) {
		if (this->handle == INVALID_HANDLE_VALUE) {
			ILOG("COM port write error: port closed");
			return false;
		}
		ILOG("Writing to COM" + std::to_string(prms.port) + ": '" + stdStr + "'");
		const char *data = stdStr.c_str();
		DWORD feedback;
		if(!WriteFile(handle, data, (DWORD) strlen(data), &feedback, 0) || feedback != (DWORD) strlen(data)) {
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
			this->errors.push_back("Error writing to COM" + std::to_string(prms.port));
			getDeviceManager()->saveError();
			return false;
		}
		ILOG("Write finished");
		return true;
	}

	int NL300Instrument::readSerial() {
		if (this->handle == INVALID_HANDLE_VALUE) {
			return EOF;
		}

		int chr;
		DWORD feedback;
		if (!ReadFile(this->handle, &chr, 1, &feedback, NULL) || feedback != (DWORD) 1) {
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
			this->errors.push_back("Error reading from COM" + std::to_string(prms.port));
			getDeviceManager()->saveError();
			return EOF;
		}
		return chr;
	}
	
	bool NL300Instrument::enabled() {
		return state;
	}
	
	std::string NL300Instrument::getDeviceInfo() {
		std::string out = "Connected via serial port: COM" +
			std::to_string(prms.port) + "; speed: " +
			std::to_string(prms.speed) + "; parity: ";
		switch (prms.parity) {
			case SerialPortParity::No:
				out += "No";
			break;
			case SerialPortParity::Odd:
				out += "Odd";
			break;
			case SerialPortParity::Even:
				out += "Even";
			break;
			case SerialPortParity::Mark:
				out += "Mark";
			break;
			case SerialPortParity::Space:
				out += "Space";
			break;
		}
		if (!this->hardwareInfo.empty()) {
			out += "\nHardware info: " + this->hardwareInfo;
		}
		if (!this->softwareInfo.empty()) {
			out += "\nSoftware info: " + this->softwareInfo;
		}
		return out;
	}
	
	std::pair<std::string, std::string> NL300Instrument::getSystemCommandResponse(NL300SystemCommand &syscom) {
		bool b = writeMessage(syscom);
		if (!b) {
			return std::make_pair("", "");
		}
		NL300Message *res = readMessage();
		if (res == nullptr) {
			return std::make_pair("", "");
		}
		if (res->getType() != NL300MessageType::System) {
			delete res;
			return std::make_pair("", "");
		}
		NL300SystemCommand *sysres = (NL300SystemCommand*) res;
		std::pair<std::string, std::string> out = make_pair(sysres->getCommand(), sysres->getParameter());
		delete sysres;
		return out;
	}
	
	std::string NL300Instrument::getSystemCommandResponse(std::string cmd, std::string def) {
		NL300SystemCommand syscom(NL300_LASER_NAME, cmd, "", NL300_PC_NAME);
		std::pair<std::string, std::string> res = getSystemCommandResponse(syscom);
		if (res.first.compare(cmd) == 0) {
			return res.second;
		} else {
			return def;
		}
	}
	
	NL300GeneralCommand *NL300Instrument::inquireGeneralParameter(char array, uint16_t index) {
		NL300GeneralCommand cmd(NL300_LASER_NAME, array, index, NL300GeneralAction::Inquiry, new NL300NoneParameter(), NL300_PC_NAME);
		bool b = writeMessage(cmd);
		if (!b) {
			return nullptr;
		}
		NL300Message *res = readMessage();
		if (res == nullptr) {	
			return nullptr;
		}
		if (res->getType() != NL300MessageType::General) {
			delete res;
			return nullptr;
		}
		NL300GeneralCommand *gencom = (NL300GeneralCommand*) res;
		if (gencom->getArray() != array ||
			gencom->getIndex() != index ||
			gencom->getAction() != NL300GeneralAction::Set) {
			delete gencom;
			return nullptr;
		}
		return gencom;
	}
	
	int64_t NL300Instrument::inquireIntegerParameter(char array, uint16_t index, int64_t def) {
		NL300GeneralCommand *cmd = inquireGeneralParameter(array, index);
		if (cmd == nullptr) {
			return def;
		}
		NL300Parameter *parm = cmd->getParameter();
		if (parm == nullptr) {
			delete cmd;
			return def;
		}
		int64_t val = parm->getInt(def);
		delete cmd;
		return val;
	}
}