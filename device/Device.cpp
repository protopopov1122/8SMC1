#include "DeviceManager.h"

/* Implementations of API wrappers */

namespace Controller {


	Device::Device(DWORD dev, DeviceManager *devman) {
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

	Device::~Device() {

	}

	DeviceManager *Device::getDeviceManager() {
		return this->devman;
	}

	bool Device::isAutoSaving() {
		return this->autoSaveConfig;
	}

	void Device::setAutoSave(bool as) {
		this->autoSaveConfig = as;
	}

	bool Device::flush() {
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

	DWORD Device::getID() {
		return this->dev;
	}

	bool Device::setCurrentPosition(int pos) {
		if (USMC_SetCurrentPosition(dev, pos)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool Device::updateMode() {
		if (USMC_GetMode(dev, mode)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	void Device::updateState() {
		if (USMC_GetState(dev, state)) {
			devman->saveError();
		}
	}

	std::string Device::getSerial() {
		return this->devman->getDeviceSerial(this->dev);
	}

	std::string Device::getVersion() {
		return this->devman->getDeviceVersion(this->dev);
	}

	int Device::getPosition() {
		this->updateState();
		return this->state.CurPos;
	}

	float Device::getTemperature() {
		this->updateState();
		return this->state.Temp;
	}

	float Device::getVoltage() {
		this->updateState();
		return this->state.Voltage;
	}

	unsigned char Device::getStepDivisor() {
		this->updateState();
		return this->state.SDivisor;
	}

	bool Device::isTrailerPressed(int t) {	// Accepts trailer id, if unknown returns false
		this->updateState();
		if (t == 1) {
			return this->state.Trailer1;
		}
		if (t == 2) {
			return this->state.Trailer2;
		}
		return false;
	}

	bool Device::getLoftState() {
		this->updateState();
		return this->state.Loft;
	}

	Power Device::getPowerState() {		// Translate power state into enum
		this->updateState();
		if (this->state.Power && this->state.FullPower) {
			return Power::FullPower;
		} else if (this->state.Power) {
			return Power::HalfPower;
		} else {
			return Power::NoPower;
		}
	}
	
	bool Device::isRunning() {
		this->updateState();
		return this->state.RUN;
	}

	bool Device::hasFullSpeed() {
		this->updateState();
		return this->state.FullSpeed;
	}

	bool Device::isAfterReset() {
		this->updateState();
		return this->state.AReset;
	}

	bool Device::flipPower() {
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

	bool Device::revertStart() {
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

	bool Device::saveToFlash() {
		if (USMC_SaveParametersToFlash(dev)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	bool Device::start(int dest, float speed, unsigned char divisor) {
		if (USMC_GetStartParameters(dev, startPrms)) {
			devman->saveError();
			return false;
		}
		startPrms.SDivisor = divisor;
		startPrms.SlStart = this->slow_start;
		if (USMC_Start(dev, dest, speed, startPrms)) {
			devman->saveError();
			return false;
		}
		this->devman->setLastDevice(this);
		return true;
	}

	bool Device::start(int dest, float speed) {
		if (USMC_GetStartParameters(dev, startPrms)) {
			devman->saveError();
			return false;
		}
		startPrms.SlStart = this->slow_start;
		if (USMC_Start(dev, dest, speed, startPrms)) {
			devman->saveError();
			return false;
		}
		this->devman->setLastDevice(this);
		return true;
	}

	bool Device::start(int dest) {
		return this->start(dest, this->speed);
	}

	bool Device::stop() {
		if (USMC_Stop(dev)) {
			devman->saveError();
			return false;
		}
		return true;
	}

	float Device::getSpeed() {
		return this->speed;
	}

	void Device::setSpeed(float s) {
		this->speed = s;
	}

	bool Device::hasSlowStart() {
		return this->slow_start;
	}

	void Device::setSlowStart(bool s) {
		this->slow_start = s;
	}

	void Device::updateParameters() {
		if(USMC_GetParameters(dev, prms)) {
			devman->saveError();
		}
	}

	float Device::getAccelerationTime() {
		this->updateParameters();
		return this->prms.AccelT;
	}

	float Device::getDecelerationTime() {
		this->updateParameters();
		return this->prms.DecelT;
	}

	float Device::getMaxTemperature() {
		this->updateParameters();
		return this->prms.MaxTemp;
	}

	bool Device::waitsForSync() {
		return this->waitSync;
	}

	void Device::waitForSync(bool ws) {
		this->waitSync = ws;
	
	}

	int Device::getEncoderPosition() {
		if (USMC_GetEncoderState(dev, encState)) {
			devman->saveError();
		}
		return encState.EncoderPos;
	}

	int Device::getEncoderSyncPosition() {
		if (USMC_GetEncoderState(dev, encState)) {
			devman->saveError();
		}
		return encState.ECurPos;
	}

	void Device::resetSyncCounter() {
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
	bool Device::n1() {\
		if (this->autoSaveConfig && USMC_GetMode(dev, mode)) {\
			devman->saveError();\
		}\
		return this->mode.prop;\
	}\
\
	void Device::n2(bool sync) {\
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

	bool Device::getOutputSync() {
		this->updateState();
		return this->state.SyncOUT;
	}

	bool Device::getInputSync() {
		this->updateState();
		return this->state.SyncIN;
	}
}
