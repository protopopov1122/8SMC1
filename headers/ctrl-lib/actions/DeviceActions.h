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

#ifndef CALX_CTRL_LIB_ACTIONS_DEVICE_ACTIONS_H_
#define CALX_CTRL_LIB_ACTIONS_DEVICE_ACTIONS_H_

#include "ctrl-lib/actions/ActionQueue.h"
#include "ctrl-lib/ctrl/MotorController.h"
#include "ctrl-lib/ctrl/InstrumentController.h"
#include "ctrl-lib/logger/Journal.h"

namespace CalX {

	class CalxMotorBaseAction : public CalxAction {
	 public:
		CalxMotorBaseAction(MotorController &, ErrorHandlerCallback,
		                    JournalLogger &);

	 protected:
		MotorController &device;
		ErrorHandlerCallback error_handler;
		JournalLogger &journal;
	};

	class CalxMotorMoveAction : public CalxMotorBaseAction {
	 public:
		CalxMotorMoveAction(MotorController &, ErrorHandlerCallback,
		                    JournalLogger &, int, float, bool);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		int destination;
		float speed;
		bool relative;
	};

	class CalxMotorCalibrationAction : public CalxMotorBaseAction {
	 public:
		CalxMotorCalibrationAction(MotorController &, ErrorHandlerCallback,
		                           JournalLogger &, TrailerId);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		TrailerId trailer;
	};

	class CalxInstrumentBaseAction : public CalxAction {
	 public:
		CalxInstrumentBaseAction(InstrumentController &, ErrorHandlerCallback,
		                         JournalLogger &);

	 protected:
		InstrumentController &instrument;
		ErrorHandlerCallback error_handler;
		JournalLogger &journal;
	};

	class CalxInstrumentEnableAction : public CalxInstrumentBaseAction {
	 public:
		CalxInstrumentEnableAction(InstrumentController &, ErrorHandlerCallback,
		                           JournalLogger &);

		ErrorCode perform(SystemManager &) override;
		void stop() override;
	};

	class CalxInstrumentSessionAction : public CalxInstrumentBaseAction {
	 public:
		CalxInstrumentSessionAction(InstrumentController &, ErrorHandlerCallback,
		                            JournalLogger &);

		ErrorCode perform(SystemManager &sysm) override;
		void stop() override;
	};

	class CalxInstrumentModeAction : public CalxInstrumentBaseAction {
	 public:
		CalxInstrumentModeAction(InstrumentController &, ErrorHandlerCallback,
		                         JournalLogger &, InstrumentMode);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		InstrumentMode mode;
	};
}  // namespace CalX

#endif