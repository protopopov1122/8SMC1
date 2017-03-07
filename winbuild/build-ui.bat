@echo off
cl /EHsc /Zi /Gm /std:c++latest /W0 /I. /I res /I ui /I %1\include /I %1\lib\vc_dll\mswud /Feui.exe ui/CalxActionQueue.cpp ui/CalxApp.cpp ui/CalxConsoleWidget.cpp ui/CalxFrame.cpp ui/CalxPanel.cpp ui/coord/CalxCoordArcCtrl.cpp ui/coord/CalxCoordCtrl.cpp ui/coord/CalxCoordDialog.cpp ui/coord/CalxCoordGraphCtrl.cpp ui/coord/CalxCoordLinearCtrl.cpp ui/coord/CalxCoordMiscCtrl.cpp ui/coord/CalxCoordOtherCtrl.cpp ui/coord/CalxCoordPanel.cpp ui/coord/CalxCoordTranslator.cpp ui/dev/CalxDeviceCtrl.cpp ui/dev/CalxDevicePanel.cpp ui/task/CalxGcodeHandle.cpp ui/task/CalxGcodeLoader.cpp ui/task/CalxTaskPanel.cpp  /link %1\lib\vc_dll\wxmsw31u.lib build\USMCDLL.lib build\8smc1.lib /MACHINE:x86
copy %1\lib\vc_dll\wxmsw310u_vc_custom.dll build
move ui.exe build
