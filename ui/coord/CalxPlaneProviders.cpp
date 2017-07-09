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

#include "ui/coord/CalxPlaneProviders.h"

namespace CalXUI {
  class CalxCoordPlaneAddProvider : public RequestProvider {
   public:
	CalxCoordPlaneAddProvider(CalxCoordPanel *panel)
		: RequestProvider::RequestProvider("plane.create") {
	  this->panel = panel;
	}

	virtual ~CalxCoordPlaneAddProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 3)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
	  device_id_t dev1 =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))
			  ->getValue();
	  device_id_t dev2 =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 1))
			  ->getValue();
	  device_id_t instr =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 2))
			  ->getValue();
	  CoordHandle *handle = sysman->createCoord(dev1, dev2, instr);
	  if (handle == nullptr) {
		return false;
	  }
	  bool ready = false;
	  panel->updateList(handle, &ready);
	  while (!ready) {
		wxThread::Yield();
	  }

	  return true;
	}

   private:
	CalxCoordPanel *panel;
  };

  class CalxCoordPlaneMeasureProvider : public RequestProvider {
   public:
	CalxCoordPlaneMeasureProvider(CalxCoordPanel *panel)
		: RequestProvider::RequestProvider("plane.measure") {
	  this->panel = panel;
	}

	virtual ~CalxCoordPlaneMeasureProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 2)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
	  device_id_t plid =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))
			  ->getValue();
	  device_id_t tr =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 1))
			  ->getValue() %
		  2;
	  TrailerId trailer = tr == 1 ? TrailerId::Trailer1 : TrailerId::Trailer2;
	  if (sysman->getCoord((size_t) plid) != nullptr) {
		CalxCoordCtrl *ctrl = panel->getCoordCtrl((size_t) plid);
		bool ready = false;
		ctrl->getController()->measure(trailer, &ready);
		while (!ready) {
		  wxThread::Yield();
		}
		return true;
	  } else {
		return false;
	  }
	}

   private:
	CalxCoordPanel *panel;
  };

  class CalxCoordPlanePositionProvider : public RequestProvider {
   public:
	CalxCoordPlanePositionProvider(CalxCoordPanel *panel)
		: RequestProvider::RequestProvider("plane.position") {
	  this->panel = panel;
	}

	virtual ~CalxCoordPlanePositionProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 3)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Real)
	  PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Real)
	  device_id_t plid =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))
			  ->getValue();
	  double x = ((RealConfigValue *) PROVIDER_ARG(req, 1))->getValue();
	  double y = ((RealConfigValue *) PROVIDER_ARG(req, 2))->getValue();
	  if (sysman->getCoord((size_t) plid) != nullptr) {
		CalxCoordCtrl *ctrl = panel->getCoordCtrl((size_t) plid);
		ConfigManager *config =
			wxGetApp().getSystemManager()->getConfiguration();
		coord_point_t dest = { x, y };
		int_conf_t mspeed = config->getEntry("core")->getInt("maxspeed", 125);
		double speed = ((double) mspeed) / wxGetApp().getSpeedScale();
		bool ready = false;
		ctrl->getController()->move(dest, speed, &ready);
		while (!ready) {
		  wxThread::Yield();
		}
		return true;
	  } else {
		return false;
	  }
	}

   private:
	CalxCoordPanel *panel;
  };

  class CalxCoordPlanePositionAbsProvider : public RequestProvider {
   public:
	CalxCoordPlanePositionAbsProvider(CalxCoordPanel *panel)
		: RequestProvider::RequestProvider("plane.position.abs") {
	  this->panel = panel;
	}

	virtual ~CalxCoordPlanePositionAbsProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 3)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 1, ConfigValueType::Integer)
	  PROVIDER_ARG_TYPE(req, 2, ConfigValueType::Integer)
	  device_id_t plid =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))
			  ->getValue();
	  motor_coord_t x =
		  (motor_coord_t)((IntegerConfigValue *) PROVIDER_ARG(req, 1))
			  ->getValue();
	  motor_coord_t y =
		  (motor_coord_t)((IntegerConfigValue *) PROVIDER_ARG(req, 2))
			  ->getValue();
	  motor_point_t mdest = { x, y };
	  if (sysman->getCoord((size_t) plid) != nullptr) {
		CalxCoordCtrl *ctrl = panel->getCoordCtrl((size_t) plid);
		ConfigManager *config =
			wxGetApp().getSystemManager()->getConfiguration();
		int_conf_t mspeed = config->getEntry("core")->getInt("maxspeed", 125);
		coord_point_t dest = { mdest.x / wxGetApp().getUnitScale(),
							   mdest.y / wxGetApp().getUnitScale() };
		double speed = ((double) mspeed) / wxGetApp().getSpeedScale();
		bool ready = false;
		ctrl->getController()->move(dest, speed, false, false, &ready);
		while (!ready) {
		  wxThread::Yield();
		}
		return true;
	  } else {
		return false;
	  }
	}

   private:
	CalxCoordPanel *panel;
  };

  class CalxCoordPlaneCenterProvider : public RequestProvider {
   public:
	CalxCoordPlaneCenterProvider(CalxCoordPanel *panel)
		: RequestProvider::RequestProvider("plane.position.center") {
	  this->panel = panel;
	}

	virtual ~CalxCoordPlaneCenterProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 1)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  device_id_t plid =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))
			  ->getValue();
	  if (sysman->getCoord((size_t) plid) != nullptr) {
		CalxCoordCtrl *ctrl = panel->getCoordCtrl((size_t) plid);
		motor_point_t offset =
			ctrl->getController()->getHandle()->getPosition();
		motor_scale_t scale = ctrl->getController()->getMapFilter()->getScale();
		offset.x +=
			ctrl->getController()->getMapFilter()->getOffset().x / scale.x;
		offset.y +=
			ctrl->getController()->getMapFilter()->getOffset().y / scale.y;
		ctrl->setOffset(offset);
		return true;
	  } else {
		return false;
	  }
	}

   private:
	CalxCoordPanel *panel;
  };

  class CalxCoordPlaneInvertProvider : public RequestProvider {
   public:
	CalxCoordPlaneInvertProvider(CalxCoordPanel *panel)
		: RequestProvider::RequestProvider("plane.axis.invert") {
	  this->panel = panel;
	}

	virtual ~CalxCoordPlaneInvertProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 1)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  device_id_t plid =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))
			  ->getValue();
	  if (sysman->getCoord((size_t) plid) != nullptr) {
		CalxCoordCtrl *ctrl = panel->getCoordCtrl((size_t) plid);
		motor_scale_t scale = ctrl->getController()->getMapFilter()->getScale();
		scale.x *= -1;
		scale.y *= -1;
		ctrl->setScale(scale);
		return true;
	  } else {
		return false;
	  }
	}

   private:
	CalxCoordPanel *panel;
  };

  class CalxCoordPlaneWatcherProvider : public RequestProvider {
   public:
	CalxCoordPlaneWatcherProvider(CalxCoordPanel *panel)
		: RequestProvider::RequestProvider("plane.watcher") {
	  this->panel = panel;
	}

	virtual ~CalxCoordPlaneWatcherProvider() {}

	virtual bool execute(Request *req, SystemManager *sysman) {
	  PROVIDER_ARGC(req, 1)
	  PROVIDER_ARG_TYPE(req, 0, ConfigValueType::Integer)
	  device_id_t plid =
		  (device_id_t)((IntegerConfigValue *) PROVIDER_ARG(req, 0))
			  ->getValue();
	  if (sysman->getCoord((size_t) plid) != nullptr) {
		CalxCoordCtrl *ctrl = panel->getCoordCtrl((size_t) plid);
		wxThreadEvent evt(wxEVT_COORD_CTRL_WATCHER);
		wxPostEvent(ctrl, evt);
		return true;
	  } else {
		return false;
	  }
	}

   private:
	CalxCoordPanel *panel;
  };

  void calx_panel_init_resolvers(CalxCoordPanel *panel) {
	CalxApp &app = wxGetApp();
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxCoordPlaneAddProvider(panel));
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxCoordPlaneMeasureProvider(panel));
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxCoordPlanePositionProvider(panel));
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxCoordPlanePositionAbsProvider(panel));
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxCoordPlaneCenterProvider(panel));
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxCoordPlaneInvertProvider(panel));
	app.getSystemManager()->getRequestResolver()->registerProvider(
		new CalxCoordPlaneWatcherProvider(panel));
  }
}