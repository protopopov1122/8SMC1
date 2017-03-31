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


#ifndef CALX_CTRL_LIB_SYSTEM_MANAGER_H_
#define CALX_CTRL_LIB_SYSTEM_MANAGER_H_

#include "device/DeviceManager.h"
#include "InstrumentController.h"
#include "ConfigManager.h"
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
			SystemManager(DeviceManager*, ConfigManager*);
			virtual ~SystemManager();
			DeviceManager *getDeviceManager();
			ConfigManager *getConfiguration();
			DeviceController *getDeviceController(device_id_t);
			size_t getDeviceCount();
			InstrumentController *getInstrumentController(device_id_t);
			size_t getInstrumentCount();
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
			CoordHandle* createCoord(device_id_t, device_id_t, device_id_t = -1);
			void removeCoord(size_t);
			// Device connection
			DeviceController *connectDevice(DeviceConnectionPrms*);
			InstrumentController *connectInstrument(DeviceConnectionPrms*);
		private:
			DeviceManager *devman;
			ConfigManager *conf;
			std::vector<DeviceController*> dev;
			std::vector<InstrumentController*> instr;
			std::vector<CoordTask*> tasks;
			std::vector<CoordHandle*> coords;
			FunctionEngine engine;

	};

}


#endif
