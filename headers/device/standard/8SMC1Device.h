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

#ifndef CALX_DEVICE_STANDARD_8SMC1_DEVICE_H_
#define CALX_DEVICE_STANDARD_8SMC1_DEVICE_H_

#include "ctrl-lib/device/Device.h"
#include "device/standard/NL300Instrument.h"
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include "USMCDLL.h"
#include <windows.h>

/* Standa 8SMC1 device implementation. It is excess and wraps most 8SMC1 API
   functions
   that are not used by other code. */

namespace CalX {

	class StandardDeviceManager;  // Forward referencing

	class _8SMC1Motor : public Motor {
	 public:
		_8SMC1Motor(device_id_t, StandardDeviceManager &);
		virtual ~_8SMC1Motor();
		virtual void terminate();
		virtual DeviceManager &getDeviceManager();
		virtual std::string getDeviceInfo();
		virtual std::string getRuntimeInfo();
		/* Methods that implement Device interface */
		virtual motor_coord_t getPosition();       // CurPos
		virtual Power getPowerState();             // Power & FullPower
		virtual bool isRunning();                  // RUN
		virtual bool isTrailerPressed(int);        // Trailer1 & Trailer2
		virtual bool start(motor_coord_t, float);  // Specify location, speed
		virtual bool stop();                       // Stop motor
		virtual bool enablePower(bool);            // Power control

		/* Some misc methods */
		virtual bool _start(motor_coord_t, float, unsigned char,
		                    bool = false);  // Specify location, speed
		void setAutoSave(bool);
		bool isAutoSaving();
		bool flush();  // Send mode and parameters to device

		virtual float getTemperature();  // Temp
		virtual float getVoltage();      // Volatage
		// USMC_State
		/* This group of methods allows getting information from controller.
		        NOTE: Each call updates state structure. */
		void updateState();              // Utilitary
		unsigned char getStepDivisor();  // SDivisor
		bool getLoftState();             // Loft
		bool hasFullSpeed();             // FullSpeed
		bool isAfterReset();             // AReset
		bool getOutputSync();            // SyncOUT
		bool getInputSync();             // SyncIN
		// USMC_Encoder
		/* Getting info from encoder */
		int getEncoderPosition();      // EncodePos
		int getEncoderSyncPosition();  // ECusPos
		// USMC_Parameters related
		/* Reading controller parameters */
		void updateParameters();      // Utilitary
		float getAccelerationTime();  // AccelT
		float getDecelerationTime();  // DecelT
		float getMaxTemperature();    // MaxTemp
		// USMC_Mode changes
		/* Controller mode changes */
		bool updateMode();           // Utilitary
		bool flipPower();            // ResetD
		bool isOutputSyncEnabled();  // SyncOUTEn
		void setOutputSyncEnabled(bool);
		bool isTrailerSwapped();  // TrSwap
		void setTrailerSwapped(bool);
		bool isTrailer1Enabled();  // Tr1En
		void setTrailer1Enabled(bool);
		bool isTrailer2Enabled();  // Tr2En
		void setTrailer2Enabled(bool);
		bool getSyncInputMode();  // SyncINOp
		void setSyncInputMode(bool);
		bool isButtonDisabled();  // PMode
		void setButtonDisabled(bool);
		bool getTrailer1TrueState();  // Tr1T
		void setTrailer1TrueState(bool);
		bool getTrailer2TrueState();  // Tr2T
		void setTrailer2TrueState(bool);
		void resetSyncCounter();  // SyncOUTR
		// USMC_Parameters changes
		bool revertStart();            // StartPos
		bool setCurrentPosition(int);  // USMC_SetCurrentPosition
		bool saveToFlash();            // USMC_SaveParametersToFlash
		// Other
		/* Do not directly interact with USMC API but affect start parameters */
		float getSpeed();  // Default motor speed
		void setSpeed(float);
		bool hasSlowStart();  // Motor slow start
		void setSlowStart(bool);
		bool waitsForSync();  // Motor wait for sync
		void waitForSync(bool);

	 private:
		bool aborting;
		StandardDeviceManager &devman;
		// USMC API structures
		USMC_State state;
		USMC_StartParameters startPrms;
		USMC_Parameters prms;
		USMC_Mode mode;
		USMC_EncoderState encState;

		// Settings
		bool autoSaveConfig;  // Save config on modification
		// Presaved movement parameters
		bool slow_start;
		float speed;
		bool waitSync;
		int divisor;
	};
}  // namespace CalX

#endif
