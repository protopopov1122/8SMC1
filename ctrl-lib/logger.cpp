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

#include "ctrl-lib/logger.h"
#include <iostream>

std::ostream *__ERRORS = nullptr;
std::ostream *__WARNINGS = nullptr;
std::ostream *__DEBUG = nullptr;
std::ostream *__INFO = nullptr;
std::ostream *__RESOURCES = nullptr;
std::ostream *__INSTRUMENTS = nullptr;

std::ostream **getStreamPtr(std::string str) {
	if (str.compare(ERRORS) == 0) {
		return &__ERRORS;
	} else if (str.compare(WARNINGS) == 0) {
		return &__WARNINGS;
	} else if (str.compare(INFO) == 0) {
		return &__INFO;
	} else if (str.compare(RESOURCES) == 0) {
		return &__RESOURCES;
	} else if (str.compare(DEBUG) == 0) {
		return &__DEBUG;
	} else if (str.compare(INSTRUMENTS) == 0) {
		return &__INSTRUMENTS;
	} else {
		return nullptr;
	}
}

void SET_LOGGER(std::string str, std::ostream *s) {
	std::ostream **ptr = getStreamPtr(str);
	if (ptr != nullptr) {
		*ptr = s;
	}
}
