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

#ifndef CALX_CTRL_LIB_CONF_INI_H_
#define CALX_CTRL_LIB_CONF_INI_H_

#include "calx/ctrl-lib/conf/Dictionary.h"
#include <iosfwd>

namespace CalX {

	class INIConfiguration {
	 public:
		static void store(ConfigurationCatalogue &, std::ostream &);
		static std::unique_ptr<ConfigurationCatalogue> load(
		    std::istream &, std::ostream &, ConfigurationCatalogue * = nullptr);

	 private:
		static void store(ConfiguationFlatDictionary &, std::ostream &);
	};
}  // namespace CalX

#endif