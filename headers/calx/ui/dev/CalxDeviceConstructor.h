/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALX_UI_DEV_CALX_DEVICE_CONSTRUCTOR_H_
#define CALX_UI_DEV_CALX_DEVICE_CONSTRUCTOR_H_

#include "calx/ui/dev/CalxDeviceHandle.h"
#include "calx/ui/dev/CalxInstrumentComponent.h"
#include "calx/ui/dev/CalxMotorComponent.h"

namespace CalX::UI {

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
}  // namespace CalX::UI

#endif