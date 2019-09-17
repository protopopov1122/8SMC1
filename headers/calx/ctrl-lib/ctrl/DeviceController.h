/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_CTRL_LIB_DEVICE_CONTROLLER_H_
#define CALX_CTRL_LIB_DEVICE_CONTROLLER_H_

#include "calx/ctrl-lib/CtrlCore.h"
#include "calx/ctrl-lib/Resource.h"
#include "calx/ctrl-lib/EventListener.h"
#include "calx/ctrl-lib/conf/Dictionary.h"
#include "calx/ctrl-lib/device/DeviceManager.h"
#include <cinttypes>
#include <vector>

namespace CalX {

	class DeviceController : public UsableResource {
	 public:
		DeviceController(ConfigurationCatalogue &, Device &);
		virtual ~DeviceController() = default;
		device_id_t getID() const;
		ConfigurationCatalogue &getConfiguration() const;
		Device &getDevice() const;

	 protected:
		ConfigurationCatalogue &config;

	 private:
		Device &device;
	};
}  // namespace CalX

#endif
