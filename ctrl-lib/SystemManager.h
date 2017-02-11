#ifndef _8SMC1_CTRL_LIB_SYSTEM_MANAGER_H_
#define _8SMC1_CTRL_LIB_SYSTEM_MANAGER_H_

#include "DeviceManager.h"
#include "DeviceController.h"
#include "CoordTask.h"

namespace Controller {

	class SystemManager {
		public:
			SystemManager(DeviceManager*);
			virtual ~SystemManager();
			DeviceManager *getDeviceManager();
			DeviceController *getDeviceController(DWORD);
			size_t getDeviceCount();
			// Tasks control
			size_t getTaskCount();
			CoordTask *getTask(size_t);
			CoordTask *createTask();
			bool removeTask(size_t);
			// Coordinate plane control
			size_t getCoordCount();
			CoordController *getCoord(size_t);
			CoordController *createCoord(DWORD, DWORD);
		private:
			DeviceManager *devman;
			std::vector<DeviceController*> dev;
			std::vector<CoordTask*> tasks;
			std::vector<CoordController*> coords;

	};

}


#endif
