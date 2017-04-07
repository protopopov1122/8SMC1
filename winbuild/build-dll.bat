@echo off
REM This file is autogenerated by /misc/gen-winbuild script
cl /Zi /W0 /EHsc /Gm /I . /MD /LD /Fecalx.dll device/Device.cpp device/DeviceManager.cpp cli/CLI.cpp cli/DevCLI.cpp ctrl-lib/ConfigManager.cpp ctrl-lib/ConfigValidator.cpp ctrl-lib/CoordController.cpp ctrl-lib/CoordHandle.cpp ctrl-lib/graph/AST.cpp ctrl-lib/graph/DefaultFunctions.cpp ctrl-lib/graph/FunctionEngine.cpp ctrl-lib/graph/FunctionLexer.cpp ctrl-lib/graph/FunctionParser.cpp ctrl-lib/InstrumentController.cpp ctrl-lib/logger.cpp ctrl-lib/misc/CircleGenerator.cpp ctrl-lib/misc/GCodeParser.cpp ctrl-lib/misc/GCodeWriter.cpp ctrl-lib/misc/GraphBuilder.cpp ctrl-lib/MotorController.cpp ctrl-lib/plane/CoordPlaneLinearizer.cpp ctrl-lib/plane/CoordPlaneLog.cpp ctrl-lib/plane/CoordPlaneMap.cpp ctrl-lib/plane/CoordPlaneStack.cpp ctrl-lib/plane/CoordPlaneValidator.cpp ctrl-lib/plane/VirtualCoordPlane.cpp ctrl-lib/SystemManager.cpp ctrl-lib/task/CoordTask.cpp ctrl-lib/task/CoordTaskWrapper.cpp ctrl-lib/task/GCodeCoordTask.cpp ctrl-lib/translator/BasicCoordTranslator.cpp ctrl-lib/translator/ComplexCoordTranslator.cpp ctrl-lib/translator/LinearCoordTranslator.cpp ctrl-lib/translator/LogarithmicCoordTranslator.cpp ctrl-lib/translator/PolarCoordTranslator.cpp  /link /MACHINE:X86
lib /OUT:calx.lib Device.obj DeviceManager.obj CLI.obj DevCLI.obj ConfigManager.obj ConfigValidator.obj CoordController.obj CoordHandle.obj AST.obj DefaultFunctions.obj FunctionEngine.obj FunctionLexer.obj FunctionParser.obj InstrumentController.obj logger.obj CircleGenerator.obj GCodeParser.obj GCodeWriter.obj GraphBuilder.obj MotorController.obj CoordPlaneLinearizer.obj CoordPlaneLog.obj CoordPlaneMap.obj CoordPlaneStack.obj CoordPlaneValidator.obj VirtualCoordPlane.obj SystemManager.obj CoordTask.obj CoordTaskWrapper.obj GCodeCoordTask.obj BasicCoordTranslator.obj ComplexCoordTranslator.obj LinearCoordTranslator.obj LogarithmicCoordTranslator.obj PolarCoordTranslator.obj 
move calx.dll build
move calx.lib build
