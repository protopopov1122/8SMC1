#ifndef CALX_NCLI_CLI_COMMAND_SYSTEM_H_
#define CALX_NCLI_CLI_COMMAND_SYSTEM_H_

#include "CLICommand.h"
#include <vector>
#include <map>

namespace CalX {

  class NCLICommandSystem {
    public:
      NCLICommandSystem();
      virtual ~NCLICommandSystem();

      bool newCommand(NCLICommand*);
      NCLICommandInstance *newCommandInstance(std::string);
      NCLICommand *getCommand(std::string);
    private:
      std::map<std::string, NCLICommand*> commands;
  };
}

#endif
