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

#include <string.h>
#include <string>
#include "device/standart/NL300Instrument.h"
#include "device/standart/StandartDeviceManager.h"

namespace CalX {

  NL300Message::NL300Message(NL300MessageType type, std::string recv,
							 std::string msg, std::string snd) {
	this->type = type;
	this->receiver = recv;
	this->message = msg;
	this->sender = snd;
  }

  NL300Message::~NL300Message() {}

  NL300MessageType NL300Message::getType() {
	return this->type;
  }

  std::string NL300Message::getReceiver() {
	return this->receiver;
  }

  std::string NL300Message::getMessage() {
	return this->message;
  }

  std::string NL300Message::getSender() {
	return this->sender;
  }

  std::string NL300Message::toCommand() {
	return "[" + this->receiver + ":" + this->message + "\\" + this->sender +
		   "]";
  }

  NL300Parameter::NL300Parameter(NL300ParameterType t) {
	this->type = t;
  }

  NL300Parameter::~NL300Parameter() {}

  NL300ParameterType NL300Parameter::getType() {
	return this->type;
  }

  int64_t NL300Parameter::getInt(int64_t def) {
	if (this->type == NL300ParameterType::Integer) {
	  return ((NL300IntegerParameter*) this)->getValue();
	} else {
	  return def;
	}
  }

  double NL300Parameter::getReal(double def) {
	if (this->type == NL300ParameterType::Real) {
	  return ((NL300RealParameter*) this)->getValue();
	} else {
	  return def;
	}
  }

  std::string NL300Parameter::getString(std::string def) {
	if (this->type == NL300ParameterType::String) {
	  return ((NL300StringParameter*) this)->getValue();
	} else {
	  return def;
	}
  }

  NL300IntegerParameter::NL300IntegerParameter(int64_t v)
	  : NL300Parameter::NL300Parameter(NL300ParameterType::Integer) {
	this->value = v;
  }

  NL300IntegerParameter::~NL300IntegerParameter() {}

  int64_t NL300IntegerParameter::getValue() {
	return this->value;
  }

  std::string NL300IntegerParameter::getString() {
	return std::to_string(this->value);
  }

  NL300RealParameter::NL300RealParameter(double v)
	  : NL300Parameter::NL300Parameter(NL300ParameterType::Real) {
	this->value = v;
  }

  NL300RealParameter::~NL300RealParameter() {}

  double NL300RealParameter::getValue() {
	return this->value;
  }

  std::string NL300RealParameter::getString() {
	return std::to_string(this->value);
  }

  NL300StringParameter::NL300StringParameter(std::string v)
	  : NL300Parameter::NL300Parameter(NL300ParameterType::String) {
	this->value = v;
  }

  NL300StringParameter::~NL300StringParameter() {}

  std::string NL300StringParameter::getValue() {
	return this->value;
  }

  std::string NL300StringParameter::getString() {
	return "=" + this->value;
  }

  NL300NoneParameter::NL300NoneParameter()
	  : NL300Parameter::NL300Parameter(NL300ParameterType::None) {}

  NL300NoneParameter::~NL300NoneParameter() {}

  std::string NL300NoneParameter::getString() {
	return "";
  }
}
