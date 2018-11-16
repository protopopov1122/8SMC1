/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#include "device/standard/StandardDeviceManager.h"
#include <iostream>
#include <string.h>

namespace CalX {

	_8SMC1Motor::_8SMC1Motor(device_id_t id, StandardDeviceManager &devman)
	    : Motor::Motor(id), devman(devman) {
		this->speed = 1500;
		this->slow_start = false;
		this->autoSaveConfig = true;
		this->waitSync = false;
		this->aborting = false;
		memset((void *) &this->state, (int) 0, (unsigned int) sizeof(USMC_State));
		memset((void *) &this->startPrms, (int) 0,
		       (unsigned int) sizeof(USMC_StartParameters));
		memset((void *) &this->encState, (int) 0,
		       (unsigned int) sizeof(USMC_EncoderState));
		this->updateState();
		this->updateMode();
		this->updateParameters();
		getDeviceManager().loadConfiguration("8SMC1.ini", *this->config);
		ConfigEntry *motor_sect = this->config->getEntry(CalxConfiguration::Motor);
		this->divisor = motor_sect->getInt(CalxMotorConfiguration::Divisor, 8);
	}

	_8SMC1Motor::~_8SMC1Motor() {}

	DeviceManager &_8SMC1Motor::getDeviceManager() {
		return this->devman;
	}

	void _8SMC1Motor::terminate() {
		this->aborting = true;
		USMC_Stop((DWORD) id);
	}

	std::string _8SMC1Motor::getDeviceInfo() {
		return "Serial: " + this->devman.getMotorSerial(this->id) +
		       "; Version: " + this->devman.getMotorVersion(this->id);
	}

	std::string _8SMC1Motor::getRuntimeInfo() {
		return "Temperature: " + std::to_string(this->getTemperature()) +
		       "; Voltage: " + std::to_string(this->getVoltage());
	}

	bool _8SMC1Motor::enablePower(bool power) {
		Power pwr = getPowerState();
		if ((pwr == Power::NoPower && power) || (pwr != Power::NoPower && !power)) {
			return flipPower();
		}
		return true;
	}

	bool _8SMC1Motor::start(motor_coord_t dest, float speed) {
		return this->_start(dest, speed, this->divisor);
	}

	bool _8SMC1Motor::isAutoSaving() {
		return this->autoSaveConfig;
	}

	void _8SMC1Motor::setAutoSave(bool as) {
		this->autoSaveConfig = as;
	}

	bool _8SMC1Motor::flush() {
		lock();
		if (USMC_SetMode((DWORD) id, mode)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		if (USMC_SetParameters((DWORD) id, prms)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		unlock();
		return true;
	}

	bool _8SMC1Motor::setCurrentPosition(int pos) {
		lock();
		if (USMC_SetCurrentPosition((DWORD) id, pos)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		unlock();
		return true;
	}

	bool _8SMC1Motor::updateMode() {
		lock();
		if (USMC_GetMode((DWORD) id, mode)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		unlock();
		return true;
	}

	void _8SMC1Motor::updateState() {
		lock();
		if (USMC_GetState((DWORD) id, state)) {
			devman.saveMotorError();
		}
		unlock();
	}

	motor_coord_t _8SMC1Motor::getPosition() {
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

	bool _8SMC1Motor::isTrailerPressed(
	    int t) {  // Accepts trailer id, if unknown returns false
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

	Power _8SMC1Motor::getPowerState() {  // Translate power state into enum
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
		lock();
		bool current = this->getPowerState() != Power::NoPower;
		if (USMC_GetMode((DWORD) id, mode)) {
			this->devman.saveMotorError();
			unlock();
			return false;
		}
		this->mode.ResetD = current;
		if (USMC_SetMode((DWORD) id, mode)) {
			this->devman.saveMotorError();
			unlock();
			return false;
		}
		unlock();
		return true;
	}

	bool _8SMC1Motor::revertStart() {
		lock();
		if (USMC_GetParameters((DWORD) id, prms)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		prms.StartPos = 0;
		if (USMC_SetParameters((DWORD) id, prms)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		unlock();
		return true;
	}

	bool _8SMC1Motor::saveToFlash() {
		lock();
		if (USMC_SaveParametersToFlash((DWORD) id)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		unlock();
		return true;
	}

	bool _8SMC1Motor::_start(motor_coord_t dest, float speed,
	                         unsigned char divisor, bool syncIn) {
		if (this->aborting) {
			return false;
		}
		lock();
		if (USMC_GetStartParameters((DWORD) id, startPrms)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		startPrms.WSyncIN = syncIn;
		startPrms.SDivisor = divisor;
		startPrms.SlStart = this->slow_start;
		if (USMC_Start((DWORD) id, (int) dest, speed, startPrms)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		unlock();
		return true;
	}

	bool _8SMC1Motor::stop() {
		lock();
		if (USMC_Stop((DWORD) id)) {
			devman.saveMotorError();
			unlock();
			return false;
		}
		unlock();
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
		lock();
		if (USMC_GetParameters((DWORD) id, prms)) {
			devman.saveMotorError();
		}
		unlock();
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
		lock();
		if (USMC_GetEncoderState((DWORD) id, encState)) {
			devman.saveMotorError();
		}
		unlock();
		return encState.EncoderPos;
	}

	int _8SMC1Motor::getEncoderSyncPosition() {
		lock();
		if (USMC_GetEncoderState((DWORD) id, encState)) {
			devman.saveMotorError();
		}
		unlock();
		return encState.ECurPos;
	}

	void _8SMC1Motor::resetSyncCounter() {
		lock();
		if (USMC_GetMode((DWORD) id, mode)) {
			devman.saveMotorError();
			unlock();
			return;
		}
		mode.SyncOUTR = true;
		if (USMC_SetMode((DWORD) id, mode)) {
			devman.saveMotorError();
		}
		unlock();
	}

#define MODE_ACCESS(n1, n2, prop)                                              \
	bool _8SMC1Motor::n1() {                                                     \
		lock();                                                                    \
		if (this->autoSaveConfig && USMC_GetMode((DWORD) id, mode)) {              \
			devman.saveMotorError();                                                 \
		}                                                                          \
		unlock();                                                                  \
		return this->mode.prop;                                                    \
	}                                                                            \
                                                                               \
	void _8SMC1Motor::n2(bool sync) {                                            \
		lock();                                                                    \
		if (this->autoSaveConfig && USMC_GetMode((DWORD) id, mode)) {              \
			devman.saveMotorError();                                                 \
			unlock();                                                                \
			return;                                                                  \
		}                                                                          \
		mode.prop = sync;                                                          \
		if (this->autoSaveConfig && USMC_SetMode((DWORD) id, mode)) {              \
			devman.saveMotorError();                                                 \
		}                                                                          \
		unlock();                                                                  \
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
}  // namespace CalX
