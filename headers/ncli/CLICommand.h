#ifndef CALX_NCLI_CLI_COMMAND_H_
#define CALX_NCLI_CLI_COMMAND_H_

#include "ncli/CLICore.h"
#include "ncli/CLIParameter.h"
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
      void addParameterAlias(char, std::string);
      bool hasParameterAlias(char);
      std::string getParameterAlias(char);
      virtual void execute(std::map<std::string, NCLIParameter*>&, std::ostream&, std::istream&) = 0;
    private:
      std::string name;
      std::vector<std::pair<std::string, NCLIParameterType>> parameters;
      std::map<std::string, size_t> prms_map;
      std::map<std::string, NCLIParameter*> default_values;
      std::map<char, std::string> aliases;
  };

  class NCLICommandInstance {
    public:
      NCLICommandInstance(NCLICommand*);
      virtual ~NCLICommandInstance();

      NCLICommand *getCommand();
      bool addArgument(NCLIParameter*, std::string = "");
      void appendArguments(std::vector<std::pair<std::string, NCLIParameter*>>&);
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
