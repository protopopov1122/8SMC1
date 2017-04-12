CC=i686-w64-mingw32-g++
WINDRES=i686-w64-mingw32-windres
MSGFMT := $(shell command -v msgfmt 2> /dev/null)

CONVERT := $(shell command -v convert 2> /dev/null)
ifdef CONVERT
ifdef WINDRES
ICON=calxui.res
endif
endif

OUTPUT=calx
UI=calxui
BUILD=./build
WXLIB=

TARGET=debug
LANG=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-attributes
DBG_debug=-O0 -DLOGGING
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

NL300Instrument.o:
	$(CC) $(CFLAGS) -Ires -c device/8smc1/NL300Instrument.cpp

dev_8smc1.dll: 8SMC1Device.o 8SMC1DeviceManager.o NL300Instrument.o  $(OUTPUT).dll
	$(CC) $(CFLAGS) -shared -o $(BUILD)/dev_8smc1.dll 8SMC1Device.o 8SMC1DeviceManager.o NL300Instrument.o  -Wl,-Bdynamic,--library-path=$(BUILD) -lUSMCDLL $(LDFLAGS) -Wl,--library-path=$(LIB) -lcalx -Wl,--out-implib,$(BUILD)/\dev_8smc1.a

Device.o:
	$(CC) $(CFLAGS) -c device/Device.cpp

DeviceManager.o:
	$(CC) $(CFLAGS) -c device/DeviceManager.cpp

Stub.o:
	$(CC) $(CFLAGS) -Ires -c misc/Stub.cpp

ConfigManager.o:
	$(CC) $(CFLAGS) -c ctrl-lib/ConfigManager.cpp

ConfigValidator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/ConfigValidator.cpp

CoordController.o:
	$(CC) $(CFLAGS) -c ctrl-lib/CoordController.cpp

CoordHandle.o:
	$(CC) $(CFLAGS) -c ctrl-lib/CoordHandle.cpp

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

logger.o:
	$(CC) $(CFLAGS) -c ctrl-lib/logger.cpp

CircleGenerator.o:
	$(CC) $(CFLAGS) -c ctrl-lib/misc/CircleGenerator.cpp

GCodeParser.o:
	$(CC) $(CFLAGS) -c ctrl-lib/misc/GCodeParser.cpp

GCodeWriter.o:
	$(CC) $(CFLAGS) -c ctrl-lib/misc/GCodeWriter.cpp

GraphBuilder.o:
	$(CC) $(CFLAGS) -c ctrl-lib/misc/GraphBuilder.cpp

MotorController.o:
	$(CC) $(CFLAGS) -c ctrl-lib/MotorController.cpp

CoordPlaneLinearizer.o:
	$(CC) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneLinearizer.cpp

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


CalxActionQueue.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxActionQueue.cpp

CalxApp.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxApp.cpp

CalxConfigEditor.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxConfigEditor.cpp

CalxConsoleWidget.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxConsoleWidget.cpp

CalxDebugConsole.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxDebugConsole.cpp

CalxErrorHandler.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxErrorHandler.cpp

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

CalxCoordPlaneWatcher.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordPlaneWatcher.cpp

CalxVirtualPlane.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxVirtualPlane.cpp

CalxCOMSelectDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxCOMSelectDialog.cpp

CalxDevicePanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxDevicePanel.cpp

CalxInstrumentCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxInstrumentCtrl.cpp

CalxMotorCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxMotorCtrl.cpp

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

$(OUTPUT).dll: ConfigManager.o ConfigValidator.o CoordController.o CoordHandle.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o InstrumentController.o logger.o CircleGenerator.o GCodeParser.o GCodeWriter.o GraphBuilder.o MotorController.o CoordPlaneLinearizer.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o SystemManager.o CoordTask.o CoordTaskWrapper.o GCodeCoordTask.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o CLI.o DevCLI.o Device.o DeviceManager.o
	@mkdir -p $(BUILD)
	$(CC) -shared -o $(BUILD)/$(OUTPUT).dll ConfigManager.o ConfigValidator.o CoordController.o CoordHandle.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o InstrumentController.o logger.o CircleGenerator.o GCodeParser.o GCodeWriter.o GraphBuilder.o MotorController.o CoordPlaneLinearizer.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o SystemManager.o CoordTask.o CoordTaskWrapper.o GCodeCoordTask.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o CLI.o DevCLI.o Device.o DeviceManager.o  $(LDFLAGS) -Wl,--out-implib,$(BUILD)/$(OUTPUT).a

langs:
ifdef MSGFMT
	mkdir -p $(BUILD)/lang
endif
$(UI).exe: langs CalxActionQueue.o CalxApp.o CalxConfigEditor.o CalxConsoleWidget.o CalxDebugConsole.o CalxErrorHandler.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordFilter.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCoordPlaneWatcher.o CalxVirtualPlane.o CalxCOMSelectDialog.o CalxDevicePanel.o CalxInstrumentCtrl.o CalxMotorCtrl.o CalxGcodeHandle.o CalxGcodeLoader.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxTaskStepHandle.o
	@mkdir -p $(BUILD)
ifneq ($(WXLIB),)
	cp $(WX)/$(WXLIB) $(BUILD)
endif
	$(MAKE) icon
	$(MAKE) lang
	`$(WX)/wx-config --rescomp` winbuild/calxui.rc -O coff -o $(ICON)
	$(CC) -o $(BUILD)/$(UI).exe CalxActionQueue.o CalxApp.o CalxConfigEditor.o CalxConsoleWidget.o CalxDebugConsole.o CalxErrorHandler.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordFilter.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCoordPlaneWatcher.o CalxVirtualPlane.o CalxCOMSelectDialog.o CalxDevicePanel.o CalxInstrumentCtrl.o CalxMotorCtrl.o CalxGcodeHandle.o CalxGcodeLoader.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxTaskStepHandle.o  $(ICON) $(LDFLAGS) -Wl,--library-path=\$(BUILD) -lcalx `$(WX)/wx-config --libs`

all:
	$(MAKE) $(OUTPUT).dll
	$(MAKE) dev_8smc1.dll
	$(MAKE) $(OUTPUT).exe
	$(MAKE) $(UI).exe
	$(MAKE) copy



icon:
ifdef ICON
	convert misc/icon.svg -resize 256x256 -quality 100 icon.png
	convert icon.png -resize 128x128 -quality 100 icon128.png
	convert icon.png -resize 64x64 -quality 100 icon64.png
	convert icon.png -resize 48x48 -quality 100 icon48.png
	convert icon.png -resize 32x32 -quality 100 icon32.png
	convert *.png $(BUILD)/icon.ico
endif


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
	@rm -f *.png
	@rm -f *.res
	@rm -f *.log
	
copy:
	zip -r $(BUILD)/src.zip *.md Makefile platform.h config.ini device ctrl-lib ui winbuild misc cli tests COPYING COPYING.LESSER NOTICE
	cp config.ini $(BUILD)
	cp COPYING $(BUILD)
	cp COPYING.LESSER $(BUILD)
	cp NOTICE $(BUILD)