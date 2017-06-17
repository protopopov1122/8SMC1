#ifndef CALX_NCLI_CLI_PROCESSOR_H_
#define CALX_NCLI_CLI_PROCESSOR_H_

#include "CLICommandSystem.h"
#include <iostream>

namespace CalX {

  class NCLIProcessor {
    public:
      NCLIProcessor(NCLICommandSystem*);
      virtual ~NCLIProcessor();

      std::string getPrompt();
      void setPrompt(std::string);
      bool shell(std::ostream&, std::istream&);
    private:
      std::string parseLine(std::string, std::vector<std::pair<std::string, NCLIParameter*>>&);
      NCLICommandSystem *cmdsys;
      std::string prompt;
  };
}

#endif
