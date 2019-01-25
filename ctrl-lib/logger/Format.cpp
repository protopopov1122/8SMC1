/*
        SPDX-License-Identifier: LGPL-3.0-or-later

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published
   by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ctrl-lib/logger/Format.h"
#include <chrono>
#include <iostream>

namespace CalX {

	std::function<void(const LogEntry &, std::ostream &)>
	    JournalFormatter::defaultFormat(const std::string &name) {
		return [name](const LogEntry &entry, std::ostream &out) {
			constexpr unsigned int SinkNamePadding = 10;
			std::string fullName(name);
			if (!name.empty()) {
				if (fullName.size() < SinkNamePadding) {
					fullName.insert(fullName.size(), SinkNamePadding - fullName.size(),
					                ' ');
				}
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