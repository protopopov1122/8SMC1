#ifndef CALX_NCLI_CLI_COMMAND_H_
#define CALX_NCLI_CLI_COMMAND_H_

#include "CLICore.h"
#include "CLIParameter.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace CalX {

  class NCLICommand {
    public:
      NCLICommand(std::string);
      virtual ~NCLICommand();

      std::string getName();
      bool addParameter(std::string, NCLIParameterType);
      bool hasParameter(std::string);
      NCLIParameterType getParameter(std::string);
      std::string getParameterName(size_t);
      bool setDefaultValue(std::string, NCLIParameter*);
      bool hasDefaultValue(std::string);
      NCLIParameter *getDefaultValue(std::string);
      void getParameters(std::vector<std::string>&);
      virtual void execute(std::map<std::string, NCLIParameter*>&, std::ostream&, std::istream&) = 0;
    private:
      std::string name;
      std::vector<std::pair<std::string, NCLIParameterType>> parameters;
      std::map<std::string, size_t> prms_map;
      std::map<std::string, NCLIParameter*> default_values;
  };

  class NCLICommandInstance {
    public:
      NCLICommandInstance(NCLICommand*);
      virtual ~NCLICommandInstance();

      NCLICommand *getCommand();
      bool addArgument(NCLIParameter*, std::string = "");
      void askArguments(std::ostream&, std::istream&);
      void execute(std::ostream&, std::istream&);
    private:
      void askArgument(std::string, std::ostream&, std::istream&);

      NCLICommand *command;
      size_t unnamed_arg_count;
      std::map<std::string, NCLIParameter*> args;
  };
}


#endif
