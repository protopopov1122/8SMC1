/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_CTRL_LIB_SYSTEM_MANAGER_H_
#define CALX_CTRL_LIB_SYSTEM_MANAGER_H_

#include <memory>
#include "ctrl-lib/ExtEngine.h"
#include "ctrl-lib/conf/ConfigManager.h"
#include "ctrl-lib/conf/RequestResolver.h"
#include "ctrl-lib/ctrl/InstrumentController.h"
#include "ctrl-lib/device/DeviceManager.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include "ctrl-lib/misc/GraphBuilder.h"
#include "ctrl-lib/plane/CoordHandle.h"
#include "ctrl-lib/plane/CoordPlane.h"
#include "ctrl-lib/task/CoordTask.h"

/* SystemManager is component that store and control most system
   objects like devices, coord planes, tasks and configuration.
   System manager was made to use with command line, so
   in GUI application may duplicate its functions if it's
   necessary. */

namespace CalX {

	class SystemManager {
	 public:
		SystemManager(std::unique_ptr<DeviceManager>, std::unique_ptr<ConfigManager>, std::unique_ptr<ExtEngine> = nullptr);
		virtual ~SystemManager();
		DeviceManager *getDeviceManager();
		ConfigManager *getConfiguration();
		MotorController *getMotorController(device_id_t);
		size_t getMotorCount();
		InstrumentController *getInstrumentController(device_id_t);
		size_t getInstrumentCount();
		FunctionEngine *getFunctionEngine();
		RequestResolver *getRequestResolver();
		ExtEngine *getExtEngine();
		// Tasks control
		size_t getTaskCount();
		CoordTask *getTask(size_t);
		ProgrammedCoordTask *createProgrammedTask();
		size_t addTask(std::unique_ptr<CoordTask>);
		bool removeTask(size_t);
		// Coordinate plane control
		size_t getCoordCount();
		CoordHandle *getCoord(size_t);
		CoordHandle *createCoord(device_id_t, device_id_t, device_id_t = -1);
		void removeCoord(size_t);
		// Device connection
		MotorController *connectMotor(DeviceConnectionPrms *);
		InstrumentController *connectInstrument(DeviceConnectionPrms *);

	 private:
		std::unique_ptr<DeviceManager> devman;
		std::unique_ptr<ConfigManager> conf;
		std::vector<std::unique_ptr<MotorController>> dev;
		std::vector<std::unique_ptr<InstrumentController>> instr;
		std::vector<std::unique_ptr<CoordTask>> tasks;
		std::vector<std::unique_ptr<CoordHandle>> coords;
		FunctionEngine engine;
		std::unique_ptr<RequestResolver> resolver;
		std::unique_ptr<ExtEngine> ext_engine;
	};
}

#endif
