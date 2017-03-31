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


#include <iostream>
#include <string.h>
#include "8SMC1DeviceManager.h"

namespace CalX {
	
	_8SMC1Instrument::_8SMC1Instrument(device_id_t id, DeviceSerialPortConnectionPrms *prms, DeviceManager *devman)
		: Instrument::Instrument() {
		this->dev = id;
		this->devman = devman;
		this->state = false;
		memcpy(&this->prms, prms, sizeof(DeviceSerialPortConnectionPrms));
		
		// Testing stub values
		int baudrate = prms->speed;
		int TIMEOUT = 1000;
		
		this->handle = CreateFile(("COM" + std::to_string(prms->port)).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (this->handle == INVALID_HANDLE_VALUE) {
			this->error = true;
			return;
		}
		
		SetCommMask(handle, EV_RXCHAR);
		SetupComm(handle, 1500, 1500);

		COMMTIMEOUTS CommTimeOuts;
		CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
		CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
		CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;

		if(!SetCommTimeouts(handle, &CommTimeOuts)) {
			CloseHandle(handle);
			this->error = true;
			return;
		}
	
		DCB ComDCM;
	
		memset(&ComDCM,0,sizeof(ComDCM));
		ComDCM.DCBlength = sizeof(DCB);
		GetCommState(handle, &ComDCM);
		ComDCM.BaudRate = DWORD(baudrate);
		ComDCM.Parity = static_cast<BYTE>(prms->parity);

		if(!SetCommState(handle, &ComDCM)) {
			CloseHandle(handle);
			this->error = true;
		}
	}
	
	_8SMC1Instrument::~_8SMC1Instrument() {
		if (!error) {
			CloseHandle(handle);
		}
	}
	
	bool _8SMC1Instrument::enable(bool en) {
		if (en == enabled()) {
			return true;
		}
		
		// Testing stub command
		const char *data = std::string("Laser: " + std::string(en ? "enable\r\n" : "disable\r\n")).c_str();
		DWORD feedback;
		if(!WriteFile(handle, data, (DWORD) strlen(data), &feedback, 0) || feedback != (DWORD) strlen(data)) {
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
			this->error = true;
			return false;
		}
		state = en;
		return true;
	}
	
	bool _8SMC1Instrument::enabled() {
		return state;
	}
	
	std::string _8SMC1Instrument::getInfo() {
		std::string out = "Connected via serial: COM" +
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
		return out;
	}
	
	_8SMC1Device::_8SMC1Device(device_id_t dev, DeviceManager *devman) {
		this->dev = dev;
		this->devman = devman;
		this->speed = 1500;
		this->slow_start = false;
		this->autoSaveConfig = true;
		this->waitSync = false;
		memset((void*) &this->state, (int) 0, (unsigned int) sizeof(USMC_State));	// Temporary
		this->updateState();
		this->updateMode();
		this->updateParameters();
	}

	_8SMC1Device::~_8SMC1Device() {

	}

	bool _8SMC1Device::isAutoSaving() {
		return this->autoSaveConfig;
	}

	void _8SMC1Device::setAutoSave(bool as) {
		this->autoSaveConfig = as;
	}

	bool _8SMC1Device::flush() {
		if (USMC_SetMode(dev, mode)) {
			devman->saveError();
			return false;
		}
		if (USMC_SetParameters(dev, prms)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Device::setCurrentPosition(int pos) {
		if (USMC_SetCurrentPosition(dev, pos)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Device::updateMode() {
		if (USMC_GetMode(dev, mode)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	void _8SMC1Device::updateState() {
		if (USMC_GetState(dev, state)) {
			devman->saveError();
		}
	}

	int _8SMC1Device::getPosition() {
		this->updateState();
		return this->state.CurPos;
	}

	float _8SMC1Device::getTemperature() {
		this->updateState();
		return this->state.Temp;
	}

	float _8SMC1Device::getVoltage() {
		this->updateState();
		return this->state.Voltage;
	}

	unsigned char _8SMC1Device::getStepDivisor() {
		this->updateState();
		return this->state.SDivisor;
	}

	bool _8SMC1Device::isTrailerPressed(int t) {	// Accepts trailer id, if unknown returns false
		this->updateState();
		if (t == 1) {
			return this->state.Trailer1;
		}
		if (t == 2) {
			return this->state.Trailer2;
		}
		return false;
	}

	bool _8SMC1Device::getLoftState() {
		this->updateState();
		return this->state.Loft;
	}

	Power _8SMC1Device::getPowerState() {		// Translate power state into enum
		this->updateState();
		if (this->state.Power && this->state.FullPower) {
			return Power::FullPower;
		} else if (this->state.Power) {
			return Power::HalfPower;
		} else {
			return Power::NoPower;
		}
	}
	
	bool _8SMC1Device::isRunning() {
		this->updateState();
		return this->state.RUN;
	}

	bool _8SMC1Device::hasFullSpeed() {
		this->updateState();
		return this->state.FullSpeed;
	}

	bool _8SMC1Device::isAfterReset() {
		this->updateState();
		return this->state.AReset;
	}

	bool _8SMC1Device::flipPower() {
		bool current = this->getPowerState() != Power::NoPower;
		if (USMC_GetMode(dev, mode)) {
			this->devman->saveError();
			return false;
		}
		this->mode.ResetD = current;
		if (USMC_SetMode(dev, mode)) {
			this->devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Device::revertStart() {
		if(USMC_GetParameters(dev, prms)) {
			devman->saveError();
			return false;
		}
		prms.StartPos = 0;
		if(USMC_SetParameters(dev, prms)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Device::saveToFlash() {
		if (USMC_SaveParametersToFlash(dev)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Device::start(int dest, float speed,
			unsigned char divisor, bool syncIn) {
		if (USMC_GetStartParameters(dev, startPrms)) {
			devman->saveError();
			return false;
		}
		startPrms.WSyncIN = syncIn;
		startPrms.SDivisor = divisor;
		startPrms.SlStart = this->slow_start;
		if (USMC_Start(dev, dest, speed, startPrms)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Device::start(int dest, float speed) {
		if (USMC_GetStartParameters(dev, startPrms)) {
			devman->saveError();
			return false;
		}
		startPrms.SlStart = this->slow_start;
		if (USMC_Start(dev, dest, speed, startPrms)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Device::start(int dest) {
		return this->start(dest, this->speed);
	}

	bool _8SMC1Device::stop() {
		if (USMC_Stop(dev)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	float _8SMC1Device::getSpeed() {
		return this->speed;
	}

	void _8SMC1Device::setSpeed(float s) {
		this->speed = s;
	}

	bool _8SMC1Device::hasSlowStart() {
		return this->slow_start;
	}

	void _8SMC1Device::setSlowStart(bool s) {
		this->slow_start = s;
	}

	void _8SMC1Device::updateParameters() {
		if(USMC_GetParameters(dev, prms)) {
			devman->saveError();
		}
	}

	float _8SMC1Device::getAccelerationTime() {
		this->updateParameters();
		return this->prms.AccelT;
	}

	float _8SMC1Device::getDecelerationTime() {
		this->updateParameters();
		return this->prms.DecelT;
	}

	float _8SMC1Device::getMaxTemperature() {
		this->updateParameters();
		return this->prms.MaxTemp;
	}

	bool _8SMC1Device::waitsForSync() {
		return this->waitSync;
	}

	void _8SMC1Device::waitForSync(bool ws) {
		this->waitSync = ws;
	
	}

	int _8SMC1Device::getEncoderPosition() {
		if (USMC_GetEncoderState(dev, encState)) {
			devman->saveError();
		}
		return encState.EncoderPos;
	}

	int _8SMC1Device::getEncoderSyncPosition() {
		if (USMC_GetEncoderState(dev, encState)) {
			devman->saveError();
		}
		return encState.ECurPos;
	}

	void _8SMC1Device::resetSyncCounter() {
		if (USMC_GetMode(dev, mode)) {
			devman->saveError();
			return;\
		}
		mode.SyncOUTR = true;
		if (USMC_SetMode(dev, mode)) {
			devman->saveError();
		}
	}

	#define MODE_ACCESS(n1, n2, prop)\
	bool _8SMC1Device::n1() {\
		if (this->autoSaveConfig && USMC_GetMode(dev, mode)) {\
			devman->saveError();\
		}\
		return this->mode.prop;\
	}\
\
	void _8SMC1Device::n2(bool sync) {\
		if (this->autoSaveConfig && USMC_GetMode(dev, mode)) {\
			devman->saveError();\
			return;\
		}\
		mode.prop = sync;\
		if (this->autoSaveConfig && USMC_SetMode(dev, mode)) {\
			devman->saveError();\
		}\
	}

	MODE_ACCESS(isOutputSyncEnabled, setOutputSyncEnabled, SyncOUTEn)
	MODE_ACCESS(isTrailerSwapped, setTrailerSwapped, TrSwap)
	MODE_ACCESS(isTrailer1Enabled, setTrailer1Enabled, Tr1En)
	MODE_ACCESS(isTrailer2Enabled, setTrailer2Enabled, Tr2En)
	MODE_ACCESS(getSyncInputMode, setSyncInputMode, SyncINOp)
	MODE_ACCESS(isButtonDisabled, setButtonDisabled, PMode)
	MODE_ACCESS(getTrailer1TrueState, setTrailer1TrueState, Tr1T)
	MODE_ACCESS(getTrailer2TrueState, setTrailer2TrueState, Tr2T)
	#undef MODE_ACCESS

	bool _8SMC1Device::getOutputSync() {
		this->updateState();
		return this->state.SyncOUT;
	}

	bool _8SMC1Device::getInputSync() {
		this->updateState();
		return this->state.SyncIN;
	}
}