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

#ifndef CALX_UI_CALX_UNIT_PROCESSOR_H_
#define CALX_UI_CALX_UNIT_PROCESSOR_H_

#include "ui/calx.h"

namespace CalXUI {

	class CalxUnitProcessor {
	 public:
		CalxUnitProcessor(ConfigManager &);
		std::string formatDouble(double);
		std::string getUnits();
		std::string getSpeedUnits();
		double getUnitPrecision();
		double getSpeedPrecision();
		double getUnitScale();
		double getSpeedScale();
		coord_point_t getUnitOffset();

	 private:
		ConfigManager &config;
	};
}  // namespace CalXUI

#endif