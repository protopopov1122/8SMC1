/*
        SPDX-License-Identifier: LGPL-3.0

        Copyright (c) 2017-2019 Jevgenijs Protopopovs

        This file is part of CalX project.

        CalX is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License version 3 as published
   by
        the Free Software Foundation.


        CalX is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with CalX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "calx/ctrl-lib/script/ScriptEngine.h"

namespace CalX {

	CalXScriptEnvironment::CalXScriptEnvironment(ConfigurationCatalogue &conf,
	                                             SettingsRepository *settings)
	    : config(conf), settings(settings) {}

	ConfigurationCatalogue &CalXScriptEnvironment::getConfiguration() {
		return this->config;
	}

	SettingsRepository *CalXScriptEnvironment::getSettings() {
		return this->settings;
	}

	CalXScript::CalXScript(CalXScriptEnvironment &env) : env(env) {}
}  // namespace CalX
