#ifndef _8SMC1_CLI_H_
#define _8SMC1_CLI_H_

#include <string>
#include <cinttypes>
#include <vector>
#include <map>

namespace _8SMC1 {
		
	class CLICommand {
		public:
			virtual ~CLICommand() {};
			virtual void execute(std::vector<std::string>&) = 0;
			int requireArgument(std::vector<std::string>&, size_t, std::string,
						int);
			float requireArgument(std::vector<std::string>&, size_t, std::string,
						float);
	};
	
	class CLI {
		public:
			CLI();
			virtual ~CLI();
			bool shell();
			void error(std::string);
			void addCommand(std::string, CLICommand*);
		private:
			std::map<std::string, CLICommand*> commands;
	};
}

#endif
