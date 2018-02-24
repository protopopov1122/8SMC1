/*
        Copyright (c) 2017 Jevgenijs Protopopovs

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

#ifndef CALX_CTRL_LIB_RESOURCE_H_
#define CALX_CTRL_LIB_RESOURCE_H_

#include "ctrl-lib/CtrlCore.h"

namespace CalX {

  class UsableResource {
   public:
    UsableResource();
    virtual ~UsableResource() = default;
    virtual void use();
    virtual void unuse();
   protected:
    uint8_t getResourceCounter();
    bool isResourceUsed();
   private:
    uint8_t counter;
  };

  class ResourceUse {
   public:
    ResourceUse(UsableResource &);
    ResourceUse(UsableResource * = nullptr);
    ~ResourceUse();
    UsableResource *swap(UsableResource &);
    UsableResource *swap(UsableResource *);
   private:
    UsableResource *resource;
  };
}

#endif