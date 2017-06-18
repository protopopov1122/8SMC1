#include "ncli/CLIException.h"

namespace CalX {

  NCLIException::NCLIException(NCLIError err) {
    this->err = err;
  }

  NCLIException::~NCLIException() {}

  NCLIError NCLIException::getError() {
    return this->err;
  }

  void NCLIException::raise() {
    throw *this;
  }

  NCLIUnknownCommandException::NCLIUnknownCommandException(std::string com)
    : NCLIException::NCLIException(NCLIError::UNKNOWN_COMMAND), command(com) {}

  std::string NCLIUnknownCommandException::getCommand() {
    return this->command;
  }

  void NCLIUnknownCommandException::raise() {
    throw *this;
  }

  NCLIUnknownParameterException::NCLIUnknownParameterException(std::string com, std::string par)
    : NCLIException::NCLIException(NCLIError::UNKNOWN_PARAMETER), command(com), parameter(par) {}

  std::string NCLIUnknownParameterException::getCommand() {
    return this->command;
  }

  std::string NCLIUnknownParameterException::getParameter() {
    return this->parameter;
  }

  void NCLIUnknownParameterException::raise() {
    throw *this;
  }

  NCLIUnknownParameterAliasException::NCLIUnknownParameterAliasException(std::string com, char al)
    : NCLIException::NCLIException(NCLIError::UNKNOWN_PARAMETER_ALIAS), command(com), alias(al) {}

  std::string NCLIUnknownParameterAliasException::getCommand() {
    return this->command;
  }

  char NCLIUnknownParameterAliasException::getParameterAlias() {
    return this->alias;
  }

  void NCLIUnknownParameterAliasException::raise() {
    throw *this;
  }

  NCLIIncompatibleTypeException::NCLIIncompatibleTypeException(std::string com, std::string par,
    NCLIParameterType recv, NCLIParameterType exp)
    : NCLIException::NCLIException(NCLIError::WRONG_PARAMETER_TYPE), command(com), parameter(par),
      received(recv), expected(exp) {}

  std::string NCLIIncompatibleTypeException::getCommand() {
    return this->command;
  }

  std::string NCLIIncompatibleTypeException::getParameter() {
    return this->parameter;
  }

  NCLIParameterType NCLIIncompatibleTypeException::getReceviedType() {
    return this->received;
  }

  NCLIParameterType NCLIIncompatibleTypeException::getExpectedType() {
    return this->expected;
  }

  void NCLIIncompatibleTypeException::raise() {
    throw *this;
  }

  NCLIExcessParameterException::NCLIExcessParameterException(std::string com)
    : NCLIException::NCLIException(NCLIError::LONG_PARAMETER_LIST), command(com) {}

  std::string NCLIExcessParameterException::getCommand() {
    return this->command;
  }

  void NCLIExcessParameterException::raise() {
    throw *this;
  }
}
