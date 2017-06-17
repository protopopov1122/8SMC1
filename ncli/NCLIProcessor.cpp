#include "CLIProcessor.h"
#include <string>

namespace CalX {

  NCLIProcessor::NCLIProcessor(NCLICommandSystem *cmdsys) {
    this->cmdsys = cmdsys;
    this->prompt = ">>> ";
  }

  NCLIProcessor::~NCLIProcessor() {}

  std::string NCLIProcessor::getPrompt() {
    return this->prompt;
  }

  void NCLIProcessor::setPrompt(std::string pr) {
    this->prompt = pr;
  }

  bool NCLIProcessor::shell(std::ostream &out, std::istream &in) {
    std::string line;
    out << this->prompt;
    std::getline(in, line);
    if (line.empty()) {
      return false;
    }
    std::vector<std::pair<std::string, NCLIParameter*>> args;
    std::string cmd = this->parseLine(line, args);
    NCLICommandInstance *command = this->cmdsys->newCommandInstance(cmd);
    if (command == nullptr) {
      out << "Unknown command: " << cmd << std::endl;
    } else {
      for (const auto& arg : args) {
        command->addArgument(arg.second, arg.first);
      }
      command->askArguments(out, in);
      command->execute(out, in);
      delete command;
    }
    return true;
  }

  std::string NCLIProcessor::parseLine(std::string line,
    std::vector<std::pair<std::string, NCLIParameter*>> &args) {

    return line;
  }
}
