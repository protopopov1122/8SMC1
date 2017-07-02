# Build settings
TARGET=debug
PLATFORM=win
NAME=calx
UINAME=calx_ui
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
INCLUDES_linux=-I./headers
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
INCLUDES_win=-I./headers
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

NL300Command.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/NL300Command.cpp

NL300Config.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/NL300Config.cpp

NL300Instrument.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/NL300Instrument.cpp

NL300Message.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/NL300Message.cpp

StandartDeviceManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/StandartDeviceManager.cpp

devapi_standart: 8SMC1Device.o NL300Command.o NL300Config.o NL300Instrument.o NL300Message.o StandartDeviceManager.o  $(OUTPUT_LIB)
	$(CC) -shared 8SMC1Device.o NL300Command.o NL300Config.o NL300Instrument.o NL300Message.o StandartDeviceManager.o   -o $(BUILD)/dev_standart.dll -Wl,-Bdynamic,--library-path=$(BUILD) -lUSMCDLL  -Wl,--out-implib,$(BUILD)/dev_standart.a $(LDFLAGS) $(LDFLAGS_LIB) -Wl,--library-path=$(BUILD) -lcalx
EmuDeviceManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS)  -c device/emulated/EmuDeviceManager.cpp

EmuInstrument.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS)  -c device/emulated/EmuInstrument.cpp

EmuMotor.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS)  -c device/emulated/EmuMotor.cpp

devapi_emulated: EmuDeviceManager.o EmuInstrument.o EmuMotor.o  $(OUTPUT_LIB)
	$(CC) -shared EmuDeviceManager.o EmuInstrument.o EmuMotor.o   -o $(BUILD)/libdev_emulated.so $(LDFLAGS) $(LDFLAGS_LIB) -Wl,--library-path=$(BUILD) -lcalx
ext_sample_main.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS)  -c ext/sample/ext_sample_main.cpp

exteng_sample: ext_sample_main.o  $(OUTPUT_LIB)
	$(CC) -shared ext_sample_main.o   -o $(BUILD)/ext_sample.dll $(LDFLAGS) $(LDFLAGS_LIB) -Wl,--library-path=$(BUILD) -lcalx

Stub.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c device/standart/Stub.cpp

ConfigManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/conf/ConfigManager.cpp

ConfigValidator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/conf/ConfigValidator.cpp

RequestResolver.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/conf/RequestResolver.cpp

StandartRequestResolvers.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/conf/StandartRequestResolvers.cpp

InstrumentController.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/ctrl/InstrumentController.cpp

MotorController.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/ctrl/MotorController.cpp

Device.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/device/Device.cpp

DeviceManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/device/DeviceManager.cpp

GCodeInterpreter.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/gcode/GCodeInterpreter.cpp

GCodeStream.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/gcode/GCodeStream.cpp

GCodeWriter.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/gcode/GCodeWriter.cpp

AST.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/AST.cpp

DefaultFunctions.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/DefaultFunctions.cpp

FunctionEngine.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/FunctionEngine.cpp

FunctionLexer.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/FunctionLexer.cpp

FunctionParser.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/graph/FunctionParser.cpp

logger.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/logger.cpp

CircleGenerator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/misc/CircleGenerator.cpp

GraphBuilder.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/misc/GraphBuilder.cpp

CoordController.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordController.cpp

CoordHandle.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordHandle.cpp

CoordPlane.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlane.cpp

CoordPlaneLinearizer.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneLinearizer.cpp

CoordPlaneLog.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneLog.cpp

CoordPlaneMap.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneMap.cpp

CoordPlaneStack.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneStack.cpp

CoordPlaneValidator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneValidator.cpp

FloatCoordPlane.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/FloatCoordPlane.cpp

VirtualCoordPlane.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/plane/VirtualCoordPlane.cpp

SystemManager.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/SystemManager.cpp

CoordTask.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/task/CoordTask.cpp

GCodeCoordTask.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/task/GCodeCoordTask.cpp

LinearCoordTask.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/task/LinearCoordTask.cpp

BasicCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/BasicCoordTranslator.cpp

ComplexCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/ComplexCoordTranslator.cpp

LinearCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/LinearCoordTranslator.cpp

LogarithmicCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/LogarithmicCoordTranslator.cpp

PolarCoordTranslator.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c ctrl-lib/translator/PolarCoordTranslator.cpp

CLI.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c cli/CLI.cpp

DevCLI.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c cli/DevCLI.cpp

main.o:
	$(CC) $(CFLAGS_LIB) $(CFLAGS) -c cli/main.cpp


CalxActionQueue.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxActionQueue.cpp

CalxApp.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxApp.cpp

CalxAutoconfDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxAutoconfDialog.cpp

CalxConfigEditor.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxConfigEditor.cpp

CalxConfigLoader.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxConfigLoader.cpp

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

CalxCoordPositionCtrl.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/coord/CalxCoordPositionCtrl.cpp

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

CalxLinearTaskHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxLinearTaskHandle.cpp

CalxProgrammedTaskHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxProgrammedTaskHandle.cpp

CalxTaskPanel.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxTaskPanel.cpp

CalxTaskStepHandle.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/task/CalxTaskStepHandle.cpp

stub: Stub.o
	$(CC) -shared -o $(BUILD)/USMCDLL.dll Stub.o $(LDFLAGS)

$(OUTPUT): $(OUTPUT_LIB) devapi_$(DEVAPI) CLI.o DevCLI.o main.o
	@mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(OUTPUT) CLI.o DevCLI.o main.o  $(LDFLAGS) -Wl,--library-path=$(BUILD) -ldev_$(DEVAPI) -l$(NAME)

$(OUTPUT_LIB): ConfigManager.o ConfigValidator.o RequestResolver.o StandartRequestResolvers.o InstrumentController.o MotorController.o Device.o DeviceManager.o GCodeInterpreter.o GCodeStream.o GCodeWriter.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o logger.o CircleGenerator.o GraphBuilder.o CoordController.o CoordHandle.o CoordPlane.o CoordPlaneLinearizer.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o FloatCoordPlane.o VirtualCoordPlane.o SystemManager.o CoordTask.o GCodeCoordTask.o LinearCoordTask.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o
	@mkdir -p $(BUILD)
	$(CC) -shared -o $(BUILD)/$(OUTPUT_LIB) ConfigManager.o ConfigValidator.o RequestResolver.o StandartRequestResolvers.o InstrumentController.o MotorController.o Device.o DeviceManager.o GCodeInterpreter.o GCodeStream.o GCodeWriter.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o logger.o CircleGenerator.o GraphBuilder.o CoordController.o CoordHandle.o CoordPlane.o CoordPlaneLinearizer.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o FloatCoordPlane.o VirtualCoordPlane.o SystemManager.o CoordTask.o GCodeCoordTask.o LinearCoordTask.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o  $(LDFLAGS) $(LDFLAGS_LIB)

$(OUTPUT_UI): CalxActionQueue.o CalxApp.o CalxAutoconfDialog.o CalxConfigEditor.o CalxConfigLoader.o CalxDebugConsole.o CalxErrorHandler.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordFilter.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCoordPlaneWatcher.o CalxCoordPositionCtrl.o CalxVirtualPlane.o CalxCOMSelectDialog.o CalxDevicePanel.o CalxInstrumentCtrl.o CalxMotorCtrl.o CalxGcodeHandle.o CalxGcodeLoader.o CalxLinearTaskHandle.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxTaskStepHandle.o CLI.o DevCLI.o
	@mkdir -p $(BUILD)
ifneq ($(WXLIB),)
	cp $(WX)/$(WXLIB) $(BUILD)
endif
	$(MAKE) -f ./misc/old-build/Makefile icon BUILD=$(BUILD)
	$(CC) -o $(BUILD)/$(OUTPUT_UI) CalxActionQueue.o CalxApp.o CalxAutoconfDialog.o CalxConfigEditor.o CalxConfigLoader.o CalxDebugConsole.o CalxErrorHandler.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordFilter.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCoordPlaneWatcher.o CalxCoordPositionCtrl.o CalxVirtualPlane.o CalxCOMSelectDialog.o CalxDevicePanel.o CalxInstrumentCtrl.o CalxMotorCtrl.o CalxGcodeHandle.o CalxGcodeLoader.o CalxLinearTaskHandle.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxTaskStepHandle.o CLI.o DevCLI.o  $(ICON_RES) $(LDFLAGS) -Wl,--library-path=\$(BUILD) -l$(NAME) `$(WX)/wx-config --libs`

all:
	$(MAKE) -f ./misc/old-build/Makefile $(OUTPUT_LIB) BUILD=$(BUILD)
	$(MAKE) -f ./misc/old-build/Makefile devapi_standart BUILD=$(BUILD)
	$(MAKE) -f ./misc/old-build/Makefile devapi_emulated BUILD=$(BUILD)
	$(MAKE) -f ./misc/old-build/Makefile exteng_sample BUILD=$(BUILD)
	$(MAKE) -f ./misc/old-build/Makefile $(OUTPUT) BUILD=$(BUILD)
	$(MAKE) -f ./misc/old-build/Makefile $(OUTPUT_LIB) $(OUTPUT_UI) BUILD=$(BUILD)
	$(MAKE) -f ./misc/old-build/Makefile copy BUILD=$(BUILD)

icon:
ifdef ICON
	convert misc/icon.svg -resize 256x256 -quality 100 icon.png
	convert icon.png -resize 128x128 -quality 100 icon128.png
	convert icon.png -resize 64x64 -quality 100 icon64.png
	convert icon.png -resize 48x48 -quality 100 icon48.png
	convert icon.png -resize 32x32 -quality 100 icon32.png
	convert *.png $(BUILD)/icon.ico
ifeq ($(PLATFORM), win)
	`$(WX)/wx-config --rescomp` ui/calxui.rc -O coff -o $(ICON)
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
	zip -r $(BUILD)/src.zip *.md cli ctrl-lib device docs ext headers misc tests ui CMakeLists.txt COPYING COPYING.LESSER NOTICE
	cp misc/main.conf.ini $(BUILD)
	cp device/standart/NL300.ini $(BUILD)
	cp device/standart/8SMC1.ini $(BUILD)
	cp COPYING $(BUILD)
	cp COPYING.LESSER $(BUILD)
	cp NOTICE $(BUILD)
	cp misc/auto.conf $(BUILD)