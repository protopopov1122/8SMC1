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


#include "ctrl-lib/gcode/GCodeInterpreter.h"

namespace CalX {

#define TO_REAL(prm) (prm.type == GCodeParameterType::Real ? prm.value.real : static_cast<double>(prm.value.integer))

	ErrorCode GCodeInterpreter::execute(GCodeStream *input, CoordPlane *plane, CoordTranslator *trans,
		ConfigManager *config, float speed, TaskState *state) {
		
		input->reset();
		
		coord_point_t troffset = {0, 0};
		coord_scale_t trscale = {1, 1};
		LinearCoordTranslator unit_trans(troffset, trscale, trans);
		LinearCoordTranslator translator(troffset, trscale, &unit_trans);
		
		motor_point_t offset = translator.get(0, 0);
		coord_point_t last = {0, 0};
		int invert = 1;
		if (translator.get(1, 0).x < offset.x) {
			invert *= -1;
		}
		if (translator.get(0, 1).y < offset.y) {
			invert *= -1;
		}
        const int_conf_t CHORD_COUNT = config->getEntry("core")->getInt("chord_count", 100);
		ErrorCode errcode = ErrorCode::NoError;
		while (input->hasNext() && state->work && errcode == ErrorCode::NoError) {
			GCodeCmd cmd = input->next();
			switch (cmd.getOperation()) {
				case GCodeOperation::RapidMove: {
					coord_point_t dest = translator.get(plane->getPosition());
					if (cmd.hasArgument('X')) {
						GCodeParameter prm = cmd.getArgument('X');
						dest.x = TO_REAL(prm);
					}
					if (cmd.hasArgument('Y')) {
						GCodeParameter prm = cmd.getArgument('Y');
						dest.y = TO_REAL(prm);
					}
					errcode = plane->move(translator.get(dest.x, dest.y), speed, 8, false);
				} break;
				case GCodeOperation::LinearMove: {
					coord_point_t dest = translator.get(plane->getPosition());
					if (cmd.hasArgument('X')) {
						GCodeParameter prm = cmd.getArgument('X');
						dest.x = TO_REAL(prm);
					}
					if (cmd.hasArgument('Y')) {
						GCodeParameter prm = cmd.getArgument('Y');
						dest.y = TO_REAL(prm);
					}
					errcode = plane->move(translator.get(dest.x, dest.y), speed, 8, true);
				} break;
				case GCodeOperation::ClockwiseArc: if (cmd.hasArgument('I') || cmd.hasArgument('J')) {
					coord_point_t dest = translator.get(plane->getPosition());
					coord_point_t cen = {0, 0};
					if (cmd.hasArgument('X')) {
						GCodeParameter prm = cmd.getArgument('X');
						dest.x = TO_REAL(prm);
					}
					if (cmd.hasArgument('Y')) {
						GCodeParameter prm = cmd.getArgument('Y');
						dest.y = TO_REAL(prm);
					}
					if (cmd.hasArgument('I')) {
						GCodeParameter prm = cmd.getArgument('I');
						cen.x = TO_REAL(prm);
					}
					if (cmd.hasArgument('J')) {
						GCodeParameter prm = cmd.getArgument('J');
						cen.y = TO_REAL(prm);
					}
					motor_point_t current = plane->getPosition();
					motor_point_t mdest = translator.get(dest.x, dest.y); 
					motor_point_t mcen = translator.get(cen.x, cen.y);
					mcen.x -= offset.x;
					mcen.y -= offset.y;
					mcen.x += current.x;
					mcen.y += current.y;
					errcode = plane->arc(mdest, mcen,
						CHORD_COUNT, speed, 8, invert == 1);
				} break;
				case GCodeOperation::CounterClockwiseArc: if (cmd.hasArgument('I') || cmd.hasArgument('J')) {
					coord_point_t dest = translator.get(plane->getPosition());
					coord_point_t cen = {0, 0};
					if (cmd.hasArgument('X')) {
						GCodeParameter prm = cmd.getArgument('X');
						dest.x = TO_REAL(prm);
					}
					if (cmd.hasArgument('Y')) {
						GCodeParameter prm = cmd.getArgument('Y');
						dest.y = TO_REAL(prm);
					}
					if (cmd.hasArgument('I')) {
						GCodeParameter prm = cmd.getArgument('I');
						cen.x = TO_REAL(prm);
					}
					if (cmd.hasArgument('J')) {
						GCodeParameter prm = cmd.getArgument('J');
						cen.y = TO_REAL(prm);
					}
					motor_point_t current = plane->getPosition();
					motor_point_t mdest = translator.get(dest.x, dest.y); 
					motor_point_t mcen = translator.get(cen.x, cen.y);
					mcen.x -= offset.x;
					mcen.y -= offset.y;
					mcen.x += current.x;
					mcen.y += current.y;
					errcode = plane->arc(mdest, mcen,
						CHORD_COUNT, speed, 8, invert != 1);
				} break;
				case GCodeOperation::SwitchInches: {
					coord_scale_t scale = {25.4, 25.4};
					unit_trans.setScale(scale);
				} break;
				case GCodeOperation::SwitchMillimeters: {
					coord_scale_t scale = {1.0, 1.0};
					unit_trans.setScale(scale);
				} break;
				case GCodeOperation::Home: {
					coord_point_t dest = {0, 0};
					errcode = plane->move(translator.get(dest.x, dest.y), speed, 8, false);
				} break;
			}
		}
		return errcode;
	}
}