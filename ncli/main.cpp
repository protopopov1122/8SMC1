#include "ncli/CLICommandSystem.h"
#include "ncli/CLIProcessor.h"
#include "ncli/CLIException.h"
#include "ncli/processors/CalXNCLI.h"
#include <iostream>
#include <fstream>

using namespace CalX;

int main(int argc, char **argv) {
  DeviceManager *devman = getDeviceManager();
  ConfigManager *conf = nullptr;
  std::ifstream cnf("main.conf.ini");
  if (!cnf.good()) {
      std::cout << "Can't load configuration, using default values." << std::endl;
      conf = new ConfigManager();
  } else {
      conf = ConfigManager::load(&cnf, &std::cout);
  }
	cnf.close();
	SystemManager *sysman = new SystemManager(devman, conf);

  NCLICommandSystem cmdsys;
  NCLIProcessor sh(&cmdsys);
  cmdsys.newCommand(new MotorSerialConnectorNCLI(sysman));
  cmdsys.newCommand(new InstrumentSerialConnectorNCLI(sysman));
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
