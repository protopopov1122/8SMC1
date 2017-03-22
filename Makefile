CC=i686-w64-mingw32-g++

OUTPUT=calx
UI=calxui
BUILD=./build
WXLIB=

TARGET=debug
LANG=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-attributes
DBG_debug=-O0
DBG_release=-O2
DBG=$(DBG_$(TARGET))
INCLUDES=-I.
EXTRA=
CFLAGS=$(LANG) $(DBG) $(INCLUDES) $(EXTRA)

LDFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic

8SMC1Device.o:
	$(CC) $(CFLAGS) -Ires -c device/8smc1/8SMC1Device.cpp

8SMC1DeviceManager.o:
	$(CC) $(CFLAGS) -Ires -c device/8smc1/8SMC1DeviceManager.cpp

dev_8smc1.dll: 8SMC1Device.o 8SMC1DeviceManager.o  $(OUTPUT).dll
	$(CC) $(CFLAGS) -shared -o $(BUILD)/dev_8smc1.dll 8SMC1Device.o 8SMC1DeviceManager.o  -Wl,-Bdynamic,--library-path=$(BUILD) -lUSMCDLL $(LDFLAGS) -Wl,--library-path=$(LIB) -lcalx -Wl,--out-implib,$(BUILD)/\dev_8smc1.a

Device.o:
	$(CC) $(CFLAGS) -c device/Device.cpp

DeviceManager.o:
	$(CC) $(CFLAGS) -c device/DeviceManager.cpp

Stub.o:
	$(CC) $(CFLAGS) -Ires -c misc/Stub.cpp

CoordController.o:
	$(CC) $(CFLAGS) -c ctrl-lib/CoordController.cpp

CoordHandle.o:
	$(CC) $(CFLAGS) -c ctrl-lib/CoordHandle.cpp

DeviceController.o:
	$(CC) $(CFLAGS) -c ctrl-lib/DeviceController.cpp

AST.o:
	$(CC) $(CFLAGS) -c ctrl-lib/graph/AST.cpp

DefaultFunctions.o:
	$(CC) $(CFLAGS) -c ctrl-lib/graph/DefaultFunctions.cpp

FunctionEngine.o:
	$(CC) $(CFLAGS) -c ctrl-lib/graph/FunctionEngine.cpp

FunctionLexer.o:
	$(CC) $(CFLAGS) -c ctrl-lib/graph/FunctionLexer.cpp

FunctionParser.o:
	$(CC) $(CFLAGS) -c ctrl-lib/graph/FunctionParser.cpp

InstrumentController.o:
	$(CC) $(CFLAGS) -c ctrl-lib/InstrumentController.cpp

CircleGenerator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/misc/CircleGenerator.cpp

GCodeParser.o:
	$(CC) $(CFLAGS) -c ctrl-lib/misc/GCodeParser.cpp

GraphBuilder.o:
	$(CC) $(CFLAGS) -c ctrl-lib/misc/GraphBuilder.cpp

CoordPlaneLog.o:
	$(CC) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneLog.cpp

CoordPlaneMap.o:
	$(CC) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneMap.cpp

CoordPlaneStack.o:
	$(CC) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneStack.cpp

CoordPlaneValidator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneValidator.cpp

VirtualCoordPlane.o:
	$(CC) $(CFLAGS) -c ctrl-lib/plane/VirtualCoordPlane.cpp

SystemManager.o:
	$(CC) $(CFLAGS) -c ctrl-lib/SystemManager.cpp

CoordTask.o:
	$(CC) $(CFLAGS) -c ctrl-lib/task/CoordTask.cpp

CoordTaskWrapper.o:
	$(CC) $(CFLAGS) -c ctrl-lib/task/CoordTaskWrapper.cpp

GCodeCoordTask.o:
	$(CC) $(CFLAGS) -c ctrl-lib/task/GCodeCoordTask.cpp

BasicCoordTranslator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/translator/BasicCoordTranslator.cpp

ComplexCoordTranslator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/translator/ComplexCoordTranslator.cpp

LinearCoordTranslator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/translator/LinearCoordTranslator.cpp

LogarithmicCoordTranslator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/translator/LogarithmicCoordTranslator.cpp

PolarCoordTranslator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/translator/PolarCoordTranslator.cpp

CLI.o:
	$(CC) $(CFLAGS) -c cli/CLI.cpp

DevCLI.o:
	$(CC) $(CFLAGS) -c cli/DevCLI.cpp

main.o:
	$(CC) $(CFLAGS) -c cli/main.cpp


CalxAboutDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxAboutDialog.cpp

CalxActionQueue.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxActionQueue.cpp

CalxApp.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxApp.cpp

CalxConsoleWidget.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxConsoleWidget.cpp

CalxDebugConsole.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxDebugConsole.cpp

CalxFrame.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxFrame.cpp

CalxPanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxPanel.cpp

CalxCoordArcCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordArcCtrl.cpp

CalxCoordCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordCtrl.cpp

CalxCoordDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordDialog.cpp

CalxCoordFilter.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordFilter.cpp

CalxCoordGraphCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordGraphCtrl.cpp

CalxCoordLinearCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordLinearCtrl.cpp

CalxCoordMiscCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordMiscCtrl.cpp

CalxCoordOtherCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordOtherCtrl.cpp

CalxCoordPanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordPanel.cpp

CalxCOMSelectDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxCOMSelectDialog.cpp

CalxDeviceCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxDeviceCtrl.cpp

CalxDevicePanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxDevicePanel.cpp

CalxGcodeHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxGcodeHandle.cpp

CalxGcodeLoader.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxGcodeLoader.cpp

CalxProgrammedTaskHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxProgrammedTaskHandle.cpp

CalxTaskPanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxTaskPanel.cpp

CalxTaskStepHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxTaskStepHandle.cpp

stub: Stub.o
	$(CC) -shared -o $(BUILD)/USMCDLL.dll Stub.o $(LDFLAGS)

$(OUTPUT).exe: $(OUTPUT).dll dev_8smc1.dll main.o
	@mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(OUTPUT).exe main.o  $(LDFLAGS) -Wl,--library-path=$(BUILD) -ldev_8smc1 -l$(OUTPUT)

$(OUTPUT).dll: CoordController.o CoordHandle.o DeviceController.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o InstrumentController.o CircleGenerator.o GCodeParser.o GraphBuilder.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o SystemManager.o CoordTask.o CoordTaskWrapper.o GCodeCoordTask.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o CLI.o DevCLI.o Device.o DeviceManager.o
	@mkdir -p $(BUILD)
	$(CC) -shared -o $(BUILD)/$(OUTPUT).dll CoordController.o CoordHandle.o DeviceController.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o InstrumentController.o CircleGenerator.o GCodeParser.o GraphBuilder.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o SystemManager.o CoordTask.o CoordTaskWrapper.o GCodeCoordTask.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o CLI.o DevCLI.o Device.o DeviceManager.o  $(LDFLAGS) -Wl,--out-implib,$(BUILD)/$(OUTPUT).a

$(UI).exe: CalxAboutDialog.o CalxActionQueue.o CalxApp.o CalxConsoleWidget.o CalxDebugConsole.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordFilter.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCOMSelectDialog.o CalxDeviceCtrl.o CalxDevicePanel.o CalxGcodeHandle.o CalxGcodeLoader.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxTaskStepHandle.o
	@mkdir -p $(BUILD)
ifneq ($(WXLIB),)
	cp $(WX)/$(WXLIB) $(BUILD)
endif
	$(CC) -o $(BUILD)/$(UI).exe CalxAboutDialog.o CalxActionQueue.o CalxApp.o CalxConsoleWidget.o CalxDebugConsole.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordFilter.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCOMSelectDialog.o CalxDeviceCtrl.o CalxDevicePanel.o CalxGcodeHandle.o CalxGcodeLoader.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxTaskStepHandle.o  $(LDFLAGS) -Wl,--library-path=\$(BUILD) -lcalx `$(WX)/wx-config --libs`

all: $(OUTPUT).dll dev_8smc1.dll $(OUTPUT).exe $(UI).exe copy


clean:
	@rm -f *.o
	@rm -rf $(BUILD)
	@mkdir -p $(BUILD)
	@rm -f *.obj
	@rm -f *.exe
	@rm -f *.lib
	@rm -f *.dll
	@rm -f *.exp
	@rm -f *.ilk
	@rm -f *.pdb
	@rm -f *.idb
	@rm -f *.manifest
	
copy:
	zip -r $(BUILD)/src.zip *.md Makefile device ctrl-lib ui winbuild misc cli tests COPYING COPYING.LESSER NOTICE
	cp COPYING $(BUILD)
	cp COPYING.LESSER $(BUILD)
	cp NOTICE $(BUILD)