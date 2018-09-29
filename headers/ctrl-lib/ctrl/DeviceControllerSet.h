#ifndef CALX_CTRL_LIB_CTRL_DEVICE_CONTROLLER_SET_H_
#define CALX_CTRL_LIB_CTRL_DEVICE_CONTROLLER_SET_H_

#include "ctrl-lib/ctrl/MotorController.h"
#include "ctrl-lib/ctrl/InstrumentController.h"
#include "ctrl-lib/device/DeviceManager.h"
#include <vector>
#include <memory>

namespace CalX {

	template <typename T>
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
		virtual void deviceAdded(std::shared_ptr<DeviceController>) = 0;
	};
	
	class MotorControllerSet : public DeviceControllerSet<MotorController> {
	 public:
		MotorControllerSet(ConfigManager &, DeviceManager &, DeviceControllerSetListener *);
		std::weak_ptr<MotorController> connectDevice(DeviceConnectionPrms *) override;
		std::weak_ptr<MotorController> getDeviceController(device_id_t) const override;
		std::size_t getDeviceCount() const override;
	 private:
		ConfigManager &config;
		DeviceManager &devman;
		DeviceControllerSetListener *listener;
		std::vector<std::shared_ptr<MotorController>> motors;
	};
	
	class InstrumentControllerSet : public DeviceControllerSet<InstrumentController> {
	 public:
		InstrumentControllerSet(ConfigManager &, DeviceManager &, DeviceControllerSetListener *);
		std::weak_ptr<InstrumentController> connectDevice(DeviceConnectionPrms *) override;
		std::weak_ptr<InstrumentController> getDeviceController(device_id_t) const override;
		std::size_t getDeviceCount() const override;
	 private:
		ConfigManager &config;
		DeviceManager &devman;
		DeviceControllerSetListener *listener;
		std::vector<std::shared_ptr<InstrumentController>> instruments;
	};
}

#endif