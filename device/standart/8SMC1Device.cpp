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
#include "StandartDeviceManager.h"

namespace CalX {
	
	_8SMC1Motor::_8SMC1Motor(device_id_t dev, StandartDeviceManager *devman) {
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

	_8SMC1Motor::~_8SMC1Motor() {
	}
	
	DeviceManager *_8SMC1Motor::getDeviceManager() {
		return this->devman;
	}

	std::string _8SMC1Motor::getDeviceInfo() {
		return "Serial: " + this->devman->getMotorSerial(this->dev) +
			"; Version: " + this->devman->getMotorVersion(this->dev);
	}
	
	bool _8SMC1Motor::enablePower(bool power) {
		Power pwr = getPowerState();
		if ((pwr == Power::NoPower &&
				power) ||
			(pwr != Power::NoPower &&
				!power)) {
			return flipPower();		
		}
		return true;
	}

	bool _8SMC1Motor::isAutoSaving() {
		return this->autoSaveConfig;
	}

	void _8SMC1Motor::setAutoSave(bool as) {
		this->autoSaveConfig = as;
	}

	bool _8SMC1Motor::flush() {
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

	bool _8SMC1Motor::setCurrentPosition(int pos) {
		if (USMC_SetCurrentPosition(dev, pos)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Motor::updateMode() {
		if (USMC_GetMode(dev, mode)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	void _8SMC1Motor::updateState() {
		if (USMC_GetState(dev, state)) {
			devman->saveError();
		}
	}

	int _8SMC1Motor::getPosition() {
		this->updateState();
		return this->state.CurPos;
	}

	float _8SMC1Motor::getTemperature() {
		this->updateState();
		return this->state.Temp;
	}

	float _8SMC1Motor::getVoltage() {
		this->updateState();
		return this->state.Voltage;
	}

	unsigned char _8SMC1Motor::getStepDivisor() {
		this->updateState();
		return this->state.SDivisor;
	}

	bool _8SMC1Motor::isTrailerPressed(int t) {	// Accepts trailer id, if unknown returns false
		this->updateState();
		if (t == 1) {
			return this->state.Trailer1;
		}
		if (t == 2) {
			return this->state.Trailer2;
		}
		return false;
	}

	bool _8SMC1Motor::getLoftState() {
		this->updateState();
		return this->state.Loft;
	}

	Power _8SMC1Motor::getPowerState() {		// Translate power state into enum
		this->updateState();
		if (this->state.Power && this->state.FullPower) {
			return Power::FullPower;
		} else if (this->state.Power) {
			return Power::HalfPower;
		} else {
			return Power::NoPower;
		}
	}
	
	bool _8SMC1Motor::isRunning() {
		this->updateState();
		return this->state.RUN;
	}

	bool _8SMC1Motor::hasFullSpeed() {
		this->updateState();
		return this->state.FullSpeed;
	}

	bool _8SMC1Motor::isAfterReset() {
		this->updateState();
		return this->state.AReset;
	}

	bool _8SMC1Motor::flipPower() {
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

	bool _8SMC1Motor::revertStart() {
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

	bool _8SMC1Motor::saveToFlash() {
		if (USMC_SaveParametersToFlash(dev)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool _8SMC1Motor::start(int dest, float speed,
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

	bool _8SMC1Motor::start(int dest, float speed) {
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

	bool _8SMC1Motor::start(int dest) {
		return this->start(dest, this->speed);
	}

	bool _8SMC1Motor::stop() {
		if (USMC_Stop(dev)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	float _8SMC1Motor::getSpeed() {
		return this->speed;
	}

	void _8SMC1Motor::setSpeed(float s) {
		this->speed = s;
	}

	bool _8SMC1Motor::hasSlowStart() {
		return this->slow_start;
	}

	void _8SMC1Motor::setSlowStart(bool s) {
		this->slow_start = s;
	}

	void _8SMC1Motor::updateParameters() {
		if(USMC_GetParameters(dev, prms)) {
			devman->saveError();
		}
	}

	float _8SMC1Motor::getAccelerationTime() {
		this->updateParameters();
		return this->prms.AccelT;
	}

	float _8SMC1Motor::getDecelerationTime() {
		this->updateParameters();
		return this->prms.DecelT;
	}

	float _8SMC1Motor::getMaxTemperature() {
		this->updateParameters();
		return this->prms.MaxTemp;
	}

	bool _8SMC1Motor::waitsForSync() {
		return this->waitSync;
	}

	void _8SMC1Motor::waitForSync(bool ws) {
		this->waitSync = ws;
	
	}

	int _8SMC1Motor::getEncoderPosition() {
		if (USMC_GetEncoderState(dev, encState)) {
			devman->saveError();
		}
		return encState.EncoderPos;
	}

	int _8SMC1Motor::getEncoderSyncPosition() {
		if (USMC_GetEncoderState(dev, encState)) {
			devman->saveError();
		}
		return encState.ECurPos;
	}

	void _8SMC1Motor::resetSyncCounter() {
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
	bool _8SMC1Motor::n1() {\
		if (this->autoSaveConfig && USMC_GetMode(dev, mode)) {\
			devman->saveError();\
		}\
		return this->mode.prop;\
	}\
\
	void _8SMC1Motor::n2(bool sync) {\
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

	bool _8SMC1Motor::getOutputSync() {
		this->updateState();
		return this->state.SyncOUT;
	}

	bool _8SMC1Motor::getInputSync() {
		this->updateState();
		return this->state.SyncIN;
	}
}