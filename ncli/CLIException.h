#ifndef CALX_NCLI_CLI_EXCEPTION_H_
#define CALX_NCLI_CLI_EXCEPTION_H_


#include "CLICore.h"
#include "CLIParameter.h"
#include <exception>
#include <string>

namespace CalX {

  enum class NCLIError {
    UNKNOWN_COMMAND,
    WRONG_PARAMETER_TYPE,
    UNKNOWN_PARAMETER,
    UNKNOWN_PARAMETER_ALIAS,
    LONG_PARAMETER_LIST
  };

  class NCLIException : public std::exception {
    public:
      NCLIException(NCLIError);
      virtual ~NCLIException();
      NCLIError getError();
      virtual void raise();
    private:
      NCLIError err;
  };

  class NCLIUnknownCommandException : public NCLIException {
    public:
      NCLIUnknownCommandException(std::string);
      std::string getCommand();
      virtual void raise();
    private:
      std::string command;
  };

  class NCLIUnknownParameterException : public NCLIException {
    public:
      NCLIUnknownParameterException(std::string, std::string);
      std::string getCommand();
      std::string getParameter();
      virtual void raise();
    private:
      std::string command;
      std::string parameter;
  };

  class NCLIUnknownParameterAliasException : public NCLIException {
    public:
      NCLIUnknownParameterAliasException(std::string, char);
      std::string getCommand();
      char getParameterAlias();
      virtual void raise();
    private:
      std::string command;
      char alias;
  };

  class NCLIIncompatibleTypeException : public NCLIException {
    public:
      NCLIIncompatibleTypeException(std::string, std::string, NCLIParameterType, NCLIParameterType);
      std::string getCommand();
      std::string getParameter();
      NCLIParameterType getReceviedType();
      NCLIParameterType getExpectedType();
      virtual void raise();
    private:
      std::string command;
      std::string parameter;
      NCLIParameterType received;
      NCLIParameterType expected;
  };

  class NCLIExcessParameterException : public NCLIException {
    public:
      NCLIExcessParameterException(std::string);
      std::string getCommand();
      virtual void raise();
    private:
      std::string command;
  };
}

#endif
