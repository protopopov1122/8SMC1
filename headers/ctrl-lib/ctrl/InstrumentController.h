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

#ifndef CALX_CTRL_LIB_INSTRUMENT_CONTROLLER_H_
#define CALX_CTRL_LIB_INSTRUMENT_CONTROLLER_H_

#include "ctrl-lib/ctrl/DeviceController.h"

/* Instrument controller is higher-level interface for
   interface drivers. It offers a bit more complex functionality
   comparing to device driver. */

namespace CalX {

	class InstrumentController : public DeviceController,
	                             public SessionableResource {
	 public:
		InstrumentController(ConfigManager &, Instrument &);
		Instrument &getInstrument() const;
		virtual bool isSessionOpened() const;
		virtual ErrorCode open_session();
		virtual ErrorCode close_session();
		bool isEnabled() const;
		ErrorCode enable(bool);
		ErrorCode flipState();

		bool isRunnable() const;
		void setRunnable(bool);
		std::string getInfo() const;

		InstrumentMode getMode() const;
		bool setMode(InstrumentMode);

		void addEventListener(std::shared_ptr<InstrumentEventListener>);
		void removeEventListener(std::shared_ptr<InstrumentEventListener>);
		virtual void use();
		virtual void unuse();

	 protected:
		void sendStateChanged();

	 private:
		Instrument &instr;
		bool state;
		std::vector<std::shared_ptr<InstrumentEventListener>> listeners;
	};
}  // namespace CalX

#endif
