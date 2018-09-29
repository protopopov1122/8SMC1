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
#include "ctrl-lib/device/DeviceManager.h"
#include "ctrl-lib/ctrl/DeviceControllerSet.h"
#include "ctrl-lib/graph/FunctionEngine.h"
#include "ctrl-lib/misc/GraphBuilder.h"
#include "ctrl-lib/plane/CoordHandle.h"
#include "ctrl-lib/plane/CoordPlane.h"
#include "ctrl-lib/plane/CoordPlaneSet.h"
#include "ctrl-lib/task/CoordTask.h"
#include "ctrl-lib/task/TaskSet.h"

/* SystemManager is component that store and control most system
   objects like devices, coord planes, tasks and configuration.
   System manager was made to use with command line, so
   GUI application may duplicate some of manager's functions if it's
   necessary. */

namespace CalX {

	class SystemManager : public TaskSetEventListener, public CoordPlaneSetListener, public DeviceControllerSetListener {
	 public:
		SystemManager(std::unique_ptr<DeviceManager>,
		              std::unique_ptr<ConfigManager>,
		              std::unique_ptr<ExtEngine> = nullptr);
		virtual ~SystemManager();
		// Main subsystems
		DeviceManager &getDeviceManager() const;
		ConfigManager &getConfiguration() const;
		FunctionEngine &getFunctionEngine();
		ExtEngine &getExtEngine() const;
		// Tasks control
		TaskSet &getTaskSet();
		// Coordinate plane control
		CoordPlaneSet &getCoordPlaneSet();
		// Device control
		MotorControllerSet &getMotorControllerSet();
		InstrumentControllerSet &getInstrumentControllerSet();

		void taskAdded(std::shared_ptr<CoordTask>) override;
		void taskRemoved(std::size_t) override;
		
		void planeAdded(std::shared_ptr<CoordHandle>) override;
		void planeRemoved(std::size_t) override;
		
		void deviceAdded(std::shared_ptr<DeviceController>) override;
	 private:		
		const std::unique_ptr<DeviceManager> devman;
		const std::unique_ptr<ConfigManager> conf;
		FunctionEngine engine;
		const std::unique_ptr<ExtEngine> ext_engine;
		MotorControllerSet motorControllerSet;
		InstrumentControllerSet instrumentControllerSet;
		VectorCoordPlaneSet planeSet;
		VectorTaskSet taskSet;
	};
}  // namespace CalX

#endif
