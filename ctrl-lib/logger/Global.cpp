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

#include "ctrl-lib/logger/Global.h"

namespace CalX {

  static const std::string Errors = "errors";
  static const std::string Warnings = "warnings";
  static const std::string Debug = "debug";
  static const std::string Information = "info";
  static const std::string Resources = "resources";
  static const std::string Instruments = "instruments";

  JournalDefaultSession GlobalLogger::logger(LoggingSeverity::Debug);

  JournalSession &GlobalLogger::getLogger() {
    return GlobalLogger::logger;
  }

  const std::string &GlobalLogger::getSink(GlobalLoggingSink id) {
    switch (id) {
      case GlobalLoggingSink::Errors:
        return Errors;
      case GlobalLoggingSink::Warnings:
        return Warnings;
      case GlobalLoggingSink::Debug:
        return Debug;
      case GlobalLoggingSink::Information:
        return Information;
      case GlobalLoggingSink::Resources:
        return Resources;
      case GlobalLoggingSink::Instruments:
        return Instruments;
      default:
        return Information;
    }
  }

  void GlobalLogger::setupSink(GlobalLoggingSink id, JournalSinkFactory &factory) {
    GlobalLogger::logger.newSink(GlobalLogger::getSink(id), factory, id == GlobalLoggingSink::Information);
  }
}