@echo off
REM This file is autogenerated by /misc/gen-winbuild script
cl /W0 /EHsc /Zi /Gm /I . /I res /Fecalx.exe device/8smc1/8SMC1Device.cpp device/8smc1/8SMC1DeviceManager.cpp device/Device.cpp device/DeviceManager.cpp ctrl-lib/CoordController.cpp ctrl-lib/CoordHandle.cpp ctrl-lib/DeviceController.cpp ctrl-lib/graph/AST.cpp ctrl-lib/graph/DefaultFunctions.cpp ctrl-lib/graph/FunctionEngine.cpp ctrl-lib/graph/FunctionLexer.cpp ctrl-lib/graph/FunctionParser.cpp ctrl-lib/misc/CircleGenerator.cpp ctrl-lib/misc/GCodeParser.cpp ctrl-lib/misc/GraphBuilder.cpp ctrl-lib/plane/CoordPlaneLog.cpp ctrl-lib/plane/CoordPlaneMap.cpp ctrl-lib/plane/CoordPlaneStack.cpp ctrl-lib/plane/CoordPlaneValidator.cpp ctrl-lib/plane/VirtualCoordPlane.cpp ctrl-lib/SystemManager.cpp ctrl-lib/task/CoordTask.cpp ctrl-lib/task/CoordTaskWrapper.cpp ctrl-lib/translator/BasicCoordTranslator.cpp ctrl-lib/translator/ComplexCoordTranslator.cpp ctrl-lib/translator/LinearCoordTranslator.cpp ctrl-lib/translator/LogarithmicCoordTranslator.cpp ctrl-lib/translator/PolarCoordTranslator.cpp cli/CLI.cpp cli/DevCLI.cpp cli/main.cpp  /link /MACHINE:X86 build\USMCDLL.lib
move calx.exe build
