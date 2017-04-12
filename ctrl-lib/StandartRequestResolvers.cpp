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


#include "RequestResolver.h"
#include "SystemManager.h"

/* This file contains some standart request provides */

namespace CalX {
	#define PROVIDER(clname, name)\
		class clname : public RequestProvider {\
			public:\
				clname() : RequestProvider::RequestProvider(name) {}\
				~clname() {}\
				virtual bool execute(Request*, SystemManager*);\
		};\
		bool clname::execute(Request *req, SystemManager *sysman)
		
	#define ARGC(count)\
		if (req->getArguments().size() != count) {\
			return false;\
		}
	#define ARG_TYPE(index, type)\
		if (index >= req->getArguments().size() ||\
			req->getArguments().at(index)->getType() != type) {\
			return false;\
		}
	#define ARG(index) (req->getArguments().at(index))
	
	PROVIDER(EchoProvider, "echo") {
		std::vector<ConfigValue*> &args = req->getArguments();
		for (const auto& arg : args) {
			std::cout << arg->toString() << " ";
		}
		std::cout << std::endl;
		return true;
	}
	
	PROVIDER(NullProvider, "") {
		return true;
	}
	
	PROVIDER(ConnectInstrumentSerialProvider, "connect.serial.instrument") {
		ARGC(3)
		ARG_TYPE(0, ConfigValueType::Integer)
		ARG_TYPE(1, ConfigValueType::Integer)
		ARG_TYPE(2, ConfigValueType::Integer)
		uint8_t port = (uint8_t) ((IntegerConfigValue*) ARG(0))->getValue();
		uint32_t speed = (uint16_t) ((IntegerConfigValue*) ARG(1))->getValue();
		int_conf_t par = ((IntegerConfigValue*) ARG(2))->getValue();
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
		return sysman->connectInstrument(&prms) != nullptr;
	}
	
	PROVIDER(ConnectMotorSerialProvider, "connect.serial.motor") {
		ARGC(3)
		ARG_TYPE(0, ConfigValueType::Integer)
		ARG_TYPE(1, ConfigValueType::Integer)
		ARG_TYPE(2, ConfigValueType::Integer)
		uint8_t port = (uint8_t) ((IntegerConfigValue*) ARG(0))->getValue();
		uint32_t speed = (uint16_t) ((IntegerConfigValue*) ARG(1))->getValue();
		int_conf_t par = ((IntegerConfigValue*) ARG(2))->getValue();
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
		return sysman->connectMotor(&prms) != nullptr;
	}
	
	PROVIDER(PlaneCreateProvider, "plane.create") {
		ARGC(3)
		ARG_TYPE(0, ConfigValueType::Integer)
		ARG_TYPE(1, ConfigValueType::Integer)
		ARG_TYPE(2, ConfigValueType::Integer)
		device_id_t dev1 = (device_id_t) ((IntegerConfigValue*) ARG(0))->getValue();
		device_id_t dev2 = (device_id_t) ((IntegerConfigValue*) ARG(1))->getValue();
		device_id_t instr = (device_id_t) ((IntegerConfigValue*) ARG(2))->getValue();
		return sysman->createCoord(dev1, dev2, instr) != nullptr;
	}
	
	PROVIDER(PlaneMeasureProvider, "plane.measure") {
		ARGC(2)
		ARG_TYPE(0, ConfigValueType::Integer)
		ARG_TYPE(1, ConfigValueType::Integer)
		device_id_t plid = (device_id_t) ((IntegerConfigValue*) ARG(0))->getValue();
		device_id_t tr = (device_id_t) ((IntegerConfigValue*) ARG(1))->getValue() % 2;
		CoordHandle *handle = sysman->getCoord(plid);
		TrailerId trailer = tr == 1 ? TrailerId::Trailer1 : TrailerId::Trailer2;
		if (handle != nullptr) {
			return handle->measure(trailer) != ErrorCode::NoError;
		} else {
			return false;
		}
	}

	void RequestResolver_init_standart_providers(RequestResolver *res) {
		#define REG(clname) res->registerProvider(new clname())
		REG(EchoProvider);
		REG(NullProvider);
		REG(ConnectInstrumentSerialProvider);
		REG(ConnectMotorSerialProvider);
		REG(PlaneCreateProvider);
		REG(PlaneMeasureProvider);
	}
}