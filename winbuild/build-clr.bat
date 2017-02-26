@echo off
cl /clr /Zi /Gm /W0 /I . /I res /Fe8smc1.exe device/Device.cpp device/DeviceManager.cpp device/MotorTask.cpp ctrl-lib/CoordController.cpp ctrl-lib/DeviceController.cpp ctrl-lib/graph/AST.cpp ctrl-lib/graph/DefaultFunctions.cpp ctrl-lib/graph/FunctionEngine.cpp ctrl-lib/graph/FunctionLexer.cpp ctrl-lib/graph/FunctionParser.cpp ctrl-lib/misc/CircleGenerator.cpp ctrl-lib/misc/GCodeParser.cpp ctrl-lib/misc/GraphBuilder.cpp ctrl-lib/plane/CoordPlaneLog.cpp ctrl-lib/plane/CoordPlaneMap.cpp ctrl-lib/plane/CoordPlaneStack.cpp ctrl-lib/plane/CoordPlaneValidator.cpp ctrl-lib/plane/VirtualCoordPlane.cpp ctrl-lib/SystemManager.cpp ctrl-lib/task/CoordTask.cpp ctrl-lib/task/CoordTaskWrapper.cpp ctrl-lib/translator/BasicCoordTranslator.cpp ctrl-lib/translator/LinearCoordTranslator.cpp ctrl-lib/translator/LogarithmicCoordTranslator.cpp ctrl-lib/translator/PolarCoordTranslator.cpp cli/CLI.cpp cli/DevCLI.cpp cli/main.cpp  /link USMCDLL.lib
