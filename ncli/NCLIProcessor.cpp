#include "ncli/CLIProcessor.h"
#include "ncli/CLIException.h"
#include <string>
#include <string.h>

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
    try {
      if (command == nullptr) {
        NCLIUnknownCommandException(cmd).raise();
      } else {
        command->appendArguments(args);
        command->askArguments(out, in);
        command->execute(out, in);
        delete command;
      }
    } catch(NCLIException &exc) {
      delete command;
      exc.raise();
    }
    return true;
  }

  std::string NCLIProcessor::parseLine(std::string line,
    std::vector<std::pair<std::string, NCLIParameter*>> &args) {

    std::string command;
    std::vector<std::string> raw_args;
    bool quote = false;
    size_t offset;
    for (offset = 0; offset < line.size() && !isspace(line.at(offset)); offset++) {
      command.push_back(line.at(offset));
    }
    NCLICommand *cmd = this->cmdsys->getCommand(command);
    if (cmd == nullptr) {
      return command;
    }

    for (; offset < line.size(); offset++) {
      char chr = line.at(offset);
      if (chr == '\'') {
        quote = !quote;
      }
      if (isspace(chr) && !quote) {
        if (raw_args.size() > 0 && !raw_args.at(raw_args.size() - 1).empty()) {
          raw_args.push_back("");
        }
      } else {
        if (raw_args.empty()) {
          raw_args.push_back("");
        }
        raw_args.at(raw_args.size() - 1).push_back(chr);
      }
    }
    for (const auto& str : raw_args) {
      if (str.size() > 2 && str.at(0) == '-' && str.at(1) == '-') {
        std::string name;
        std::string value;
        NCLIParameter *prm;
        for (offset = 2; offset < str.size() && str.at(offset) != '='; offset++) {
          name.push_back(str.at(offset));
        }
        bool prm_ex = offset < str.size() && str.at(offset) == '=';
        if (prm_ex) {
          offset++;
          for (;offset < str.size(); offset++) {
            value.push_back(str.at(offset));
          }
          prm = parseNCLIParameter(value);
        } else {
          prm = new NCLIBoolean(true);
        }
        args.push_back(std::make_pair(name, prm));
      } else if (str.size() > 1 && str.at(0) == '-') {
        char shprm = str.at(1);
        if (cmd->hasParameterAlias(shprm)) {
          std::string name = cmd->getParameterAlias(shprm);
          std::string value;
          NCLIParameter *prm;
          offset = 2;
          bool prm_ex = offset < str.size() && str.at(offset) == '=';
          if (prm_ex) {
            offset++;
            for (;offset < str.size(); offset++) {
              value.push_back(str.at(offset));
            }
            prm = parseNCLIParameter(value);
          } else {
            prm = new NCLIBoolean(true);
          }
          args.push_back(std::make_pair(name, prm));
        } else {
          NCLIUnknownParameterAliasException(command, shprm).raise();
        }
      } else {
        NCLIParameter *prm = parseNCLIParameter(str);
        args.push_back(std::make_pair("", prm));
      }
    }
    return command;
  }
}
