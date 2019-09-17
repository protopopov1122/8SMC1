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

#ifndef CALX_CTRL_LIB_TRANSLATOR_POLAR_COORD_TRANSLATOR_H_
#define CALX_CTRL_LIB_TRANSLATOR_POLAR_COORD_TRANSLATOR_H_

#include "calx/ctrl-lib/translator/AbstractCoordTranslator.h"

namespace CalX {

	class PolarCoordTranslator : public CoordTranslator {
	 public:
		PolarCoordTranslator(std::shared_ptr<CoordTranslator> = nullptr);
		std::shared_ptr<CoordTranslator> getBase() const override;
		void setBase(std::shared_ptr<CoordTranslator>) override;
		motor_point_t get(double, double) const override;
		coord_point_t get(motor_point_t) const override;
		coord_point_t floatGet(double, double) const override;
		coord_point_t floatGet(coord_point_t) const override;
		std::unique_ptr<CoordTranslator> clone(
		    std::shared_ptr<CoordTranslator>) override;

	 private:
		std::shared_ptr<CoordTranslator> base;
	};
}  // namespace CalX

#endif
