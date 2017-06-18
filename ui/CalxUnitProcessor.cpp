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


#include "ui/CalxUnitProcessor.h"

namespace CalXUI {

	CalxUnitProcessor::CalxUnitProcessor(std::string suf, double sc) {
		this->suffix = suf;
		this->scale = sc;
	}

	CalxUnitProcessor::~CalxUnitProcessor() {

	}

	std::string CalxUnitProcessor::getSuffix() {
		return this->suffix;
	}

	double CalxUnitProcessor::getScale() {
		return this->scale;
	}

	double CalxUnitProcessor::toUnits(int64_t i) {
		return ((double) i) * this->scale;
	}

	std::string CalxUnitProcessor::toUnitsStr(int64_t i) {
		double u = toUnits(i);
		std::string str = std::to_string(u);
		size_t end = str.length();
		while (str.at(end - 1) == '0') {
			end--;
		}
		if (str.at(end - 1) == '.') {
			end--;
		}

		return str.substr(0, end);
	}

	int64_t CalxUnitProcessor::toSteps(double u) {
		return (int64_t) (u / this->scale);
	}

	std::string CalxUnitProcessor::toTextUnits(int64_t i) {
		std::string s = std::to_string(toUnits(i));
		if (!this->suffix.empty()) {
			s += " " + this->suffix;
		}
		return s;
	}
}
