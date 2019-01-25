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

namespace CalXUI {

  class CalxGCodeTaskBuilder : public CalxTaskFactory {
   public:
    CalxGCodeTaskBuilder(const std::string &path) : path(path) {
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
      this->translator = std::make_shared<ComplexCoordTranslator>(unit_trans);
      this->translator->add(trans);
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
  
  CalXAppScriptTasks::CalXAppScriptTasks(CalxApp &app)
    : app(app) {}
  
  std::size_t CalXAppScriptTasks::getTaskCount() {
    return this->app.getMainFrame()->getTaskList()->getTaskCount();
  }

  void CalXAppScriptTasks::removeTask(std::size_t idx) {
    this->app.getMainFrame()->getTaskList()->removeTask(idx);
  }

  void CalXAppScriptTasks::newGCodeFile(const std::string &title, const std::string &path) {
    this->app.getMainFrame()->getTaskList()->attachTask(title, std::make_shared<CalxGCodeTaskBuilder>(path));
  }
}