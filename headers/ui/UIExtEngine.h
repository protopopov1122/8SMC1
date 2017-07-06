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

#ifndef CALX_UI_CALX_UI_EXT_ENGINE_H_
#define CALX_UI_CALX_UI_EXT_ENGINE_H_

#include "ctrl-lib/SystemManager.h"

using namespace CalX;

namespace CalXUI {

  class CalxApp;  // Forward referencing

  class UIExtEngine : public ExtEngine {
   public:
	UIExtEngine() : ExtEngine::ExtEngine() {}
	virtual ~UIExtEngine() {}
	virtual bool uiInit(CalxApp &) = 0;
  };

  typedef UIExtEngine *(*UIExtEngine_getter)();
  extern "C" LIBEXPORT UIExtEngine *getUIExtEngine();
}

#endif
