#ifndef _8SMC1_CTRL_LIB_SYSTEM_MANAGER_H_
#define _8SMC1_CTRL_LIB_SYSTEM_MANAGER_H_

#include "device/DeviceManager.h"
#include "plane/CoordPlane.h"
#include "task/CoordTask.h"
#include "misc/GraphBuilder.h"
#include "graph/FunctionEngine.h"

namespace _8SMC1 {

	class SystemManager {
		public:
			SystemManager(DeviceManager*);
			virtual ~SystemManager();
			DeviceManager *getDeviceManager();
			DeviceController *getDeviceController(DWORD);
			size_t getDeviceCount();
			FunctionEngine *getFunctionEngine();
			// Tasks control
			size_t getTaskCount();
			CoordTask *getTask(size_t);
			ProgrammedCoordTask *createProgrammedTask();
			size_t addTask(CoordTask*);
			bool removeTask(size_t);
			// Coordinate plane control
			size_t getCoordCount();
			CoordPlaneStack *getCoord(size_t);
			CoordController *getCoordController(size_t);
			CoordPlaneStack *createCoord(DWORD, DWORD);
		private:
			DeviceManager *devman;
			std::vector<DeviceController*> dev;
			std::vector<CoordTask*> tasks;
			std::vector<CoordPlaneStack*> coords;
			std::vector<CoordController*> coordCtrl;
			FunctionEngine engine;

	};

}


#endif
