#include "CLICommandSystem.h"

namespace CalX {

  NCLICommandSystem::NCLICommandSystem() {}

  NCLICommandSystem::~NCLICommandSystem() {
    for (const auto& kv : this->commands) {
      delete kv.second;
    }
  }

  bool NCLICommandSystem::newCommand(NCLICommand *cmd) {
    if (this->commands.count(cmd->getName()) != 0) {
      return false;
    }
    this->commands.insert(std::make_pair(cmd->getName(), cmd));
  }

  NCLICommandInstance *NCLICommandSystem::newCommandInstance(std::string name) {
    if (this->commands.count(name) == 0) {
      return nullptr;
    } else {
      return new NCLICommandInstance(this->commands.at(name));
    }
  }

  NCLICommand *NCLICommandSystem::getCommand(std::string name) {
    if (this->commands.count(name) == 0) {
      return nullptr;
    } else {
      return this->commands.at(name);
    }
  }
}
