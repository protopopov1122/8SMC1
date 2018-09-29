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

#ifndef CALX_CTRL_LIB_PLANE_COORD_PLANE_LOG_H_
#define CALX_CTRL_LIB_PLANE_COORD_PLANE_LOG_H_

#include "ctrl-lib/plane/ProxyCoordPlane.h"

namespace CalX {

	class CoordPlaneLog : public ProxyCoordPlane {
	 public:
		CoordPlaneLog(std::shared_ptr<CoordPlane>, std::ostream &, std::string,
		              bool = true, bool = true);
		std::string getPrefix() const;
		bool isLoggingActions() const;
		bool isLoggingErrors() const;
		void setPrefix(std::string);
		void setLoggingActions(bool);
		void setLoggingErrors(bool);

		virtual ErrorCode move(motor_point_t, float, bool);
		virtual ErrorCode arc(motor_point_t, motor_point_t, int, float, bool,
		                      float = 1.0f);
		virtual ErrorCode calibrate(TrailerId);
		virtual ErrorCode measure(TrailerId);
		virtual std::ostream &operator<<(std::ostream &) const;
		virtual std::unique_ptr<CoordPlane> clone(std::shared_ptr<CoordPlane>);

	 private:
		std::ostream &out;
		std::string prefix;
		bool log_actions;
		bool log_errors;
	};
}  // namespace CalX

#endif
