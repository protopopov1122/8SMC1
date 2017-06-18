#include "CLICommand.h"
#include "CLIException.h"

namespace CalX {

  NCLICommand::NCLICommand(std::string name) {
    this->name = name;
  }

  NCLICommand::~NCLICommand() {
    for (const auto &kv : this->default_values) {
      delete kv.second;
    }
  }

  std::string NCLICommand::getName() {
    return this->name;
  }

  bool NCLICommand::addParameter(std::string name, NCLIParameterType type) {
    if (this->prms_map.count(name) != 0) {
      return false;
    }
    this->prms_map[name] = this->parameters.size();
    this->parameters.push_back(std::make_pair(name, type));
    return true;
  }

  bool NCLICommand::hasParameter(std::string name) {
    return this->prms_map.count(name) != 0;
  }

  NCLIParameterType NCLICommand::getParameter(std::string name) {
    if (this->prms_map.count(name) == 0) {
      return NCLIParameterType::None;
    }
    return this->parameters[this->prms_map[name]].second;
  }

  std::string NCLICommand::getParameterName(size_t index) {
    if (index >= this->parameters.size()) {
      return "";
    }
    return this->parameters.at(index).first;
  }

  bool NCLICommand::setDefaultValue(std::string name, NCLIParameter *value) {
    if (this->prms_map.count(name) == 0) {
      return false;
    }
    this->default_values.insert(std::make_pair(name, value));
    return true;
  }

  bool NCLICommand::hasDefaultValue(std::string name) {
    return this->default_values.count(name) != 0;
  }

  NCLIParameter *NCLICommand::getDefaultValue(std::string name) {
    if (this->default_values.count(name) == 0) {
      return nullptr;
    } else {
      return this->default_values.at(name);
    }
  }

  void NCLICommand::getParameters(std::vector<std::string> &vec) {
    for (const auto& kv : this->parameters) {
      vec.push_back(kv.first);
    }
  }

  void NCLICommand::addParameterAlias(char c, std::string str) {
    this->aliases.insert(std::make_pair(c, str));
  }

  bool NCLICommand::hasParameterAlias(char c) {
    return this->aliases.count(c) != 0;
  }

  std::string NCLICommand::getParameterAlias(char c) {
    if (this->aliases.count(c) == 0) {
      return "";
    } else {
      return this->aliases.at(c);
    }
  }

  NCLICommandInstance::NCLICommandInstance(NCLICommand *cmd) {
    this->command = cmd;
    this->unnamed_arg_count = 0;
  }

  NCLICommandInstance::~NCLICommandInstance() {
    for (const auto &kv : this->args) {
      delete kv.second;
    }
  }

  NCLICommand *NCLICommandInstance::getCommand() {
    return this->command;
  }

  bool NCLICommandInstance::addArgument(NCLIParameter *value, std::string name) {
    if (name.empty()) {
      name = this->command->getParameterName(this->unnamed_arg_count);
      this->unnamed_arg_count++;
      if (name.empty()) {
        return false;
      }
    }
    if (!NCLITypesCompatible(value->getType(), this->command->getParameter(name))) {
      return false;
    }
    this->args.insert(std::make_pair(name, value));
    return true;
  }

  void NCLICommandInstance::appendArguments(std::vector<std::pair<std::string, NCLIParameter*>> &args) {
    size_t i = 0;
    for (const auto& arg : args) {
      std::string name = arg.first;
      NCLIParameter *value = arg.second;
      if (name.empty()) {
        name = this->command->getParameterName(this->unnamed_arg_count);
        if (name.empty()) {
          for (; i < args.size(); i++) {
            delete args.at(i).second;
          }
          NCLIExcessParameterException(this->command->getName()).raise();
        }
        this->unnamed_arg_count++;
      }
      if (this->command->hasParameter(name)) {
        if (!NCLITypesCompatible(value->getType(), this->command->getParameter(name))) {
            NCLIParameterType recv = value->getType();
            for (; i < args.size(); i++) {
              delete args.at(i).second;
            }
            NCLIIncompatibleTypeException(this->command->getName(), name,
              recv, this->command->getParameter(name)).raise();
        } else {
          this->args.insert(std::make_pair(name, value));
        }
      } else {
        for (; i < args.size(); i++) {
          delete args.at(i).second;
        }
        NCLIUnknownParameterException(this->command->getName(), name).raise();
      }
      i++;
    }
  }

  void NCLICommandInstance::askArguments(std::ostream& out, std::istream& in) {
    std::vector<std::string> full_args;
    this->command->getParameters(full_args);
    for (const auto& prname : full_args) {
      if (this->args.count(prname) == 0) {
        this->askArgument(prname, out, in);
      }
    }
  }

  void NCLICommandInstance::askArgument(std::string name, std::ostream &out, std::istream &in) {
    NCLIParameter *prm = nullptr;
    while (prm == nullptr) {
      out << "Enter '" << name << "' parameter";
      out << '[' << TYPE_STRINGS.at(this->command->getParameter(name)) << ']';
      if (this->command->hasDefaultValue(name)) {
        out << '[' << this->command->getDefaultValue(name)->toString() << ']';
      }
      out << ": ";
      std::string line;
      getline(in, line);
      if (line.empty() && this->command->hasDefaultValue(name)) {
        prm = this->command->getDefaultValue(name)->clone();
        this->addArgument(prm, name);
        return;
      }
      if (!line.empty()) {
        prm = parseNCLIParameter(line);
        if (prm != nullptr && !NCLITypesCompatible(prm->getType(), this->command->getParameter(name))) {
          out << "Command \'" << this->command->getName()
            << "\' parameter \'" << name
            << "\'  expects " << TYPE_STRINGS.at(this->command->getParameter(name))
            << "; got " << TYPE_STRINGS.at(prm->getType())
            << std::endl;
          delete prm;
          prm = nullptr;
        }
      }
    }
    this->addArgument(prm, name);
  }

  void NCLICommandInstance::execute(std::ostream &out, std::istream &in) {
    this->command->execute(this->args, out, in);
  }
}
