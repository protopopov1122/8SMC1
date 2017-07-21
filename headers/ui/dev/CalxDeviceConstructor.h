/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_UI_DEV_CALX_DEVICE_CONSTRUCTOR_H_
#define CALX_UI_DEV_CALX_DEVICE_CONSTRUCTOR_H_

#include "ui/dev/CalxDeviceHandle.h"
#include "ui/dev/CalxInstrumentComponent.h"
#include "ui/dev/CalxMotorComponent.h"

namespace CalXUI {

	class CalxMotorConstructor : public CalxDeviceConstructor {
	 public:
		CalxMotorConstructor(CalxDevicePool *, std::shared_ptr<MotorController>);
		CalxDeviceHandle *construct(wxWindow *);

	 private:
		CalxDevicePool *pool;
		std::shared_ptr<MotorController> ctrl;
	};

	class CalxInstrumentConstructor : public CalxDeviceConstructor {
	 public:
		CalxInstrumentConstructor(CalxDevicePool *,
		                          std::shared_ptr<InstrumentController>);
		CalxDeviceHandle *construct(wxWindow *win);

	 private:
		CalxDevicePool *pool;
		std::shared_ptr<InstrumentController> ctrl;
	};
}

#endif