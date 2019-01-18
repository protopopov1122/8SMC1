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

#ifndef CALX_CTRL_LIB_TRANSLATOR_COMPLEX_COORD_TRANSLATOR_H_
#define CALX_CTRL_LIB_TRANSLATOR_COMPLEX_COORD_TRANSLATOR_H_

#include "ctrl-lib/translator/AbstractCoordTranslator.h"
#include <vector>

namespace CalX {

	class ComplexCoordTranslator : public CoordTranslator {
	 public:
		ComplexCoordTranslator(std::shared_ptr<CoordTranslator>);
		motor_point_t get(double, double) const override;
		coord_point_t get(motor_point_t) const override;
		coord_point_t floatGet(double, double) const override;
		coord_point_t floatGet(coord_point_t) const override;

		std::size_t getSize() const;
		std::shared_ptr<CoordTranslator> getTranslator(size_t) const;
		void add(std::shared_ptr<CoordTranslator>);
		bool remove(std::size_t);
		bool insert(std::size_t, std::shared_ptr<CoordTranslator>);
		std::shared_ptr<CoordTranslator> getBase() const override;
		void setBase(std::shared_ptr<CoordTranslator>) override;
		std::unique_ptr<CoordTranslator> clone(
		    std::shared_ptr<CoordTranslator>) override;

	 private:
		std::vector<std::shared_ptr<CoordTranslator>> list;
	};
}  // namespace CalX

#endif
