/* This file contains USMC procedure stubs to test the rest of code without controller */

#include "USMCDLL.h"
#include <stdio.h>
#include <iostream>

int POS[] = {0, 0};

char SER01[] = "01";
char SER02[] = "02";

char *Serials[] = {
	SER01, SER02
};

DWORD USMC_Init(USMC_Devices &devs) {
	std::cout << "Init devices" << std::endl;
	devs.NOD = 2;
	devs.Serial = Serials;
	devs.Version = Serials;
	return FALSE;
}

DWORD USMC_Close() {
	std::cout << "Close devices" << std::endl;
	return FALSE;
}

DWORD USMC_GetState(DWORD dev, USMC_State &state) {
	state.CurPos = POS[dev];
	return FALSE;
}

DWORD USMC_SetCurrentPosition(DWORD dev, int pos) {
	return FALSE;
}

DWORD USMC_Start(DWORD dev, int DestPos, float &Speed, USMC_StartParameters &Str) {
/*	std::cout << "Move #" << dev << " to " << DestPos
			<< " with speed " << Speed
			<< "; sync: " << (Str.WSyncIN ? "enabled" : "disabled") << std::endl;*/
	POS[dev] = DestPos;
	return FALSE;
}

DWORD USMC_Stop(DWORD dev) {
	return FALSE;
}

DWORD USMC_GetMode(DWORD dev, USMC_Mode &mod) {
	return FALSE;
}

DWORD USMC_SetMode(DWORD dev, USMC_Mode &Str) {
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

void USMC_GetLastErr(char *str, size_t len) {
}

DWORD USMC_GetEncoderState(DWORD dev, USMC_EncoderState &encState) {
	return FALSE;
}

void USMC_GetDllVersion( DWORD &dwHighVersion, DWORD &dwLowVersion) {
}

DWORD USMC_RestoreCurPos(DWORD dev) {
	return FALSE;
}
