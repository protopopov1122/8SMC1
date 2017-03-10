#ifndef CALX_CTRL_LIB_SYSTEM_MANAGER_H_
#define CALX_CTRL_LIB_SYSTEM_MANAGER_H_

#include "device/DeviceManager.h"
#include "plane/CoordPlane.h"
#include "CoordHandle.h"
#include "task/CoordTask.h"
#include "misc/GraphBuilder.h"
#include "graph/FunctionEngine.h"

/* SystemManager is component that store and control most system 
   objects. System manager was made to use with command line, so
   in GUI application may duplicate its functions if it's
   necessary. */

namespace CalX {

	class SystemManager {
		public:
			SystemManager(DeviceManager*);
			virtual ~SystemManager();
			DeviceManager *getDeviceManager();
			DeviceController *getDeviceController(device_id_t);
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
			CoordHandle *getCoord(size_t);
			CoordHandle* createCoord(device_id_t, device_id_t);
			void removeCoord(size_t);
		private:
			DeviceManager *devman;
			std::vector<DeviceController*> dev;
			std::vector<CoordTask*> tasks;
			std::vector<CoordHandle*> coords;
			FunctionEngine engine;

	};

}


#endif
