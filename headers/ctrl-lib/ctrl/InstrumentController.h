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

#ifndef CALX_CTRL_LIB_INSTRUMENT_CONTROLLER_H_
#define CALX_CTRL_LIB_INSTRUMENT_CONTROLLER_H_

#include <vector>
#include "ctrl-lib/device/DeviceManager.h"
#include "ctrl-lib/CtrlCore.h"
#include "ctrl-lib/EventListener.h"

/* Instrument controller is higher-level interface for
   interface drivers. It offers a bit more complex functionality
   comparing to device driver. */

namespace CalX {

  class InstrumentController {
   public:
	InstrumentController(Instrument *);
	virtual ~InstrumentController();
	Instrument *getInstrument();
	device_id_t getID();
	bool isSessionOpened();
	ErrorCode open_session();
	ErrorCode close_session();
	bool isEnabled();
	ErrorCode enable(bool);
	ErrorCode flipState();

	bool isRunnable();
	void setRunnable(bool);
	std::string getInfo();

	InstrumentMode getMode();
	bool setMode(InstrumentMode);
	ConfigManager *getConfiguration();

	void addEventListener(InstrumentEventListener *);
	void removeEventListener(InstrumentEventListener *);
	void use();
	void unuse();

	void work(bool);

   protected:
	void sendStateChanged();

   private:
	Instrument *instr;
	bool session_state;
	bool state;
	std::vector<InstrumentEventListener *> listeners;
  };
}

#endif