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

/* This file contains USMC procedure stubs to test the rest of code without
 * controller */

#include "USMCDLL.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int LENGTH = 150000;
struct {
	int start;
	int finish;
	int position;
	int power;
} MOTORS[2] = { { -LENGTH, LENGTH, 0, 0 }, { -LENGTH, LENGTH, 0, 0 } };

char SER01[] = "01";
char SER02[] = "02";

char *Serials[] = { SER01, SER02 };

DWORD USMC_Init(USMC_Devices &devs) {
	time_t t;
	srand((unsigned) time(&t));
	int r1, r2;
	r1 = (rand() % (MOTORS[0].finish * 2)) - MOTORS[0].finish;
	r2 = (rand() % (MOTORS[1].finish * 2)) - MOTORS[1].finish;
	MOTORS[0].start -= r1;
	MOTORS[0].finish -= r1;
	MOTORS[0].position -= r1;
	MOTORS[1].start -= r2;
	MOTORS[1].finish -= r2;
	MOTORS[1].position -= r2;
	devs.NOD = 2;
	devs.Serial = Serials;
	devs.Version = Serials;
	return FALSE;
}

DWORD USMC_Close() {
	return FALSE;
}

DWORD USMC_GetState(DWORD dev, USMC_State &state) {
	state.CurPos = MOTORS[dev].position;
	state.Trailer1 = MOTORS[dev].position <= MOTORS[dev].start;
	state.Trailer2 = MOTORS[dev].position >= MOTORS[dev].finish;
	state.Power = MOTORS[dev].power > 0;
	state.FullPower = MOTORS[dev].power > 1;
	return FALSE;
}

DWORD USMC_SetCurrentPosition(DWORD dev, int pos) {
	return FALSE;
}

DWORD USMC_Start(DWORD dev, int DestPos, float &Speed,
                 USMC_StartParameters &Str) {
	if (MOTORS[dev].power == 0) {
		return FALSE;
	}
	MOTORS[dev].power = 2;
	MOTORS[dev].position = DestPos;
	return FALSE;
}

DWORD USMC_Stop(DWORD dev) {
	return FALSE;
}

DWORD USMC_GetMode(DWORD dev, USMC_Mode &mod) {
	return FALSE;
}

DWORD USMC_SetMode(DWORD dev, USMC_Mode &Str) {
	MOTORS[dev].power = Str.ResetD ? 0 : 1;
	return FALSE;
}

DWORD USMC_GetParameters(DWORD dev, USMC_Parameters &prms) {
	return FALSE;
}

DWORD USMC_SetParameters(DWORD dev, USMC_Parameters &prms) {
	return FALSE;
}

DWORD USMC_GetStartParameters(DWORD dev, USMC_StartParameters &prms) {
	return FALSE;
}

DWORD USMC_SaveParametersToFlash(DWORD dev) {
	return FALSE;
}

void USMC_GetLastErr(char *str, size_t len) {}

DWORD USMC_GetEncoderState(DWORD dev, USMC_EncoderState &encState) {
	return FALSE;
}

void USMC_GetDllVersion(DWORD &dwHighVersion, DWORD &dwLowVersion) {}

DWORD USMC_RestoreCurPos(DWORD dev) {
	return FALSE;
}
