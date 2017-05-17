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
		
	NL300Instrument::NL300Instrument(device_id_t id, StandartDeviceManager *devman)
		: Instrument::Instrument() {
		this->dev = id;
		this->devman = devman;
		this->state = false;
		this->log("NL300 instrument constructor");
	}
	
	NL300Instrument::~NL300Instrument() {
		if (this->listener != nullptr) {
			this->config.removeEventListener(this->listener);
		}
		if (this->handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
		}
		this->log("Instrument closed");
	}

	bool NL300Instrument::connect(DeviceSerialPortConnectionPrms *prms) {
		this->log("NL300 instrument connecting: COM" + std::to_string(prms->port) +
			"; baud rate: " + std::to_string(prms->speed) + "; parity: " + std::to_string(static_cast<int>(prms->parity)));
		this->listener = nullptr;
		getDeviceManager()->loadConfiguration("NL300.ini", &this->config);
		memcpy(&this->prms, prms, sizeof(DeviceSerialPortConnectionPrms));
		this->log("Configuration sucessfully loaded");
		
		int baudrate = prms->speed;
		this->log("Init on COM" + std::to_string(prms->port) + ", baudrate: " + std::to_string(prms->speed) +
			", parity: " + std::to_string(static_cast<int>(prms->parity)));
		this->handle = CreateFile(("COM" + std::to_string(prms->port)).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (this->handle == INVALID_HANDLE_VALUE) {
			this->log("COM port opening error");
			this->errors.push_back("Error opening COM" + std::to_string(prms->port));
			this->handle = INVALID_HANDLE_VALUE;
			return false;
		}
		this->log("COM" + std::to_string(prms->port) + " opened");
		
		this->log("Setting COM port parameters");
		
		SetCommMask(handle, EV_RXCHAR);
		SetupComm(handle, 1500, 1500);

		COMMTIMEOUTS CommTimeOuts;
		CommTimeOuts.ReadIntervalTimeout = this->config.getEntry("connection")->getInt("ReadIntervalTimeout", 1);
		CommTimeOuts.ReadTotalTimeoutMultiplier = this->config.getEntry("connection")->getInt("ReadTotalTimeoutMultiplier", 1);
		CommTimeOuts.ReadTotalTimeoutConstant = this->config.getEntry("connection")->getInt("ReadTotalTimeoutConstant", 0xFFFFFFFF);
		CommTimeOuts.WriteTotalTimeoutMultiplier = this->config.getEntry("connection")->getInt("WriteTotalTimeoutMultiplier", 1);
		CommTimeOuts.WriteTotalTimeoutConstant = this->config.getEntry("connection")->getInt("WriteTotalTimeoutConstant", 250);

		if(!SetCommTimeouts(handle, &CommTimeOuts)) {
			this->log("COM port parameters setting error");
			CloseHandle(handle);
			this->handle = INVALID_HANDLE_VALUE;
			this->errors.push_back("Error configuring COM" + std::to_string(prms->port));
			return false;
		}
		this->log("Connection timeout parameters set up");
	
		DCB ComDCM;
	
		memset(&ComDCM, 0, sizeof(ComDCM));
		ComDCM.DCBlength = sizeof(DCB);
		GetCommState(handle, &ComDCM);
		ComDCM.BaudRate = DWORD(baudrate);
		ComDCM.Parity = static_cast<BYTE>(prms->parity);

		if(!SetCommState(handle, &ComDCM)) {
			this->log("COM port parameters setting error");
			CloseHandle(handle);
			this->handle = INVALID_HANDLE_VALUE;
			this->errors.push_back("Error configuring COM" + std::to_string(prms->port));
		}
		this->log("Connection parameters set up");
		
		
		this->log("Initializing instrument state");
		NL300GeneralCommand cmd(NL300_LASER_NAME, 'E', 0, NL300GeneralAction::Set, new NL300IntegerParameter(0), NL300_PC_NAME);
		if (!writeMessage(cmd)) {
			this->log("Instrument state setting error");
			this->errors.push_back("Error configuring COM" + std::to_string(prms->port));
			CloseHandle(this->handle);
			this->handle = INVALID_HANDLE_VALUE;
			return false;
		}
		this->mode = InstrumentMode::Off;
		this->log("Instrument mode switched to off");

		this->listener = new NL300ConfigEventListener(this);
		this->config.addEventListener(this->listener);
		
		ConfigEntry *core = this->config.getEntry(NL300_ENTRY_NAME);
		core->put(NL300_PACK_PULSES, new IntegerConfigValue(inquireIntegerParameter('P', 0, 1)));
		core->put(NL300_MAX_OUTPUT_DELAY, new IntegerConfigValue(inquireIntegerParameter('D', 0, 400)));
		core->put(NL300_ADJ_OUTPUT_DELAY, new IntegerConfigValue(inquireIntegerParameter('D', 1, 400)));
		core->put(NL300_SYNC_OUT_DELAY, new IntegerConfigValue(inquireIntegerParameter('D', 2, 0)));
		core->put(NL300_REPETITION_RATE_DIV, new IntegerConfigValue(inquireIntegerParameter('F', 0, 1)));
		
		this->hardwareInfo = getSystemCommandResponse("VER", "");
		this->softwareInfo = getSystemCommandResponse("SN", "");
		this->log("Instrument ready");
		
		return true;
	}
	
	DeviceManager *NL300Instrument::getDeviceManager() {
		return this->devman;
	}
	
	bool NL300Instrument::enable(bool en) {
		lock();
		if (handle == INVALID_HANDLE_VALUE) {
			this->log("Enable error: instrument closed");
			unlock();
			return false;
		}
		if (en == enabled()) {
			this->log("Enable: state not changed");
			unlock();
			return true;
		}
		
		this->log("Changing instrument state to " + std::string(en ? "enabled" : "disabled"));
		bool status = en ? start() : stop();
		if (!status) {
			this->log("Instrument state change failed");
			unlock();
			return false;
		}
		
		state = en;
		this->log("Instrument state changed to " + std::string(en ? "enabled" : "disabled"));
		unlock();
		return true;
	}

	bool NL300Instrument::start() {
		// std::string res = getSystemCommandResponse("START", "error");
		NL300SystemCommand syscom(NL300_LASER_NAME, "START", "", NL300_PC_NAME);
		std::pair<std::string, std::string> response = getSystemCommandResponse(syscom);
		std::string res = response.second;
		std::string rescom = response.first;
		/*
		(!res.empty() && res.compare("0") != 0 &&
			rescom.compare("START") == 0) &&
			rescom.compare("READY") != 0*/
		if ((rescom.compare("READY") != 0 &&
			rescom.compare("START") != 0) ||
			(rescom.compare("START") == 0 &&
			!res.empty() && res.compare("0") != 0)) {
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
			devman->saveInstrumentError();
			return false;
		}
		if (getCoreEntry()->getBool(NL300_CORE_ENABLE_DELAY, true)) {
			int syncDelay = 0;
			if (mode == InstrumentMode::Prepare) {
				syncDelay = getCoreEntry()->getInt(NL300_CORE_ENABLE_ADJ_DELAY, 400);
			} else if (mode == InstrumentMode::Full) {
				syncDelay = getCoreEntry()->getInt(NL300_CORE_ENABLE_MAX_DELAY, 400);
			} else {
				syncDelay = getCoreEntry()->getInt(NL300_CORE_ENABLE_OFF_DELAY, 400);
			}
			Sleep(syncDelay);
		}
		return true;
	}

	bool NL300Instrument::stop() {
		NL300SystemCommand syscom(NL300_LASER_NAME, "STOP", "", NL300_PC_NAME);
		writeMessage(syscom);
		std::pair<std::string, std::string> res = std::make_pair("NOTREADY", "");
		while (res.first.compare("NOTREADY") == 0) {
			if (getCoreEntry()->getBool(NL300_CORE_DISABLE_DELAY, true)) {
				int syncDelay = 0;
				if (mode == InstrumentMode::Prepare) {
					syncDelay = getCoreEntry()->getInt(NL300_CORE_DISABLE_ADJ_DELAY, 400);
				} else if (mode == InstrumentMode::Full) {
					syncDelay = getCoreEntry()->getInt(NL300_CORE_DISABLE_MAX_DELAY, 400);
				} else {
					syncDelay = getCoreEntry()->getInt(NL300_CORE_DISABLE_OFF_DELAY, 400);
				}
				Sleep(syncDelay);
			}
			NL300SystemCommand syscom3(NL300_LASER_NAME, "SAY", "", NL300_PC_NAME);
			res = getSystemCommandResponse(syscom3);
		}
		return true;
	}
	
	InstrumentMode NL300Instrument::getMode() {
		return this->mode;
	}
	
	bool NL300Instrument::setMode(InstrumentMode mode) {
		lock();
		if (handle == INVALID_HANDLE_VALUE) {
			this->log("Set mode error: instrument closed");
			unlock();
			return false;
		}
		if (this->mode == mode) {
			this->log("Mode was not changed to the same");
			unlock();
			return true;
		}
		
		int imode = mode == InstrumentMode::Off ? 0 : (mode == InstrumentMode::Prepare? 1 : 2);
		this->log("Changing instrument mode to " + std::to_string(imode));
		NL300GeneralCommand cmd(NL300_LASER_NAME, 'E', 0, NL300GeneralAction::Set, new NL300IntegerParameter(imode), NL300_PC_NAME);
		if (!writeMessage(cmd)) {
			this->log("Instrument mode change command failed");
			unlock();
			return false;
		}
		if (getCoreEntry()->getBool(NL300_MODE_CHANGE_DELAY, true)) {
			int syncDelay = 0;
			if (mode == InstrumentMode::Prepare) {
				syncDelay = getCoreEntry()->getInt(NL300_CORE_ADJ_DELAY, 400);
			} else if (mode == InstrumentMode::Full) {
				syncDelay = getCoreEntry()->getInt(NL300_CORE_MAX_DELAY, 400);
			} else {
				syncDelay = getCoreEntry()->getInt(NL300_CORE_OFF_DELAY, 400);
			}
			this->log("Mode change delay sleep " + std::to_string(syncDelay) + " ms");
			Sleep(syncDelay);
		}
		
		this->mode = mode;
		this->log("Mode changed to " + std::to_string(imode));
		unlock();
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
		this->log("Received message: " + std::string(mesg));
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
		this->log("Parsed message: " + out->toCommand());

		return out;
	}
	
	bool NL300Instrument::writeSerial(std::string stdStr) {
		if (this->handle == INVALID_HANDLE_VALUE) {
			this->log("COM port write error: port closed");
			return false;
		}
		this->log("Writing to COM" + std::to_string(prms.port) + ": '" + stdStr + "'");
		const char *data = stdStr.c_str();
		DWORD feedback;
		if(!WriteFile(handle, data, (DWORD) strlen(data), &feedback, 0) || feedback != strlen(data)) {
			/* this->log("Write to COM" + std::to_string(prms.port) + " failed: '" + stdStr + "'");
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
			this->errors.push_back("Error writing to COM" + std::to_string(prms.port));
			this->devman->saveInstrumentError();
			return false;*/
			this->log("COM" + std::to_string(prms.port) + " write error, feedback: " + std::to_string(feedback));
		}
		this->log("Write finished");
		return true;
	}

	int NL300Instrument::readSerial() {
		if (this->handle == INVALID_HANDLE_VALUE) {
			return EOF;
		}

		int chr;
		DWORD feedback;
		if (!ReadFile(this->handle, &chr, 1, &feedback, NULL) || feedback != 1) {
			/*this->log("Reading from to COM" + std::to_string(prms.port) + " failed");
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
			this->errors.push_back("Error reading from COM" + std::to_string(prms.port));
			this->devman->saveInstrumentError();
			return EOF;*/
			this->log("COM" + std::to_string(prms.port) + " read error, feedback: " + std::to_string(feedback));
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
	
	std::string NL300Instrument::getRuntimeInfo() {
		return "";
	}
	
	std::pair<std::string, std::string> NL300Instrument::getSystemCommandResponse(NL300SystemCommand &syscom) {
		this->log("Sending system command: " + syscom.toCommand());
		bool b = writeMessage(syscom);
		if (!b) {
		this->log("Sending general command: " + syscom.toCommand());
			return std::make_pair("", "");
		}
		NL300Message *res = readMessage();
		if (res == nullptr) {
			this->log("System command sending error");
			return std::make_pair("", "");
		}
		this->log("System command response: " + res->toCommand());
		if (res->getType() != NL300MessageType::System) {
			delete res;
			return std::make_pair("", "");
		}
		NL300SystemCommand *sysres = (NL300SystemCommand*) res;
		std::pair<std::string, std::string> out = make_pair(sysres->getCommand(), sysres->getParameter());
		this->log("System command response: <" + out.first + ", " + out.second + ">");
		delete sysres;
		return out;
	}
	
	std::string NL300Instrument::getSystemCommandResponse(std::string cmd, std::string def) {
		this->log("Sending system command: " + cmd + "; " + def);
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
		this->log("Sending general command: " + cmd.toCommand());
		bool b = writeMessage(cmd);
		if (!b) {
			this->log("General command sending error");	
			return nullptr;
		}
		NL300Message *res = readMessage();
		if (res == nullptr) {	
			this->log("General command sending error");	
			return nullptr;
		}
		if (res->getType() != NL300MessageType::General) {
			delete res;
			return nullptr;
		}
		NL300GeneralCommand *gencom = (NL300GeneralCommand*) res;
		this->log("Received general command response: " + gencom->toCommand());
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

	ConfigEntry *NL300Instrument::getCoreEntry() {
		std::string name = this->config.getEntry(NL300_CORE_NAME)->getString(NL300_CORE_MODE, "default");
		return this->config.getEntry(name);
	}
}