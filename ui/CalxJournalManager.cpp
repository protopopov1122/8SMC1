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

#include "ui/CalxJournalManager.h"
#include "ctrl-lib/logger/TimedJournal.h"
#include "ctrl-lib/logger/Logger.h"
#include <wx/dir.h>
#include <iomanip>

namespace CalXUI {

	CalxJournalManager::CalxJournalManager(ConfigurationCatalogue &conf) {
		this->journal = nullptr;
		if (conf.getEntry(CalxConfiguration::Logging)
		        ->has(CalxLoggingConfiguration::JournalDir)) {
			this->journalDirectory =
			    conf.getEntry(CalxConfiguration::Logging)
			        ->getString(CalxLoggingConfiguration::JournalDir);
			if (!wxDir::Exists(this->journalDirectory)) {
				wxDir::Make(this->journalDirectory);
			}
			this->journal = std::make_unique<TimedJournal>([this](auto timepoint) {
				auto epochId =
				    std::chrono::time_point_cast<std::chrono::seconds>(timepoint)
				        .time_since_epoch()
				        .count();
				std::time_t time = std::chrono::system_clock::to_time_t(timepoint);
				std::stringstream ss;
				ss << this->journalDirectory << '/' << epochId << '_'
				   << std::put_time(std::localtime(&time), "%H-%M_%d-%m-%Y") << ".log";
				return ss.str();
			});
		} else {
			this->journal = std::make_unique<DefaultJournal>();
			this->journal->getSessionController().dropSink("default");
			this->journal->getSessionController().newNullSink("default");
		}
	}

	JournalLogger &CalxJournalManager::getJournal() {
		return this->journal->getSession();
	}
}  // namespace CalXUI