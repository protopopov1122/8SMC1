#ifndef CALX_NCLI_PROCESSORS_DEVICE_PROCESSOR_H_
#define CALX_NCLI_PROCESSORS_DEVICE_PROCESSOR_H_


#include "ncli/CLICommand.h"
#include "ctrl-lib/SystemManager.h"

namespace CalX {

  class MotorSerialConnectorNCLI : public NCLICommand {
    public:
      MotorSerialConnectorNCLI(SystemManager*);
      virtual void execute(std::map<std::string, NCLIParameter*>&, std::ostream&, std::istream&);
    private:
      SystemManager *sysman;
  };


  class InstrumentSerialConnectorNCLI : public NCLICommand {
    public:
      InstrumentSerialConnectorNCLI(SystemManager*);
      virtual void execute(std::map<std::string, NCLIParameter*>&, std::ostream&, std::istream&);
    private:
      SystemManager *sysman;
  };
}

#endif
