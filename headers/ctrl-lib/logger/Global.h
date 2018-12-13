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

#ifndef CALX_CTRL_LIB_LOGGER_GLOBAL_H_
#define CALX_CTRL_LIB_LOGGER_GLOBAL_H_

#include "ctrl-lib/logger/Journal.h"
#include "ctrl-lib/logger/Session.h"

namespace CalX {

  enum class GlobalLoggingSink {
    Errors,
    Warnings,
    Debug,
    Information,
    Resources,
    Instruments
  };

  class GlobalLogger {
   public:
    static JournalSession &getLogger();
    static const std::string &getSink(GlobalLoggingSink);
    static JournalSessionController &getController();
   private:
    static JournalDefaultSession logger;
  };
}

#endif