@echo off
REM This file is autogenerated by /misc/gen-winbuild script
cl /EHsc /Zi /Gm /W0 /MD /I. /I ui /I %1\include /I %1\lib\vc_dll\mswu /Fecalxui.exe ui/CalxAboutDialog.cpp ui/CalxActionQueue.cpp ui/CalxApp.cpp ui/CalxConsoleWidget.cpp ui/CalxDebugConsole.cpp ui/CalxErrorHandler.cpp ui/CalxFrame.cpp ui/CalxPanel.cpp ui/coord/CalxCoordArcCtrl.cpp ui/coord/CalxCoordCtrl.cpp ui/coord/CalxCoordDialog.cpp ui/coord/CalxCoordFilter.cpp ui/coord/CalxCoordGraphCtrl.cpp ui/coord/CalxCoordLinearCtrl.cpp ui/coord/CalxCoordMiscCtrl.cpp ui/coord/CalxCoordOtherCtrl.cpp ui/coord/CalxCoordPanel.cpp ui/coord/CalxVirtualPlane.cpp ui/dev/CalxCOMSelectDialog.cpp ui/dev/CalxDevicePanel.cpp ui/dev/CalxInstrumentCtrl.cpp ui/dev/CalxMotorCtrl.cpp ui/task/CalxGcodeHandle.cpp ui/task/CalxGcodeLoader.cpp ui/task/CalxProgrammedTaskHandle.cpp ui/task/CalxTaskPanel.cpp ui/task/CalxTaskStepHandle.cpp  /link %1\lib\vc_dll\wxmsw31u.lib build\calx.lib /MACHINE:x86
copy %1\lib\vc_dll\wxmsw310u_vc_custom.dll build
move calxui.exe build
