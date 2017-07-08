/*
	Copyright (c) 2017 Jevgenijs Protopopovs

	This file is part of CalX project.

	CalX is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
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

#include "ui/CalxApp.h"
#include "ui/CalxActionQueue.h"
#include "ui/coord/CalxFilterController.h"
#include "ui/coord/CalxVirtualPlane.h"

namespace CalXUI {

  class CalxCoordActionMove : public CalxAction {
   public:
	CalxCoordActionMove(CoordHandle *, coord_point_t, double, bool, bool);

	virtual void perform(SystemManager *);
	virtual void stop();

   private:
	CoordHandle *handle;
	coord_point_t dest;
	double speed;
	bool jump;
	bool relative;
  };

  class CalxCoordActionArc : public CalxAction {
   public:
	CalxCoordActionArc(CoordHandle *, coord_point_t,
							coord_point_t, int, double, bool, bool);

	virtual void perform(SystemManager *);
	virtual void stop();

   private:
	CoordHandle *handle;
	coord_point_t dest;
	coord_point_t cen;
	int splitter;
	double speed;
	bool clockwise;
	bool relative;
  };
  

  class CalxCoordActionCalibrate : public CalxAction {
   public:
	CalxCoordActionCalibrate(CoordHandle *, TrailerId);

	virtual void perform(SystemManager *);
	virtual void stop();

   private:
	CoordHandle *handle;
	TrailerId trailer;
  };

  class CalxCoordActionMeasure : public CalxAction {
   public:
	CalxCoordActionMeasure(CoordHandle *, TrailerId);

	virtual void perform(SystemManager *);
	virtual void stop();

   private:
	CoordHandle *handle;
	TrailerId trailer;
  };

 class CalxCoordActionConfigure : public CalxAction {
   public:
	CalxCoordActionConfigure(CoordHandle *, CalxFilterController *, coord_point_t, double);

	virtual void perform(SystemManager *);
	virtual void stop();

   private:
	CoordHandle *handle;
	CalxFilterController *filters;
	coord_point_t dest;
	double speed;
	bool work;
  };

  class CalxCoordActionGraphBuild : public CalxAction {
   public:
	CalxCoordActionGraphBuild(CoordHandle *, CoordTranslator *,
						 GraphBuilder *, double);
	virtual ~CalxCoordActionGraphBuild();

	virtual void perform(SystemManager *);
	virtual void stop();

   private:
	CoordHandle *handle;
	CoordTranslator *translator;
	GraphBuilder *builder;
	double speed;
	TaskState state;
  };

  class CalxCoordActionGraphPreview : public CalxAction {
   public:
	CalxCoordActionGraphPreview(CalxVirtualPlaneDialog *, CoordTranslator *,
						 GraphBuilder *, double);
	virtual ~CalxCoordActionGraphPreview();

	virtual void perform(SystemManager *);
	virtual void stop();

   private:
	CalxVirtualPlaneDialog *dialog;
	CoordTranslator *translator;
	GraphBuilder *builder;
	double speed;
	TaskState state;
  };
}

#endif