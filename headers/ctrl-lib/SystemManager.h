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

#include "ctrl-lib/device/DeviceManager.h"
#include "ctrl-lib/ctrl/InstrumentController.h"
#include "ctrl-lib/conf/ConfigManager.h"
#include "ctrl-lib/plane/CoordPlane.h"
#include "ctrl-lib/plane/CoordHandle.h"
#include "ctrl-lib/task/CoordTask.h"
#include "ctrl-lib/misc/GraphBuilder.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include "ctrl-lib/conf/RequestResolver.h"
#include "ctrl-lib/ExtEngine.h"

/* SystemManager is component that store and control most system
   objects. System manager was made to use with command line, so
   in GUI application may duplicate its functions if it's
   necessary. */

namespace CalX {

  class SystemManager {
   public:
	SystemManager(DeviceManager *, ConfigManager *, ExtEngine * = nullptr);
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
	size_t addTask(CoordTask *);
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
	DeviceManager *devman;
	ConfigManager *conf;
	std::vector<MotorController *> dev;
	std::vector<InstrumentController *> instr;
	std::vector<CoordTask *> tasks;
	std::vector<CoordHandle *> coords;
	FunctionEngine engine;
	RequestResolver *resolver;
	ExtEngine *ext_engine;
  };
}

#endif
