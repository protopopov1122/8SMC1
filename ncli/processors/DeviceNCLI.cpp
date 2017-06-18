#include "ncli/processors/DeviceNCLI.h"

namespace CalX {

  MotorSerialConnectorNCLI::MotorSerialConnectorNCLI(SystemManager *sysman)
    : NCLICommand("motor.connect.serial") {

    this->sysman = sysman;
    this->addParameter("port", NCLIParameterType::Integer);
    this->addParameter("speed", NCLIParameterType::Integer);
    this->addParameter("parity", NCLIParameterType::Integer);
    this->setDefaultValue("parity", new NCLIInteger(0));
  }

  void MotorSerialConnectorNCLI::execute(std::map<std::string, NCLIParameter*> &args,
    std::ostream &out, std::istream &in) {

      DeviceSerialPortConnectionPrms prms;
      prms.port = args["port"]->as_int();
      prms.speed = args["speed"]->as_int();
      int64_t par = args["parity"]->as_int();
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
      prms.parity = parity;
      MotorController *ctrl = sysman->connectMotor(&prms);
      if (ctrl == nullptr) {
        out << "Motor connection failed" << std::endl;
      } else {
        out << "Motor connected" << std::endl;
      }
  }

  InstrumentSerialConnectorNCLI::InstrumentSerialConnectorNCLI(SystemManager *sysman)
    : NCLICommand("instrument.connect.serial") {

    this->sysman = sysman;
    this->addParameter("port", NCLIParameterType::Integer);
    this->addParameter("speed", NCLIParameterType::Integer);
    this->addParameter("parity", NCLIParameterType::Integer);
    this->setDefaultValue("parity", new NCLIInteger(0));
  }

  void InstrumentSerialConnectorNCLI::execute(std::map<std::string, NCLIParameter*> &args,
    std::ostream &out, std::istream &in) {

      DeviceSerialPortConnectionPrms prms;
      prms.port = args["port"]->as_int();
      prms.speed = args["speed"]->as_int();
      int64_t par = args["parity"]->as_int();
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
      prms.parity = parity;
      InstrumentController *ctrl = sysman->connectInstrument(&prms);
      if (ctrl == nullptr) {
        out << "Instrument connection failed" << std::endl;
      } else {
        out << "Instrument connected" << std::endl;
      }
  }
}
