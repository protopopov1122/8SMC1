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

#ifndef CALX_CTRL_LIB_RESOURCE_H_
#define CALX_CTRL_LIB_RESOURCE_H_

#include "calx/ctrl-lib/CtrlCore.h"

namespace CalX {

	class UsableResource {
	 public:
		UsableResource();
		virtual ~UsableResource() = default;
		virtual void use();
		virtual void unuse();

	 protected:
		uint8_t getResourceCounter() const;
		bool isResourceUsed() const;

	 private:
		uint8_t counter;
	};

	class SessionableResource {
	 public:
		SessionableResource();
		virtual ~SessionableResource() = default;
		virtual ErrorCode open_session();
		virtual ErrorCode close_session();

	 protected:
		bool isSessionOpened() const;

	 private:
		bool session_state;
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

	class ResourceSession {
	 public:
		ResourceSession(SessionableResource &);
		ResourceSession(SessionableResource *);
		~ResourceSession();
		ErrorCode getStatus() const;
		ErrorCode close();

	 private:
		SessionableResource *resource;
		ErrorCode errcode;
	};
}  // namespace CalX

#endif