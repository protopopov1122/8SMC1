/* This file contains USMC procedure stubs to test the rest of code without controller */

#include "USMCDLL.h"
#include <stdio.h>
#include <iostream>


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
	std::cout << "Get #" << dev << " state" << std::endl;
	return FALSE;
}

DWORD USMC_SetCurrentPosition(DWORD dev, int pos) {
	std::cout << "Set #" << dev << " position to " << pos << std::endl;
	return FALSE;
}

DWORD USMC_Start(DWORD dev, int DestPos, float &Speed, USMC_StartParameters &Str) {
	std::cout << "Move #" << dev << " to " << DestPos
			<< " with speed " << Speed
			<< "; sync: " << (Str.WSyncIN ? "enabled" : "disabled") << std::endl;
	return FALSE;
}

DWORD USMC_Stop(DWORD dev) {
	std::cout << "Stop #" << dev << std::endl;
	return FALSE;
}

DWORD USMC_GetMode(DWORD dev, USMC_Mode &mod) {
	std::cout << "Get #" << dev << " mode" << std::endl;
	return FALSE;
}

DWORD USMC_SetMode(DWORD dev, USMC_Mode &Str) {
	std::cout << "Set #" << dev << " mode" << std::endl;
	return FALSE;
}

DWORD USMC_GetParameters(DWORD dev, USMC_Parameters &prms) {
	std::cout << "Get #" << dev << " parameters" << std::endl;
	return FALSE;
}

DWORD USMC_SetParameters(DWORD dev, USMC_Parameters &prms) {
	std::cout << "Set #" << dev << " parameters" << std::endl;
	return FALSE;
}

DWORD USMC_GetStartParameters(DWORD dev, USMC_StartParameters &prms) {
	std::cout << "Get #" << dev << " start parameters" << std::endl;
	return FALSE;
}

DWORD USMC_SaveParametersToFlash(DWORD dev) {
	std::cout << "Save #" << dev << " parameters to flash" << std::endl;
	return FALSE;
}

void USMC_GetLastErr(char *str, size_t len) {
	std::cout << "Get last error" << std::endl;
}

DWORD USMC_GetEncoderState(DWORD dev, USMC_EncoderState &encState) {
	std::cout << "Get #" << dev << " encoder state" << std::endl;
	return FALSE;
}

void USMC_GetDllVersion( DWORD &dwHighVersion, DWORD &dwLowVersion) {
	// Do nothing
}

DWORD USMC_RestoreCurPos(DWORD dev) {
	// Also
	return FALSE;
}
