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
#include "device/DeviceManager.h"
#include "CtrlCore.h"
#include "EventListener.h"

namespace CalX {
	
	class InstrumentController {
		public:
			InstrumentController(Instrument*);
			virtual ~InstrumentController();
			Instrument *getInstrument();
			device_id_t getID();
			bool isEnabled();
			void enable(bool);
			void flipState();
			
			bool isRunnable();
			void setRunnable(bool);
			std::string getInfo();
			
			void addEventListener(InstrumentEventListener*);
			void removeEventListener(InstrumentEventListener*);
			void use();
			void unuse();
		protected:
			void sendStateChanged();
		private:
			Instrument *instr;
			bool state;
			std::vector<InstrumentEventListener*> listeners;
	};
}


#endif