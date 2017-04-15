# Build settings
TARGET=debug
PLATFORM=win
NAME=calx
UINAME=calxui
BUILD=./build
WXLIB=
EXTRA=
DEVAPI=standart

# Linux build specific
OUTPUT_linux=$(NAME)
OUTPUT_LIB_linux=lib$(NAME).so
OUTPUT_UI_linux=$(UINAME)
CC_linux=g++
LANG_linux=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter
DBG_debug_linux=-O0 -DLOGGING
DBG_release_linux=-O2
INCLUDES_linux=-I.
CFLAGS_LIB_linux=-fPIC
LDFLAGS_LIB_linux=
CFLAGS_linux=$(LANG_linux) $(INCLUDES_linux)
LDFLAGS_linux=-lpthread

# Windows specific
OUTPUT_win=$(NAME).exe
OUTPUT_LIB_win=$(NAME).dll
OUTPUT_UI_win=$(UINAME).exe
CC_win=i686-w64-mingw32-g++
LANG_win=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-attributes
DBG_debug_win=-O0 -DLOGGING
DBG_release_win=-O2
INCLUDES_win=-I.
CFLAGS_LIB_win=
LDFLAGS_LIB_win=
CFLAGS_win=$(LANG_win) $(DBG_win) $(INCLUDES_win)
LDFLAGS_win=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
CONVERT := $(shell command -v convert 2> /dev/null)
ifdef CONVERT
ICON=calxui.res
ifeq ($(PLATFORM), win)
ICON_RES=$(ICON)
endif
endif

# Build parameters
OUTPUT=$(OUTPUT_$(PLATFORM))
OUTPUT_LIB=$(OUTPUT_LIB_$(PLATFORM))
OUTPUT_UI=$(OUTPUT_UI_$(PLATFORM))
CC=$(CC_$(PLATFORM))
LANG=$(LANG_$(PLATFORM))
DBG=$(DBG_$(TARGET)_$(PLATFORM))
INCLUDES=$(INCLUDES_$(PLATFORM))
CFLAGS_LIB=$(CFLAGS_LIB_$(PLATFORM))
LDFLAGS_LIB=$(LDFLAGS_LIB_$(PLATFORM))
CFLAGS=$(CFLAGS_$(PLATFORM)) $(DBG) $(EXTRA)
LDFLAGS=$(LDFLAGS_$(PLATFORM))

8SMC1Device.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/8SMC1Device.cpp

StandartDeviceManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/StandartDeviceManager.cpp

NL300Instrument.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/NL300Instrument.cpp

devapi_standart: 8SMC1Device.o StandartDeviceManager.o NL300Instrument.o  $(OUTPUT_LIB)
	$(CC) -shared 8SMC1Device.o StandartDeviceManager.o NL300Instrument.o   -o $(BUILD)/dev_standart.dll -Wl,-Bdynamic,--library-path=$(BUILD) -lUSMCDLL  -Wl,--out-implib,$(BUILD)/\dev_standart.a $(LDFLAGS) $(LDFLAGS_LIB) -Wl,--library-path=$(BUILD) -lcalx
EmuInstrument.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS)  -c device/emulated/EmuInstrument.cpp

EmuDeviceManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS)  -c device/emulated/EmuDeviceManager.cpp

EmuMotor.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS)  -c device/emulated/EmuMotor.cpp

devapi_emulated: EmuInstrument.o EmuDeviceManager.o EmuMotor.o  $(OUTPUT_LIB)
	$(CC) -shared EmuInstrument.o EmuDeviceManager.o EmuMotor.o   -o $(BUILD)/libdev_emulated.so $(LDFLAGS) $(LDFLAGS_LIB) -Wl,--library-path=$(BUILD) -lcalx

Stub.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c misc/Stub.cpp

CoordTask.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/task/CoordTask.cpp

CoordTaskWrapper.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/task/CoordTaskWrapper.cpp

GCodeCoordTask.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/task/GCodeCoordTask.cpp

LogarithmicCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/LogarithmicCoordTranslator.cpp

PolarCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/PolarCoordTranslator.cpp

LinearCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/LinearCoordTranslator.cpp

BasicCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/BasicCoordTranslator.cpp

ComplexCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/ComplexCoordTranslator.cpp

MotorController.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/MotorController.cpp

GraphBuilder.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/misc/GraphBuilder.cpp

GCodeWriter.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/misc/GCodeWriter.cpp

GCodeParser.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/misc/GCodeParser.cpp

CircleGenerator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/misc/CircleGenerator.cpp

ConfigValidator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/ConfigValidator.cpp

Device.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/device/Device.cpp

DeviceManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/device/DeviceManager.cpp

DefaultFunctions.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/DefaultFunctions.cpp

FunctionLexer.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/FunctionLexer.cpp

FunctionEngine.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/FunctionEngine.cpp

AST.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/AST.cpp

FunctionParser.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/FunctionParser.cpp

logger.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/logger.cpp

CoordHandle.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/CoordHandle.cpp

CoordPlaneLog.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneLog.cpp

CoordPlaneMap.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneMap.cpp

CoordPlaneStack.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneStack.cpp

VirtualCoordPlane.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/VirtualCoordPlane.cpp

CoordPlaneValidator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneValidator.cpp

CoordPlaneLinearizer.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneLinearizer.cpp

RequestResolver.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/RequestResolver.cpp

SystemManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/SystemManager.cpp

StandartRequestResolvers.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/StandartRequestResolvers.cpp

ConfigManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/ConfigManager.cpp

CoordController.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/CoordController.cpp

InstrumentController.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/InstrumentController.cpp

DevCLI.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c cli/DevCLI.cpp

main.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c cli/main.cpp

CLI.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c cli/CLI.cpp


CalxGcodeLoader.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxGcodeLoader.cpp

CalxProgrammedTaskHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxProgrammedTaskHandle.cpp

CalxTaskPanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxTaskPanel.cpp

CalxGcodeHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxGcodeHandle.cpp

CalxTaskStepHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxTaskStepHandle.cpp

CalxDebugConsole.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxDebugConsole.cpp

CalxActionQueue.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxActionQueue.cpp

CalxFrame.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxFrame.cpp

CalxCoordArcCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordArcCtrl.cpp

CalxCoordCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordCtrl.cpp

CalxCoordFilter.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordFilter.cpp

CalxCoordOtherCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordOtherCtrl.cpp

CalxCoordDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordDialog.cpp

CalxCoordMiscCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordMiscCtrl.cpp

CalxCoordPlaneWatcher.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordPlaneWatcher.cpp

CalxCoordGraphCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordGraphCtrl.cpp

CalxVirtualPlane.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxVirtualPlane.cpp

CalxCoordPanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordPanel.cpp

CalxCoordLinearCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordLinearCtrl.cpp

CalxPanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxPanel.cpp

CalxDevicePanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxDevicePanel.cpp

CalxCOMSelectDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxCOMSelectDialog.cpp

CalxInstrumentCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxInstrumentCtrl.cpp

CalxMotorCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/dev/CalxMotorCtrl.cpp

CalxConfigEditor.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxConfigEditor.cpp

CalxErrorHandler.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxErrorHandler.cpp

CalxApp.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxApp.cpp

CalxAutoconfDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxAutoconfDialog.cpp

CalxConsoleWidget.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxConsoleWidget.cpp

stub: Stub.o
	$(CC) -shared -o $(BUILD)/USMCDLL.dll Stub.o $(LDFLAGS)

$(OUTPUT): $(OUTPUT_LIB) devapi_$(DEVAPI) DevCLI.o main.o CLI.o
	@mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(OUTPUT) DevCLI.o main.o CLI.o  $(LDFLAGS) -Wl,--library-path=$(BUILD) -ldev_$(DEVAPI) -l$(NAME)

$(OUTPUT_LIB): CoordTask.o CoordTaskWrapper.o GCodeCoordTask.o LogarithmicCoordTranslator.o PolarCoordTranslator.o LinearCoordTranslator.o BasicCoordTranslator.o ComplexCoordTranslator.o MotorController.o GraphBuilder.o GCodeWriter.o GCodeParser.o CircleGenerator.o ConfigValidator.o Device.o DeviceManager.o DefaultFunctions.o FunctionLexer.o FunctionEngine.o AST.o FunctionParser.o logger.o CoordHandle.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o VirtualCoordPlane.o CoordPlaneValidator.o CoordPlaneLinearizer.o RequestResolver.o SystemManager.o StandartRequestResolvers.o ConfigManager.o CoordController.o InstrumentController.o
	@mkdir -p $(BUILD)
	$(CC) -shared -o $(BUILD)/$(OUTPUT_LIB) CoordTask.o CoordTaskWrapper.o GCodeCoordTask.o LogarithmicCoordTranslator.o PolarCoordTranslator.o LinearCoordTranslator.o BasicCoordTranslator.o ComplexCoordTranslator.o MotorController.o GraphBuilder.o GCodeWriter.o GCodeParser.o CircleGenerator.o ConfigValidator.o Device.o DeviceManager.o DefaultFunctions.o FunctionLexer.o FunctionEngine.o AST.o FunctionParser.o logger.o CoordHandle.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o VirtualCoordPlane.o CoordPlaneValidator.o CoordPlaneLinearizer.o RequestResolver.o SystemManager.o StandartRequestResolvers.o ConfigManager.o CoordController.o InstrumentController.o  $(LDFLAGS) $(LDFLAGS_LIB)

langs:
ifdef MSGFMT
	mkdir -p $(BUILD)/lang
endif

$(OUTPUT_UI): langs CalxGcodeLoader.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxGcodeHandle.o CalxTaskStepHandle.o CalxDebugConsole.o CalxActionQueue.o CalxFrame.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordFilter.o CalxCoordOtherCtrl.o CalxCoordDialog.o CalxCoordMiscCtrl.o CalxCoordPlaneWatcher.o CalxCoordGraphCtrl.o CalxVirtualPlane.o CalxCoordPanel.o CalxCoordLinearCtrl.o CalxPanel.o CalxDevicePanel.o CalxCOMSelectDialog.o CalxInstrumentCtrl.o CalxMotorCtrl.o CalxConfigEditor.o CalxErrorHandler.o CalxApp.o CalxAutoconfDialog.o CalxConsoleWidget.o DevCLI.o CLI.o
	@mkdir -p $(BUILD)
ifneq ($(WXLIB),)
	cp $(WX)/$(WXLIB) $(BUILD)
endif
	$(MAKE) icon
	$(MAKE) lang
	$(CC) -o $(BUILD)/$(OUTPUT_UI) CalxGcodeLoader.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxGcodeHandle.o CalxTaskStepHandle.o CalxDebugConsole.o CalxActionQueue.o CalxFrame.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordFilter.o CalxCoordOtherCtrl.o CalxCoordDialog.o CalxCoordMiscCtrl.o CalxCoordPlaneWatcher.o CalxCoordGraphCtrl.o CalxVirtualPlane.o CalxCoordPanel.o CalxCoordLinearCtrl.o CalxPanel.o CalxDevicePanel.o CalxCOMSelectDialog.o CalxInstrumentCtrl.o CalxMotorCtrl.o CalxConfigEditor.o CalxErrorHandler.o CalxApp.o CalxAutoconfDialog.o CalxConsoleWidget.o DevCLI.o CLI.o  $(ICON_RES) $(LDFLAGS) -Wl,--library-path=\$(BUILD) -l$(NAME) `$(WX)/wx-config --libs`

all:
	$(MAKE) $(OUTPUT_LIB)
	$(MAKE) devapi_standart
	$(MAKE) devapi_emulated
	$(MAKE) $(OUTPUT)
	$(MAKE) $(OUTPUT_UI)
	$(MAKE) copy

icon:
ifdef ICON
	convert misc/icon.svg -resize 256x256 -quality 100 icon.png
	convert icon.png -resize 128x128 -quality 100 icon128.png
	convert icon.png -resize 64x64 -quality 100 icon64.png
	convert icon.png -resize 48x48 -quality 100 icon48.png
	convert icon.png -resize 32x32 -quality 100 icon32.png
	convert *.png $(BUILD)/icon.ico
ifeq ($(PLATFORM), win)
	`$(WX)/wx-config --rescomp` winbuild/calxui.rc -O coff -o $(ICON)
endif
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
	zip -r $(BUILD)/src.zip *.md Makefile platform.h config.ini device ctrl-lib ui winbuild misc cli tests COPYING COPYING.LESSER NOTICE auto.conf
	cp config.ini $(BUILD)
	cp COPYING $(BUILD)
	cp COPYING.LESSER $(BUILD)
	cp NOTICE $(BUILD)
	cp auto.conf $(BUILD)