/*
        Copyright (c) 2017-2018 Jevgenijs Protopopovs

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

#ifndef CALX_CTRL_LIB_LOGGER_FILTER_H_
#define CALX_CTRL_LIB_LOGGER_FILTER_H_

#include "ctrl-lib/logger/Journal.h"
#include <functional>

namespace CalX {

	class LoggerFilter {
	 public:
		static std::function<bool(const LogEntry &entry)> severity_any() {
			return [](const LogEntry &entry) { return true; };
		}

		static std::function<bool(const LogEntry &entry)> severity_at_least(
		    LoggingSeverity severity) {
			return [severity](const LogEntry &entry) {
				return static_cast<int>(entry.severity) >= static_cast<int>(severity);
			};
		}

		static std::function<bool(const LogEntry &entry)> severity_exact(
		    LoggingSeverity severity) {
			return [severity](const LogEntry &entry) {
				return entry.severity == severity;
			};
		}
	};
}  // namespace CalX

#endif