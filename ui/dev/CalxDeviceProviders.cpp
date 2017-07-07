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

#include "ui/dev/CalxDeviceProviders.h"

namespace CalXUI {

	
  class CalxInstrumentSerialConnectProvider : public RequestProvider {
   public:
	CalxInstrumentSerialConnectProvider(CalxDevicePanel *devpanel)
		: RequestProvider::RequestProvider("connect.serial.instrument") {
	  this->devpanel = devpanel;
	}

	virtual ~CalxInstrumentSerialConnectProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 3)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
	  uint8_t port =
		  (uint8_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))->getValue();
	  uint32_t speed =
		  (uint16_t)((IntegerConfigValue *) PROVIDER_ARG(req, 1))->getValue();
	  int_conf_t par =
		  ((IntegerConfigValue *) PROVIDER_ARG(req, 2))->getValue();
	  SerialPortParity parity = SerialPortParity::No;
	  switch (par) {
		case 0:
		  parity = SerialPortParity::No;
		  break;
		case 1:
		  parity = SerialPortParity::Odd;
		  break;
		case 2:
		  parity = SerialPortParity::Even;
		  break;
		case 3:
		  parity = SerialPortParity::Mark;
		  break;
		case 4:
		  parity = SerialPortParity::Space;
		  break;
	  }
	  DeviceSerialPortConnectionPrms prms;
	  prms.port = port;
	  prms.speed = speed;
	  prms.parity = parity;
	  InstrumentController *ctrl = sysman->connectInstrument(&prms);
	  if (ctrl == nullptr) {
		return false;
	  }
	  wxThreadEvent evt(wxEVT_DEVICE_PANEL_UPDATE);
	  wxPostEvent(devpanel, evt);
	  return true;
	}

   private:
	CalxDevicePanel *devpanel;
  };

  class CalxMotorSerialConnectProvider : public RequestProvider {
   public:
	CalxMotorSerialConnectProvider(CalxDevicePanel *devpanel)
		: RequestProvider::RequestProvider("connect.serial.motor") {
	  this->devpanel = devpanel;
	}

	virtual ~CalxMotorSerialConnectProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 3)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
	  uint8_t port =
		  (uint8_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))->getValue();
	  uint32_t speed =
		  (uint16_t)((IntegerConfigValue *) PROVIDER_ARG(req, 1))->getValue();
	  int_conf_t par =
		  ((IntegerConfigValue *) PROVIDER_ARG(req, 2))->getValue();
	  SerialPortParity parity = SerialPortParity::No;
	  switch (par) {
		case 0:
		  parity = SerialPortParity::No;
		  break;
		case 1:
		  parity = SerialPortParity::Odd;
		  break;
		case 2:
		  parity = SerialPortParity::Even;
		  break;
		case 3:
		  parity = SerialPortParity::Mark;
		  break;
		case 4:
		  parity = SerialPortParity::Space;
		  break;
	  }
	  DeviceSerialPortConnectionPrms prms;
	  prms.port = port;
	  prms.speed = speed;
	  prms.parity = parity;
	  MotorController *ctrl = sysman->connectMotor(&prms);
	  if (ctrl == nullptr) {
		return false;
	  }
	  wxThreadEvent evt(wxEVT_DEVICE_PANEL_UPDATE);
	  wxPostEvent(devpanel, evt);
	  return true;
	}

   private:
	CalxDevicePanel *devpanel;
  };

  class CalxMotorPowerProvider : public RequestProvider {
   public:
	CalxMotorPowerProvider(CalxDevicePanel *devpanel)
		: RequestProvider::RequestProvider("motor.power") {
	  this->devpanel = devpanel;
	}

	virtual ~CalxMotorPowerProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 2)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Boolean)
	  device_id_t devid =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))
			  ->getValue();
	  bool power = ((BoolConfigValue *) PROVIDER_ARG(req, 1))->getValue();
	  MotorController *ctrl = sysman->getMotorController(devid);
	  if (ctrl != nullptr) {
		return ctrl->getMotor()->enablePower(power);
	  } else {
		return false;
	  }
	}

   private:
	CalxDevicePanel *devpanel;
  };
  
  void calx_init_device_providers(CalxDevicePanel *panel) {
	CalxApp &app = wxGetApp();
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxMotorSerialConnectProvider(panel));
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxInstrumentSerialConnectProvider(panel));
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxMotorPowerProvider(panel));
  }
}