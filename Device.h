#ifndef _8SMC1_DEVICE_H_
#define _8SMC1_DEVICE_H_

#include <string>
#include "USMCDLL.h"

/* Device class contains USMC raw function wrappers */

namespace Controller {

	class DeviceManager;	// For forward referencing

	enum Power {
		NoPower, HalfPower, FullPower
	};

	class Device {
		public:
			Device(DWORD, DeviceManager*);
			~Device();
			/* Some misc methods */
			DeviceManager *getDeviceManager();
			void setAutoSave(bool);
			bool isAutoSaving();
			bool flush();	// Send mode and parameters to device
			/* Getting device info */
			DWORD getID();
			std::string getSerial();
			std::string getVersion();

			// USMC_State
			/* This group of methods allows getting information from controller.
				NOTE: Each call updates state structure. */
			void updateState();	// Utilitary
			int getPosition();		// CurPos
			float getTemperature();		// Temp
			float getVoltage();		// Volatage
			unsigned char getStepDivisor();	// SDivisor
			bool isTrailerPressed(int);	// Trailer1 & Trailer2
			bool getLoftState();		// Loft
			Power getPowerState();		// Power & FullPower
			bool isRunning();		// RUN
			bool hasFullSpeed();		// FullSpeed
			bool isAfterReset();		// AReset
			bool getOutputSync();		// SyncOUT
			bool getInputSync();		// SyncIN
			// USMC_Encoder
			/* Getting info from encoder */
			int getEncoderPosition();	// EncodePos
			int getEncoderSyncPosition();	// ECusPos
			// USMC_Parameters related
			/* Reading controller parameters */
			void updateParameters();	// Utilitary
			float getAccelerationTime();	// AccelT
			float getDecelerationTime();	// DecelT
			float getMaxTemperature();	// MaxTemp
			// USMC_Mode changes
			/* Controller mode changes */
			bool updateMode();		// Utilitary
			bool flipPower();		// ResetD
			bool isOutputSyncEnabled();	// SyncOUTEn
			void setOutputSyncEnabled(bool);
			bool isTrailerSwapped();	// TrSwap
			void setTrailerSwapped(bool);
			bool isTrailer1Enabled();	// Tr1En
			void setTrailer1Enabled(bool);
			bool isTrailer2Enabled();	// Tr2En
			void setTrailer2Enabled(bool);
			bool getSyncInputMode();	// SyncINOp
			void setSyncInputMode(bool);
			bool isButtonDisabled();	// PMode
			void setButtonDisabled(bool);
			bool getTrailer1TrueState();	// Tr1T
			void setTrailer1TrueState(bool);
			bool getTrailer2TrueState();	// Tr2T
			void setTrailer2TrueState(bool);
			void resetSyncCounter();	// SyncOUTR
			// USMC_Parameters changes
			bool revertStart();		// StartPos
			bool setCurrentPosition(int);	// USMC_SetCurrentPosition
			bool saveToFlash();		// USMC_SaveParametersToFlash
			// Motor moving
			/* Accept movement parameters or use presaved */
			bool start(int, float, unsigned char);	// Specify location, speed, divisor 
			bool start(int, float);			// Specify location and speed
			bool start(int);			// Specify only location
			bool stop();				// Stop motor
			// Other
			/* Do not directly interact with USMC API but affect start parameters */
			float getSpeed();		// Default motor speed
			void setSpeed(float);
			bool hasSlowStart();		// Motor slow start		
			void setSlowStart(bool);
			bool waitsForSync();		// Motor wait for sync
			void waitForSync(bool);
		private:
			DeviceManager *devman;
			// USMC API structures
			DWORD dev;
			USMC_State state;
			USMC_StartParameters startPrms;
			USMC_Parameters prms;
			USMC_Mode mode;
			USMC_EncoderState encState;

			// Settings
			bool autoSaveConfig; // Save config on modification
			// Presaved movement parameters
			bool slow_start;
			float speed;
			bool waitSync;
	};


	/* Used to specify movement with parameters different from default */
	class MotorTask {
		public:
			MotorTask(int, Device*);
			virtual ~MotorTask();
			MotorTask *setSpeed(float);
			MotorTask *setStepDivisor(int);
			MotorTask *setSlowStart(bool);
			MotorTask *setWaitSync(bool);
			bool start();
		private:
			Device *dev;
			int dest;
			float speed;
			int step_divisor;
			bool slow_start;
			bool wait_sync;
	};
}

#endif
