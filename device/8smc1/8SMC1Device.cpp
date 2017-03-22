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
#include "8SMC1DeviceManager.h"

namespace CalX {
	
	CALXInstrument::CALXInstrument(device_id_t id, std::string port, DeviceManager *devman) {
		this->dev = id;
		this->devman = devman;
		this->state = false;
	}
	
	CALXInstrument::~CALXInstrument() {
		
	}
	
	bool CALXInstrument::enable(bool en) {
		std::cout << "Instrument #" << dev << " " << (en ? "enabled" : "disabled") << std::endl;
		state = en;
		return true;
	}
	
	bool CALXInstrument::enabled() {
		return state;
	}
	
	CALXDevice::CALXDevice(device_id_t dev, DeviceManager *devman) {
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

	CALXDevice::~CALXDevice() {

	}

	bool CALXDevice::isAutoSaving() {
		return this->autoSaveConfig;
	}

	void CALXDevice::setAutoSave(bool as) {
		this->autoSaveConfig = as;
	}

	bool CALXDevice::flush() {
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

	bool CALXDevice::setCurrentPosition(int pos) {
		if (USMC_SetCurrentPosition(dev, pos)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool CALXDevice::updateMode() {
		if (USMC_GetMode(dev, mode)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	void CALXDevice::updateState() {
		if (USMC_GetState(dev, state)) {
			devman->saveError();
		}
	}

	int CALXDevice::getPosition() {
		this->updateState();
		return this->state.CurPos;
	}

	float CALXDevice::getTemperature() {
		this->updateState();
		return this->state.Temp;
	}

	float CALXDevice::getVoltage() {
		this->updateState();
		return this->state.Voltage;
	}

	unsigned char CALXDevice::getStepDivisor() {
		this->updateState();
		return this->state.SDivisor;
	}

	bool CALXDevice::isTrailerPressed(int t) {	// Accepts trailer id, if unknown returns false
		this->updateState();
		if (t == 1) {
			return this->state.Trailer1;
		}
		if (t == 2) {
			return this->state.Trailer2;
		}
		return false;
	}

	bool CALXDevice::getLoftState() {
		this->updateState();
		return this->state.Loft;
	}

	Power CALXDevice::getPowerState() {		// Translate power state into enum
		this->updateState();
		if (this->state.Power && this->state.FullPower) {
			return Power::FullPower;
		} else if (this->state.Power) {
			return Power::HalfPower;
		} else {
			return Power::NoPower;
		}
	}
	
	bool CALXDevice::isRunning() {
		this->updateState();
		return this->state.RUN;
	}

	bool CALXDevice::hasFullSpeed() {
		this->updateState();
		return this->state.FullSpeed;
	}

	bool CALXDevice::isAfterReset() {
		this->updateState();
		return this->state.AReset;
	}

	bool CALXDevice::flipPower() {
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

	bool CALXDevice::revertStart() {
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

	bool CALXDevice::saveToFlash() {
		if (USMC_SaveParametersToFlash(dev)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool CALXDevice::start(int dest, float speed,
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

	bool CALXDevice::start(int dest, float speed) {
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

	bool CALXDevice::start(int dest) {
		return this->start(dest, this->speed);
	}

	bool CALXDevice::stop() {
		if (USMC_Stop(dev)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	float CALXDevice::getSpeed() {
		return this->speed;
	}

	void CALXDevice::setSpeed(float s) {
		this->speed = s;
	}

	bool CALXDevice::hasSlowStart() {
		return this->slow_start;
	}

	void CALXDevice::setSlowStart(bool s) {
		this->slow_start = s;
	}

	void CALXDevice::updateParameters() {
		if(USMC_GetParameters(dev, prms)) {
			devman->saveError();
		}
	}

	float CALXDevice::getAccelerationTime() {
		this->updateParameters();
		return this->prms.AccelT;
	}

	float CALXDevice::getDecelerationTime() {
		this->updateParameters();
		return this->prms.DecelT;
	}

	float CALXDevice::getMaxTemperature() {
		this->updateParameters();
		return this->prms.MaxTemp;
	}

	bool CALXDevice::waitsForSync() {
		return this->waitSync;
	}

	void CALXDevice::waitForSync(bool ws) {
		this->waitSync = ws;
	
	}

	int CALXDevice::getEncoderPosition() {
		if (USMC_GetEncoderState(dev, encState)) {
			devman->saveError();
		}
		return encState.EncoderPos;
	}

	int CALXDevice::getEncoderSyncPosition() {
		if (USMC_GetEncoderState(dev, encState)) {
			devman->saveError();
		}
		return encState.ECurPos;
	}

	void CALXDevice::resetSyncCounter() {
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
	bool CALXDevice::n1() {\
		if (this->autoSaveConfig && USMC_GetMode(dev, mode)) {\
			devman->saveError();\
		}\
		return this->mode.prop;\
	}\
\
	void CALXDevice::n2(bool sync) {\
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

	bool CALXDevice::getOutputSync() {
		this->updateState();
		return this->state.SyncOUT;
	}

	bool CALXDevice::getInputSync() {
		this->updateState();
		return this->state.SyncIN;
	}
}