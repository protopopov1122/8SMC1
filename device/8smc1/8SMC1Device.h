#ifndef CALX_8SMC1_DEVICE_H_
#define CALX_8SMC1_DEVICE_H_

#include "device/Device.h"

#define WIN32_LEAN_AND_MEAN
#include "USMCDLL.h"

/* Standa 8SMC1 device implementation. It is excess and wraps most 8SMC1 API functions
   that are not used by other code. */

namespace CalX {
	
	// Temporary stub implementation of instrument
	
	class CALXInstrument : public Instrument {
		public:
			CALXInstrument(device_id_t, DeviceManager*);
			virtual ~CALXInstrument();
			virtual bool enable(bool);
			virtual bool enabled();
		private:
			bool state;
	};
	
	class CALXDevice : public Device {
		public:
			CALXDevice(device_id_t, DeviceManager*);
			virtual ~CALXDevice();
			/* Methods that implement Device interface */
			virtual int getPosition();		// CurPos
			virtual Power getPowerState();		// Power & FullPower
			virtual bool isRunning();		// RUN
			virtual bool isTrailerPressed(int);	// Trailer1 & Trailer2
			virtual float getTemperature();		// Temp
			virtual float getVoltage();		// Volatage
			virtual bool start(int, float,
					unsigned char, bool  = false);	// Specify location, speed, divisor
			virtual bool stop();				// Stop motor
			
			/* Some misc methods */
			void setAutoSave(bool);
			bool isAutoSaving();
			bool flush();	// Send mode and parameters to device

			// USMC_State
			/* This group of methods allows getting information from controller.
				NOTE: Each call updates state structure. */
			void updateState();	// Utilitary
			unsigned char getStepDivisor();	// SDivisor
			bool getLoftState();		// Loft
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
			bool start(int, float);			// Specify location and speed
			bool start(int);			// Specify only location
			// Other
			/* Do not directly interact with USMC API but affect start parameters */
			float getSpeed();		// Default motor speed
			void setSpeed(float);
			bool hasSlowStart();		// Motor slow start		
			void setSlowStart(bool);
			bool waitsForSync();		// Motor wait for sync
			void waitForSync(bool);
		private:
			// USMC API structures
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
}

#endif