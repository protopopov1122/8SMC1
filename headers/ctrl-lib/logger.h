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

#ifndef CALX_CTRL_LIB_LOGGER_H_
#define CALX_CTRL_LIB_LOGGER_H_

/* This file contains simple logging system used by project.
   All logger output controller by few macros:
        * WRITE_LOG(output, tag, message) - log some debug info with tag
        * INIT_LOG & DESTROY_LOG - track resource allocation and destroy with
   tag
   "resource"

        To enable logging define LOGGING macro */

#include "platform.h"
#include "ctrl-lib/logger/Global.h"
#include <string>
#include <string.h>

#ifdef LOGGING
#define WRITE_LOG(__output, tag, msg)                                          \
	do {                                                                         \
		const char *__file = (strrchr(__FILE__, PATH_SEPARATOR)                    \
		                          ? strrchr(__FILE__, PATH_SEPARATOR) + 1          \
		                          : __FILE__);                                     \
		int __line = __LINE__;                                                     \
		GlobalLogger::getLogger().stream(__output, tag,                            \
		                                 SourcePosition(__file, __line))           \
		    << msg << Flush();                                                     \
	} while (false)
#else
#define WRITE_LOG(output, tag, msg)
#endif

#define LOG_ERROR(tag, msg) WRITE_LOG(LoggingSeverity::Error, tag, msg)
#define LOG_WARNING(tag, msg) WRITE_LOG(LoggingSeverity::Warning, tag, msg)
#define LOG_DEBUG(tag, msg) WRITE_LOG(LoggingSeverity::Debug, tag, msg)
#define LOG(tag, msg) WRITE_LOG(LoggingSeverity::Info, tag, msg)
#define LOG_INSTR(id, msg)                                                     \
	WRITE_LOG(LoggingSeverity::Info, "Instrument #" + std::to_string(id), msg)

#define INIT_LOG(name)                                                         \
	WRITE_LOG(LoggingSeverity::Debug, "resource",                                \
	          std::string(name) + " initialized")
#define DESTROY_LOG(name)                                                      \
	WRITE_LOG(LoggingSeverity::Debug, "resource",                                \
	          std::string(name) + " destroyed")

#endif
