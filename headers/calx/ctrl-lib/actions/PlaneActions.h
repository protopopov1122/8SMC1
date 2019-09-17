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

#ifndef CALX_CTRL_LIB_ACTIONS_PLANE_ACTIONS_H_
#define CALX_CTRL_LIB_ACTIONS_PLANE_ACTIONS_H_

#include "calx/ctrl-lib/actions/ActionQueue.h"
#include "calx/ctrl-lib/plane/CoordPlane.h"
#include "calx/ctrl-lib/plane/CoordHandle.h"
#include "calx/ctrl-lib/logger/Journal.h"
#include "calx/ctrl-lib/task/CoordTask.h"
#include "calx/ctrl-lib/translator/CoordTranslator.h"

namespace CalX {

	class CalxCoordBaseAction : public CalxAction {
	 public:
		CalxCoordBaseAction(std::shared_ptr<CoordHandle>, ErrorHandlerCallback,
		                    JournalLogger &);

	 protected:
		std::shared_ptr<CoordHandle> handle;
		ErrorHandlerCallback error_handler;
		JournalLogger &journal;
	};

	class CalxCoordActionMove : public CalxCoordBaseAction {
	 public:
		CalxCoordActionMove(std::shared_ptr<CoordHandle>, ErrorHandlerCallback,
		                    JournalLogger &, coord_point_t, double, bool, bool);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		coord_point_t dest;
		double speed;
		bool sync;
		bool relative;
	};

	class CalxCoordActionArc : public CalxCoordBaseAction {
	 public:
		CalxCoordActionArc(std::shared_ptr<CoordHandle>, ErrorHandlerCallback,
		                   JournalLogger &, coord_point_t, coord_point_t, int,
		                   double, bool, bool);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		coord_point_t dest;
		coord_point_t cen;
		int splitter;
		double speed;
		bool clockwise;
		bool relative;
	};

	class CalxCoordActionCalibrate : public CalxCoordBaseAction {
	 public:
		CalxCoordActionCalibrate(std::shared_ptr<CoordHandle>, ErrorHandlerCallback,
		                         JournalLogger &, TrailerId);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		TrailerId trailer;
	};

	class CalxCoordActionMeasure : public CalxCoordBaseAction {
	 public:
		CalxCoordActionMeasure(std::shared_ptr<CoordHandle>, ErrorHandlerCallback,
		                       JournalLogger &, TrailerId);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		TrailerId trailer;
	};

	class CalxCoordActionConfigure : public CalxCoordBaseAction {
	 public:
		CalxCoordActionConfigure(std::shared_ptr<CoordHandle>, ErrorHandlerCallback,
		                         JournalLogger &, PlaneMapper &, coord_point_t,
		                         double);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		PlaneMapper &mapper;
		coord_point_t dest;
		double speed;
		bool work;
	};

	class CalxCoordActionGraphBuild : public CalxCoordBaseAction {
	 public:
		CalxCoordActionGraphBuild(std::shared_ptr<CoordHandle>,
		                          ErrorHandlerCallback, JournalLogger &,
		                          std::shared_ptr<CoordTranslator>,
		                          std::unique_ptr<GraphBuilder>, double);

		ErrorCode perform(SystemManager &) override;
		void stop() override;

	 private:
		std::shared_ptr<CoordTranslator> translator;
		std::unique_ptr<GraphBuilder> builder;
		double speed;
		std::shared_ptr<TaskState> state;
	};
}  // namespace CalX

#endif
