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

#include "ui/script/CalXScriptEngine.h"
#include "ui/task/CalxTaskList.h"
#include "ui/task/CalxGCodeTask.h"
#include "ui/task/CalxLinearTask.h"
#include <sstream>

namespace CalXUI {
	
  static std::shared_ptr<ComplexCoordTranslator> getDefaultTranslator() {
	  ConfigurationCatalogue &conf =
		    wxGetApp().getSystemManager().getConfiguration();
      coord_point_t trans_offset = {
        static_cast<double>(conf.getEntry(CalxConfiguration::Coordinates)
                                ->getReal(CalxCoordConfiguration::OffsetX, 0.0)),
        static_cast<double>(conf.getEntry(CalxConfiguration::Coordinates)
                                ->getReal(CalxCoordConfiguration::OffsetY, 0.0))
      };
      coord_scale_t trans_scale = {
        static_cast<double>(conf.getEntry(CalxConfiguration::Coordinates)
                                ->getReal(CalxCoordConfiguration::ScaleX, 1.0)),
        static_cast<double>(conf.getEntry(CalxConfiguration::Coordinates)
                                ->getReal(CalxCoordConfiguration::ScaleY, 1.0))
      };
      coord_point_t unit_offset = wxGetApp().getUnitProcessor().getUnitOffset();
      coord_scale_t unit_scale = { wxGetApp().getUnitProcessor().getUnitScale(),
                                  wxGetApp().getUnitProcessor().getUnitScale() };
      std::shared_ptr<LinearCoordTranslator> unit_trans =
          std::make_shared<LinearCoordTranslator>(unit_offset, unit_scale);
      std::shared_ptr<LinearCoordTranslator> trans =
          std::make_shared<LinearCoordTranslator>(trans_offset, trans_scale,
                                                  unit_trans);
      auto translator = std::make_shared<ComplexCoordTranslator>(unit_trans);
      translator->add(trans);
	  return translator;
  }

  class CalxGCodeFileTaskBuilder : public CalxTaskFactory {
   public:
    CalxGCodeFileTaskBuilder(const std::string &path) : path(path) {
      this->translator = getDefaultTranslator();
    }
  
    CalxTaskHandle *newTask(wxWindow *win) override {
      CalxGcodeHandle *handle = nullptr;
      std::fstream is(this->path);
      if (is.good()) {
        handle = new CalxGcodeHandle(win, wxID_ANY, this->path, &is,
                                    this->translator);
      }
      is.close();
      return handle;
    }
   private:
    std::string path;
    std::shared_ptr<ComplexCoordTranslator> translator;
  };
  
  class CalxGCodeStringTaskBuilder : public CalxTaskFactory {
   public:
    CalxGCodeStringTaskBuilder(const std::string &title, const std::string &gcode) : title(title), gcode(gcode) {
      this->translator = getDefaultTranslator();
    }
  
    CalxTaskHandle *newTask(wxWindow *win) override {
      CalxGcodeHandle *handle = nullptr;
      std::stringstream is(this->gcode);
      handle = new CalxGcodeHandle(win, wxID_ANY, this->title, &is,
								  this->translator);
      return handle;
    }
   private:
	std::string title;
    std::string gcode;
    std::shared_ptr<ComplexCoordTranslator> translator;
  };
  
  class CalxLinearTaskBuilder : public CalxTaskFactory {
   public:
    CalxLinearTaskBuilder(std::size_t id, CalxLinearTaskParameters &prms) : id(id), prms(prms) {
	  this->translator = getDefaultTranslator();
	}
	
    CalxTaskHandle *newTask(wxWindow *win) override {
	  return new CalxLinearTaskHandle(win, wxID_ANY, id, &prms);
	}
   private:
    std::size_t id;
    CalxLinearTaskParameters prms;
    std::shared_ptr<ComplexCoordTranslator> translator;
  };
  
  CalXAppScriptTasks::CalXAppScriptTasks(CalxApp &app)
    : app(app) {}
  
  std::size_t CalXAppScriptTasks::getTaskCount() {
    return this->app.getMainFrame()->getTaskList()->getTaskCount();
  }

  void CalXAppScriptTasks::removeTask(std::size_t idx) {
    this->app.getMainFrame()->getTaskList()->removeTask(idx);
  }

  void CalXAppScriptTasks::newGCodeFile(const std::string &title, const std::string &path) {
    this->app.getMainFrame()->getTaskList()->attachTask(title, std::make_shared<CalxGCodeFileTaskBuilder>(path));
  }
  
  void CalXAppScriptTasks::newGCode(const std::string &title, const std::string &path) {
    this->app.getMainFrame()->getTaskList()->attachTask(title, std::make_shared<CalxGCodeStringTaskBuilder>(title, path));
  }
  
  void CalXAppScriptTasks::newLinear(const std::string &title, CalxLinearTaskParameters &prms) {
	this->app.getMainFrame()->getTaskList()->attachTask(title, std::make_shared<CalxLinearTaskBuilder>(0, prms));
  }
}