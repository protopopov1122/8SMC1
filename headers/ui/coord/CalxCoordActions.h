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

#ifndef CALX_UI_COORD_CALX_COORD_ACTIONS_H_
#define CALX_UI_COORD_CALX_COORD_ACTIONS_H_

#include "ui/CalxActionQueue.h"
#include "ui/CalxApp.h"
#include "ui/coord/CalxCoordController.h"
#include "ui/coord/CalxVirtualPlane.h"

namespace CalXUI {

	class CalxCoordActionMove : public CalxAction {
	 public:
		CalxCoordActionMove(std::shared_ptr<CoordHandle>, coord_point_t, double,
		                    bool, bool, ActionResult * = nullptr);

		virtual void perform(SystemManager *);
		virtual void stop();

	 private:
		std::shared_ptr<CoordHandle> handle;
		coord_point_t dest;
		double speed;
		bool sync;
		bool relative;
		ActionResult *action_result;
	};

	class CalxCoordActionArc : public CalxAction {
	 public:
		CalxCoordActionArc(std::shared_ptr<CoordHandle>, coord_point_t,
		                   coord_point_t, int, double, bool, bool,
		                   ActionResult * = nullptr);

		virtual void perform(SystemManager *);
		virtual void stop();

	 private:
		std::shared_ptr<CoordHandle> handle;
		coord_point_t dest;
		coord_point_t cen;
		int splitter;
		double speed;
		bool clockwise;
		bool relative;
		ActionResult *action_result;
	};

	class CalxCoordActionCalibrate : public CalxAction {
	 public:
		CalxCoordActionCalibrate(std::shared_ptr<CoordHandle>, TrailerId,
		                         ActionResult * = nullptr);

		virtual void perform(SystemManager *);
		virtual void stop();

	 private:
		std::shared_ptr<CoordHandle> handle;
		TrailerId trailer;
		ActionResult *action_result;
	};

	class CalxCoordActionMeasure : public CalxAction {
	 public:
		CalxCoordActionMeasure(std::shared_ptr<CoordHandle>, TrailerId,
		                       ActionResult * = nullptr);

		virtual void perform(SystemManager *);
		virtual void stop();

	 private:
		std::shared_ptr<CoordHandle> handle;
		TrailerId trailer;
		ActionResult *action_result;
	};

	class CalxCoordActionConfigure : public CalxAction {
	 public:
		CalxCoordActionConfigure(std::shared_ptr<CoordHandle>,
		                         CalxCoordController *, coord_point_t, double,
		                         ActionResult * = nullptr);

		virtual void perform(SystemManager *);
		virtual void stop();

	 private:
		std::shared_ptr<CoordHandle> handle;
		CalxCoordController *controller;
		coord_point_t dest;
		double speed;
		bool work;
		ActionResult *action_result;
	};

	class CalxCoordActionGraphBuild : public CalxAction {
	 public:
		CalxCoordActionGraphBuild(std::shared_ptr<CoordHandle>,
		                          std::shared_ptr<CoordTranslator>,
		                          std::unique_ptr<GraphBuilder>, double);

		virtual void perform(SystemManager *);
		virtual void stop();

	 private:
		std::shared_ptr<CoordHandle> handle;
		std::shared_ptr<CoordTranslator> translator;
		std::unique_ptr<GraphBuilder> builder;
		double speed;
		std::shared_ptr<TaskState> state;
	};

	class CalxCoordActionGraphPreview : public CalxAction {
	 public:
		CalxCoordActionGraphPreview(CalxVirtualPlaneDialog *,
		                            std::shared_ptr<CoordTranslator>,
		                            std::unique_ptr<GraphBuilder>, double);

		virtual void perform(SystemManager *);
		virtual void stop();

	 private:
		CalxVirtualPlaneDialog *dialog;
		std::shared_ptr<CoordTranslator> translator;
		std::unique_ptr<GraphBuilder> builder;
		double speed;
		std::shared_ptr<TaskState> state;
	};
}  // namespace CalXUI

#endif
