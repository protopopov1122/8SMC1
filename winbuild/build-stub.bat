@echo off
REM This file is autogenerated by /misc/gen-winbuild script
cl /W0 /EHsc /I res /Zi /Gm /LD /FeUSMCDLL.dll device/standart/Stub.cpp /link /MACHINE:X86
move USMCDLL.dll build
move USMCDLL.lib build
