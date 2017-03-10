#ifndef _8SMC1_DEVICE_H_
#define _8SMC1_DEVICE_H_

#include <cinttypes>
#include <string>

/* Device class contains interface to abstract Device. */

namespace _8SMC1 {

	class DeviceManager;	// For forward referencing

	typedef int64_t device_id_t;

	enum Power {
		NoPower, HalfPower, FullPower
	};
	
	class Device {
		public:
			virtual ~Device();
			virtual DeviceManager *getDeviceManager();
			virtual device_id_t getID();
			virtual std::string getSerial();
			virtual std::string getVersion();
			virtual int getPosition() = 0;
			virtual bool isTrailerPressed(int) = 0;
			virtual bool isRunning() = 0;
			virtual Power getPowerState() = 0;
			virtual bool start(int, float,
					unsigned char, bool  = false) = 0;
			virtual bool stop() = 0;
			virtual bool flipPower() = 0;
			virtual float getTemperature() = 0;
			virtual float getVoltage() = 0;
		protected:
			device_id_t dev;
			DeviceManager *devman;
	};

}

#endif
