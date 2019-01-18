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

#include "ui/CalxApp.h"
#include "ui/CalxErrorHandler.h"
#include "ui/coord/CalxCoordPane.h"

namespace CalXUI {

	void CalxCoordTimer::Notify() {
		ctrl->updateUI();
	}

	CalxCoordEventListener::CalxCoordEventListener(CalxCoordPane *ctrl) {
		this->ctrl = ctrl;
	}

	CalxCoordEventListener::~CalxCoordEventListener() {}

	void CalxCoordEventListener::onUse() {
		ctrl->use();
	}

	void CalxCoordEventListener::onUnuse() {
		ctrl->unuse();
	}

	CalxCoordMotorListener::CalxCoordMotorListener(CalxCoordPane *ctrl) {
		this->ctrl = ctrl;
	}

	CalxCoordMotorListener::~CalxCoordMotorListener() {}

	void CalxCoordMotorListener::onUse() {
		ctrl->use();
	}

	void CalxCoordMotorListener::onUnuse() {
		ctrl->unuse();
	}

	CalxCoordInstrumentListener::CalxCoordInstrumentListener(
	    CalxCoordPane *ctrl) {
		this->ctrl = ctrl;
	}

	CalxCoordInstrumentListener::~CalxCoordInstrumentListener() {}

	void CalxCoordInstrumentListener::onUse() {
		ctrl->use();
	}

	void CalxCoordInstrumentListener::onUnuse() {
		ctrl->unuse();
	}
}  // namespace CalXUI
