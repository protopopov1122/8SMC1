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


#ifndef CALX_UI_CALX_UNIT_PROCESSOR_H_
#define CALX_UI_CALX_UNIT_PROCESSOR_H_

#include "CalxApp.h"

namespace CalXUI {

	class CalxUnitProcessor {
		public:
			CalxUnitProcessor(std::string, double);
			virtual ~CalxUnitProcessor();
			
			double getScale();
			std::string getSuffix();
			
			double toUnits(int64_t);
			int64_t toSteps(double);
			std::string toTextUnits(int64_t);
		private:
			std::string suffix;
			double scale;
	};
}

#endif