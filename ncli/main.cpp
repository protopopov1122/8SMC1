#include "CLICommandSystem.h"
#include "CLIProcessor.h"
#include <iostream>
#include "CLIException.h"

using namespace CalX;

class EchoCommand : public NCLICommand {
  public:
    EchoCommand();
    virtual void execute(std::map<std::string, NCLIParameter*>&, std::ostream&, std::istream&);
};

EchoCommand::EchoCommand() : NCLICommand("echo") {
  this->addParameter("text", NCLIParameterType::String);
  this->addParameterAlias('t', "text");
  this->setDefaultValue("text", new NCLIString("Hello, world!"));
}

void EchoCommand::execute(std::map<std::string, NCLIParameter*> &prms, std::ostream &out, std::istream &in) {
  out << prms["text"]->toString() << std::endl;
}

int main(int argc, char **argv) {
  NCLICommandSystem cmdsys;
  NCLIProcessor sh(&cmdsys);
  cmdsys.newCommand(new EchoCommand());
  bool work = true;
  while (work) {
    try {
      work = sh.shell(std::cout, std::cin);
    } catch (NCLIUnknownCommandException &exc) {
      std::cout << "Unknown command \'" << exc.getCommand() << "\'" << std::endl;
    } catch (NCLIUnknownParameterException &exc) {
      std::cout << "Command \'" << exc.getCommand()
        << "\' has no parameter called \'" << exc.getParameter() << "\'" << std::endl;
    } catch (NCLIUnknownParameterAliasException &exc) {
      std::cout << "Command \'" << exc.getCommand()
        << "\' has no alias \'" << exc.getParameterAlias() << '\'' << std::endl;
    } catch (NCLIIncompatibleTypeException &exc) {
      std::cout << "Command \'" << exc.getCommand()
        << "\' parameter \'" << exc.getParameter()
        << "\'  expects " << TYPE_STRINGS.at(exc.getExpectedType())
        << "; got " << TYPE_STRINGS.at(exc.getReceviedType())
        << std::endl;
    } catch (NCLIExcessParameterException &exc) {
      std::cout << "Excess parameters passed to command \'" << exc.getCommand() << "\'" << std::endl;
    } catch (...) {
      std::cout << "Caught exception" << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
