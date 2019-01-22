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

#include "ui/CalxJournalManager.h"
#include "ctrl-lib/logger/TimedJournal.h"
#include "ctrl-lib/logger/Logger.h"
#include <wx/dir.h>
#include <iomanip>
#include <sstream>
#include <utility>
#include <algorithm>
#include <wx/filename.h>

namespace CalXUI {

	static constexpr auto FullTimestampFormat = "%H-%M-%S_%d-%m-%Y";
	static constexpr auto ShortTimestampFormat = "%d-%m-%Y";

	static std::string FullLogName(const std::string &directory, ConfigurationCatalogue &conf) {
		std::stringstream ss;
		ss << directory << static_cast<char>(wxFileName::GetPathSeparator())
		   << (conf.getEntry(CalxConfiguration::Logging)->getBool(CalxLoggingConfiguration::DailyJournal, false) ? ShortTimestampFormat : FullTimestampFormat) << ".log";
		return ss.str();
	}

	CalxJournalManager::CalxJournalManager(ConfigurationCatalogue &conf)
		: config(conf) {
		this->journal = nullptr;
		if (conf.getEntry(CalxConfiguration::Logging)
		        ->has(CalxLoggingConfiguration::JournalDir)) {
			this->journalDirectory =
			    conf.getEntry(CalxConfiguration::Logging)
			        ->getString(CalxLoggingConfiguration::JournalDir);
			if (!wxDir::Exists(this->journalDirectory)) {
				wxDir::Make(this->journalDirectory);
			}
			std::string logName = FullLogName(this->journalDirectory, conf);
			this->journal =
			    std::make_unique<TimedJournal>([this, logName](auto timepoint) {
				    std::time_t time = std::chrono::system_clock::to_time_t(timepoint);
				    std::stringstream ss;
				    ss << std::put_time(std::localtime(&time), logName.c_str());
				    return ss.str();
			    });
			this->vacuum(conf);
		} else {
			this->journal = std::make_unique<DefaultJournal>();
			this->journal->getSessionController().dropSink("default");
			this->journal->getSessionController().newNullSink("default");
		}
	}

	JournalLogger &CalxJournalManager::getJournal() {
		return this->journal->getSession();
	}

	void CalxJournalManager::vacuum(ConfigurationCatalogue &conf) {
		std::vector<std::pair<std::string, std::chrono::system_clock::time_point>>
		    files;
		this->getJournalFiles(files);
		std::sort(files.begin(), files.end(), [](const auto &f1, const auto &f2) {
			return f1.second <= f2.second;
		});
		if (conf.getEntry(CalxConfiguration::Logging)
		        ->has(CalxLoggingConfiguration::MaxSavedSessions)) {
			std::size_t saved_sessions =
			    conf.getEntry(CalxConfiguration::Logging)
			        ->getInt(CalxLoggingConfiguration::MaxSavedSessions);
			if (files.size() > saved_sessions) {
				std::vector<
				    std::pair<std::string, std::chrono::system_clock::time_point>>
				    removedFiles(files.begin(),
				                 files.begin() + (files.size() - saved_sessions));
				for (const auto &file : removedFiles) {
					wxRemoveFile(file.first);
					std::remove(files.begin(), files.end(), file);
				}
			}
		}
	}

	void CalxJournalManager::getJournalFiles(
	    std::vector<std::pair<std::string, std::chrono::system_clock::time_point>>
	        &files) {
		wxArrayString fileArr;
		wxDir::GetAllFiles(this->journalDirectory, &fileArr);
		std::string logName = FullLogName(this->journalDirectory, this->config);
		std::stringstream ss;
		for (std::size_t i = 0; i < fileArr.GetCount(); i++) {
			std::tm tm = {};
			;
			ss.str(fileArr.Item(i).ToStdString());
			ss >> std::get_time(&tm, logName.c_str());
			if (!ss.fail()) {
				std::chrono::system_clock::time_point timepoint =
				    std::chrono::system_clock::from_time_t(std::mktime(&tm));
				files.push_back(
				    std::make_pair(fileArr.Item(i).ToStdString(), timepoint));
			}
		}
	}
}  // namespace CalXUI