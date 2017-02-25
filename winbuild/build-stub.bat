@echo off
cl /W0 /EHsc /I res /LD /FeUSMCDLL.dll misc/Stub.cpp /link /MACHINE:X86
