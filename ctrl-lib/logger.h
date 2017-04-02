/*
    Copyright (c) 2017 Jevgenijs Protopopovs

    This file is part of CalX project.

    CalX is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
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
	* SET_LOGGER(ptr) - set log output
	* DEBUG_LOG(tag, message) - log some debug info with tag
	* INIT_LOG & DESTROY_LOG - track resource allocation and destroy with tag "resource"
	
	To enable debug logging define DEBUGGING_LOG and to track resources define TRACK_RESOURCES */

#include <iostream>

extern std::ostream *RESOURCES;

#define SET_LOGGER(name, val) name = val;

#ifdef DEBUGGING_LOG
#define TRACK_RESOURCES
#define DEBUG_LOG(output, tag, msg) do {\
                                if ((output) != nullptr) {\
                                    *(output) << __FILE__ << ':' << __LINE__\
                                                   << '(' << __DATE__ << ' ' << __TIME__ << ')'\
								                   << ' ' << (tag) << ": " << (msg) << std::endl;\
                                }\
                            } while (false);
#else
#define DEBUG_LOG(output, tag, msg)
#endif

#ifdef TRACK_RESOURCES
#define INIT_LOG(name) DEBUG_LOG(RESOURCES, "resource", std::string(name) + " initialized")
#define DESTROY_LOG(name) DEBUG_LOG(RESOURCES, "resource", std::string(name) + " destroyed")
#else
#define INIT_LOG(name)
#define DESTROY_LOG(name)
#endif

#endif