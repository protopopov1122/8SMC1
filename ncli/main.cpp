#include "CLICommandSystem.h"
#include "CLIProcessor.h"
#include <iostream>

using namespace CalX;

class EchoCommand : public NCLICommand {
  public:
    EchoCommand();
    virtual void execute(std::map<std::string, NCLIParameter*>&, std::ostream&, std::istream&);
};

EchoCommand::EchoCommand() : NCLICommand("echo") {
  this->addParameter("text", NCLIParameterType::String);
  this->setDefaultValue("text", new NCLIString("Hello, world!"));
}

void EchoCommand::execute(std::map<std::string, NCLIParameter*> &prms, std::ostream &out, std::istream &in) {
  out << prms["text"]->toString() << std::endl;
}

int main(int argc, char **argv) {
  NCLICommandSystem cmdsys;
  NCLIProcessor sh(&cmdsys);
  cmdsys.newCommand(new EchoCommand());
  while (sh.shell(std::cout, std::cin));
  return EXIT_SUCCESS;
}
