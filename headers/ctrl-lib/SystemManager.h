/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

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
#include "ctrl-lib/conf/Dictionary.h"
#include "ctrl-lib/conf/Settings.h"
#include "ctrl-lib/device/DeviceManager.h"
#include "ctrl-lib/ctrl/DeviceControllerSet.h"
#include "ctrl-lib/graph/MathEngine.h"
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

	class SystemManager {
	 public:
		virtual ~SystemManager() = default;
		virtual DeviceManager &getDeviceManager() const = 0;
		virtual ConfigurationCatalogue &getConfiguration() const = 0;
		virtual SettingsRepository *getSettingsRepository() const = 0;
		virtual MathEngine &getMathEngine() = 0;
		virtual ExtEngine *getExtensionEngine() const = 0;
		virtual CoordPlaneSet &getCoordPlaneSet() = 0;
		virtual MotorControllerSet &getMotorControllerSet() = 0;
		virtual InstrumentControllerSet &getInstrumentControllerSet() = 0;
	};

	class SystemManagerEventLogger : public CoordPlaneSetListener,
	                                 public DeviceControllerSetListener {
	 public:
		SystemManagerEventLogger(SystemManager &);

		void onPlaneAdded(std::shared_ptr<CoordHandle>) override;
		void onPlaneRemoving(std::size_t) override;

		void onDeviceConnected(std::shared_ptr<DeviceController>) override;

	 private:
		SystemManager &sysman;
	};

	class DefaultSystemManager : public SystemManager {
	 public:
		DefaultSystemManager(std::unique_ptr<DeviceManager>,
		                     std::unique_ptr<ConfigurationCatalogue>,
		                     std::unique_ptr<ExtEngine> = nullptr);
		virtual ~DefaultSystemManager();
		DeviceManager &getDeviceManager() const override;
		ConfigurationCatalogue &getConfiguration() const override;
		SettingsRepository *getSettingsRepository() const override;
		MathEngine &getMathEngine() override;
		ExtEngine *getExtensionEngine() const override;
		CoordPlaneSet &getCoordPlaneSet() override;
		MotorControllerSet &getMotorControllerSet() override;
		InstrumentControllerSet &getInstrumentControllerSet() override;

	 private:
		const std::unique_ptr<DeviceManager> devman;
		const std::unique_ptr<ConfigurationCatalogue> conf;
		std::unique_ptr<SettingsRepository> settings;
		DefaultMathEngine engine;
		const std::unique_ptr<ExtEngine> ext_engine;
		SystemManagerEventLogger eventLogger;
		VectorMotorControllerSet motorControllerSet;
		VectorInstrumentControllerSet instrumentControllerSet;
		VectorCoordPlaneSet planeSet;
	};
}  // namespace CalX

#endif
