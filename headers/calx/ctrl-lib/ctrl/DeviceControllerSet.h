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

#ifndef CALX_CTRL_LIB_CTRL_DEVICE_CONTROLLER_SET_H_
#define CALX_CTRL_LIB_CTRL_DEVICE_CONTROLLER_SET_H_

#include "calx/ctrl-lib/ctrl/MotorController.h"
#include "calx/ctrl-lib/ctrl/InstrumentController.h"
#include "calx/ctrl-lib/device/DeviceManager.h"
#include <vector>
#include <memory>

namespace CalX {

	template<typename T>
	class DeviceControllerSet {
	 public:
		virtual ~DeviceControllerSet() = default;
		virtual std::weak_ptr<T> connectDevice(DeviceConnectionPrms *) = 0;
		virtual std::weak_ptr<T> getDeviceController(device_id_t) const = 0;
		virtual std::size_t getDeviceCount() const = 0;
	};

	class DeviceControllerSetListener {
	 public:
		virtual ~DeviceControllerSetListener() = default;
		virtual void onDeviceConnected(std::shared_ptr<DeviceController>) = 0;
	};

	typedef DeviceControllerSet<MotorController> MotorControllerSet;
	typedef DeviceControllerSet<InstrumentController> InstrumentControllerSet;

	class VectorMotorControllerSet : public DeviceControllerSet<MotorController> {
	 public:
		VectorMotorControllerSet(ConfigurationCatalogue &, DeviceManager &,
		                         DeviceControllerSetListener * = nullptr);
		std::weak_ptr<MotorController> connectDevice(
		    DeviceConnectionPrms *) override;
		std::weak_ptr<MotorController> getDeviceController(
		    device_id_t) const override;
		std::size_t getDeviceCount() const override;

	 private:
		ConfigurationCatalogue &config;
		DeviceManager &devman;
		DeviceControllerSetListener *listener;
		std::vector<std::shared_ptr<MotorController>> motors;
	};

	class VectorInstrumentControllerSet
	    : public DeviceControllerSet<InstrumentController> {
	 public:
		VectorInstrumentControllerSet(ConfigurationCatalogue &, DeviceManager &,
		                              DeviceControllerSetListener *);
		std::weak_ptr<InstrumentController> connectDevice(
		    DeviceConnectionPrms *) override;
		std::weak_ptr<InstrumentController> getDeviceController(
		    device_id_t) const override;
		std::size_t getDeviceCount() const override;

	 private:
		ConfigurationCatalogue &config;
		DeviceManager &devman;
		DeviceControllerSetListener *listener;
		std::vector<std::shared_ptr<InstrumentController>> instruments;
	};
}  // namespace CalX

#endif