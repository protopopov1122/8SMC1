@echo off
cl /EHsc /Zi /Gm /std:c++latest /W0 /I. /I res /I %1\include /I %1\lib\vc_dll\mswud /Feui.exe ui/app.cpp ui/mainFrame.cpp  /link %1\lib\vc_dll\wxmsw31u.lib USMCDLL.lib 8smc1.lib /MACHINE:x86
copy %1\lib\vc_dll\wxmsw310u_vc_custom.dll .
