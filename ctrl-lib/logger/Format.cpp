#include "ctrl-lib/logger/Format.h"
#include <chrono>
#include <iostream>

namespace CalX {

	std::function<void(const LogEntry &, std::ostream &)>
	    JournalFormatter::defaultFormat(const std::string &name) {
		return [name](const LogEntry &entry, std::ostream &out) {
			constexpr unsigned int SinkNamePadding = 10;
			std::string fullName(name);
			if (fullName.size() < SinkNamePadding) {
				fullName.insert(fullName.size(), SinkNamePadding - fullName.size(),
				                ' ');
			}
			auto timenow = std::chrono::system_clock::to_time_t(
			    std::chrono::system_clock::now());
			std::string currentTime(ctime(&timenow));
			out << fullName << currentTime.substr(0, currentTime.size() - 1) << '\t';

			std::string fullTag;
			if (entry.tag.size() > 0) {
				fullTag.append(" [");
				fullTag.append(entry.tag);
				fullTag.push_back(']');
			}
			if (!entry.position.isEmpty()) {
				fullTag.append("@(");
				fullTag.append(entry.position.file);
				fullTag.push_back(':');
				fullTag.append(std::to_string(entry.position.line));
				fullTag.push_back(')');
			}

			if (fullTag.size() > 0) {
				if (fullTag.size() < 50) {
					fullTag.insert(fullTag.size(), 50 - fullTag.size(), ' ');
				}
				out << fullTag << '\t' << entry.message << std::endl;
			} else {
				out << entry.message << std::endl;
			}
		};
	}
}  // namespace CalX