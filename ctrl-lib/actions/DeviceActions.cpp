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

#include "ctrl-lib/actions/DeviceActions.h"
#include "ctrl-lib/SystemManager.h"
#include "ctrl-lib/logger/Shortcuts.h"

namespace CalX {

	void deviceActionFinished(DeviceController &device, ErrorCode errcode,
	                          JournalLogger &journal) {
		Info(journal) << "Device #" << device.getID()
		              << " action finished with error code "
		              << static_cast<int>(errcode);
	}

	void deviceActionStopped(DeviceController &device, JournalLogger &journal) {
		Info(journal) << "Device #" << device.getID() << " action stopped";
	}

	CalxMotorBaseAction::CalxMotorBaseAction(MotorController &device,
	                                         ErrorHandlerCallback error_handler,
	                                         JournalLogger &journal)
	    : device(device), error_handler(error_handler), journal(journal) {}

	CalxMotorMoveAction::CalxMotorMoveAction(MotorController &device,
	                                         ErrorHandlerCallback error_handler,
	                                         JournalLogger &logger,
	                                         int destination, float speed,
	                                         bool relative)
	    : CalxMotorBaseAction::CalxMotorBaseAction(device, error_handler, logger),
	      destination(destination),
	      speed(speed),
	      relative(relative) {}

	ErrorCode CalxMotorMoveAction::perform(SystemManager &sysman) {
		ErrorCode errcode;
		Info(this->journal) << "Starting device #" << this->device.getID()
		                    << (this->relative ? " relative " : " ") << "move"
		                    << " to " << this->destination << " with speed "
		                    << this->speed;
		this->device.use();
		if (this->relative) {
			errcode = this->device.startRelativeMove(this->destination, this->speed);
		} else {
			errcode = this->device.startMove(this->destination, this->speed);
		}
		this->device.unuse();
		this->error_handler(errcode);
		deviceActionFinished(device, errcode, this->journal);
		return errcode;
	}

	void CalxMotorMoveAction::stop() {
		this->device.stop();
	}

	CalxMotorCalibrationAction::CalxMotorCalibrationAction(
	    MotorController &device, ErrorHandlerCallback error_handler,
	    JournalLogger &logger, TrailerId tr)
	    : CalxMotorBaseAction::CalxMotorBaseAction(device, error_handler, logger),
	      trailer(tr) {}

	ErrorCode CalxMotorCalibrationAction::perform(SystemManager &sysman) {
		Info(this->journal) << "Starting device #" << this->device.getID()
		                    << " calibration to trailer #"
		                    << static_cast<int>(this->trailer);
		this->device.use();
		ErrorCode errcode = this->device.moveToTrailer(this->trailer);
		this->device.unuse();
		this->error_handler(errcode);
		return errcode;
	}

	void CalxMotorCalibrationAction::stop() {
		this->device.stop();
		deviceActionStopped(this->device, this->journal);
	}

	CalxInstrumentBaseAction::CalxInstrumentBaseAction(
	    InstrumentController &instrument, ErrorHandlerCallback error_handler,
	    JournalLogger &journal)
	    : instrument(instrument),
	      error_handler(error_handler),
	      journal(journal) {}

	CalxInstrumentEnableAction::CalxInstrumentEnableAction(
	    InstrumentController &instrument, ErrorHandlerCallback error_handler,
	    JournalLogger &journal)
	    : CalxInstrumentBaseAction::CalxInstrumentBaseAction(
	          instrument, error_handler, journal) {}

	ErrorCode CalxInstrumentEnableAction::perform(SystemManager &sysman) {
		this->instrument.use();
		ErrorCode errcode = this->instrument.flipState();
		this->error_handler(errcode);
		this->instrument.unuse();
		return errcode;
	}

	void CalxInstrumentEnableAction::stop() {}

	CalxInstrumentSessionAction::CalxInstrumentSessionAction(
	    InstrumentController &instrument, ErrorHandlerCallback error_handler,
	    JournalLogger &journal)
	    : CalxInstrumentBaseAction::CalxInstrumentBaseAction(
	          instrument, error_handler, journal) {}

	ErrorCode CalxInstrumentSessionAction::perform(SystemManager &sysman) {
		this->instrument.use();
		ErrorCode errcode = ErrorCode::NoError;
		if (this->instrument.isSessionOpened()) {
			errcode = this->instrument.close_session();
		} else {
			errcode = this->instrument.open_session();
		}
		this->error_handler(errcode);
		this->instrument.unuse();
		return errcode;
	}

	void CalxInstrumentSessionAction::stop() {}

	CalxInstrumentModeAction::CalxInstrumentModeAction(
	    InstrumentController &instrument, ErrorHandlerCallback error_handler,
	    JournalLogger &journal, InstrumentMode mode)
	    : CalxInstrumentBaseAction::CalxInstrumentBaseAction(
	          instrument, error_handler, journal),
	      mode(mode) {}

	ErrorCode CalxInstrumentModeAction::perform(SystemManager &sysman) {
		this->instrument.use();
		this->instrument.setMode(this->mode);
		this->instrument.unuse();
		return ErrorCode::NoError;
	}

	void CalxInstrumentModeAction::stop() {}
}  // namespace CalX