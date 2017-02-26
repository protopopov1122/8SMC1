@echo off
cl /EHsc /Zi /Gm /W0 /I. /I %1\include /I %1\lib\vc_dll\mswud /Feui.exe ui/main.cpp  /link %1\lib\vc_dll\wxmsw31ud.lib 8smc1.lib /MACHINE:x86
copy %1\lib\vc_dll\wxmsw310ud_vc_custom.dll .
