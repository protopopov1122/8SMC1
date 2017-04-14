# Build target
TARGET=debug
PLATFORM=win
OUTPUT=calx
UI=calxui
BUILD=./build
WXLIB=
EXTRA=

# Linux build specific
OUTPUT.EXE_linux=$(OUTPUT)
OUTPUT.LIB_linux=lib$(OUTPUT).so
OUTPUT_UI_linux=$(UI)
CC_linux=g++
LANG_linux=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter
DBG_linux_debug=-O0 -DLOGGING
DBG_linux_release=-O2
DBG_linux=$(DBG_linux_$(TARGET))
INCLUDES_linux=-I.
LIBCFLAGS_linux=-fPIC
LIBLDFLAGS_linux=
CFLAGS_linux=$(LANG_linux) $(DBG_linux) $(INCLUDES_linux) $(EXTRA)
LDFLAGS_linux=-lpthread

# Windows build specific
OUTPUT.EXE_win=$(OUTPUT).exe
OUTPUT.LIB_win=$(OUTPUT).dll
OUTPUT_UI_win=$(UI).exe
CC_win=i686-w64-mingw32-g++
LANG_win=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-attributes
DBG_win_debug=-O0 -DLOGGING
DBG_win_release=-O2
DBG_win=$(DBG_win_$(TARGET))
INCLUDES_win=-I.
LIBCFLAGS_win=
LIB_LDFLAGS_win=
CFLAGS_win=$(LANG_win) $(DBG_win) $(INCLUDES_win) $(EXTRA)
LDFLAGS_win=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
# Windows tools
CONVERT := $(shell command -v convert 2> /dev/null)
ifdef CONVERT
ICON=calxui.res
endif

#Build flags
OUTPUT.EXE=$(OUTPUT.EXE_$(PLATFORM))
OUTPUT.LIB=$(OUTPUT.LIB_$(PLATFORM))
OUTPUT_UI=$(OUTPUT_UI_$(PLATFORM))
CC=$(CC_$(PLATFORM))
LIBCFLAGS=$(LIBCFLAGS_$(PLATFORM))
LIBLDFLAGS=$(LIBLDFLAGS_$(PLATFORM))
LDFLAGS=$(LDFLAGS_$(PLATFORM))
CFLAGS=$(CFLAGS_$(PLATFORM))


8SMC1Device.o:
	$(CC) $(CFLAGS) -Ires -c device/standart/8SMC1Device.cpp

NL300Instrument.o:
	$(CC) $(CFLAGS) -Ires -c device/standart/NL300Instrument.cpp

StandartDeviceManager.o:
	$(CC) $(CFLAGS) -Ires -c device/standart/StandartDeviceManager.cpp

dev_standart.dll: 8SMC1Device.o NL300Instrument.o StandartDeviceManager.o  $(OUTPUT).dll
	$(CC) $(CFLAGS) -shared -o $(BUILD)/dev_standart.dll 8SMC1Device.o NL300Instrument.o StandartDeviceManager.o  -Wl,-Bdynamic,--library-path=$(BUILD) -lUSMCDLL $(LDFLAGS) -Wl,--library-path=$(LIB) -lcalx -Wl,--out-implib,$(BUILD)/\dev_standart.a

Device.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c device/Device.cpp

DeviceManager.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c device/DeviceManager.cpp

Stub.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -Ires -c misc/Stub.cpp

ConfigManager.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/ConfigManager.cpp

ConfigValidator.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/ConfigValidator.cpp

CoordController.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/CoordController.cpp

CoordHandle.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/CoordHandle.cpp

AST.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/graph/AST.cpp

DefaultFunctions.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/graph/DefaultFunctions.cpp

FunctionEngine.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/graph/FunctionEngine.cpp

FunctionLexer.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/graph/FunctionLexer.cpp

FunctionParser.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/graph/FunctionParser.cpp

InstrumentController.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/InstrumentController.cpp

logger.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/logger.cpp

CircleGenerator.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/misc/CircleGenerator.cpp

GCodeParser.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/misc/GCodeParser.cpp

GCodeWriter.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/misc/GCodeWriter.cpp

GraphBuilder.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/misc/GraphBuilder.cpp

MotorController.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/MotorController.cpp

CoordPlaneLinearizer.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneLinearizer.cpp

CoordPlaneLog.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneLog.cpp

CoordPlaneMap.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneMap.cpp

CoordPlaneStack.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneStack.cpp

CoordPlaneValidator.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/plane/CoordPlaneValidator.cpp

VirtualCoordPlane.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/plane/VirtualCoordPlane.cpp

RequestResolver.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/RequestResolver.cpp

StandartRequestResolvers.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/StandartRequestResolvers.cpp

SystemManager.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/SystemManager.cpp

CoordTask.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/task/CoordTask.cpp

CoordTaskWrapper.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/task/CoordTaskWrapper.cpp

GCodeCoordTask.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/task/GCodeCoordTask.cpp

BasicCoordTranslator.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/translator/BasicCoordTranslator.cpp

ComplexCoordTranslator.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/translator/ComplexCoordTranslator.cpp

LinearCoordTranslator.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/translator/LinearCoordTranslator.cpp

LogarithmicCoordTranslator.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/translator/LogarithmicCoordTranslator.cpp

PolarCoordTranslator.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c ctrl-lib/translator/PolarCoordTranslator.cpp

CLI.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c cli/CLI.cpp

DevCLI.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c cli/DevCLI.cpp

main.o:
	$(CC) $(LIBCFLAGS) $(CFLAGS) -c cli/main.cpp


CalxActionQueue.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxActionQueue.cpp

CalxApp.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxApp.cpp

CalxAutoconfDialog.o:
	$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c ui/CalxAutoconfDialog.cpp

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

$(OUTPUT.EXE): $(OUTPUT.LIB) dev_standart.dll main.o
	@mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(OUTPUT).exe main.o  $(LDFLAGS) -Wl,--library-path=$(BUILD) -ldev_standart -l$(OUTPUT)

$(OUTPUT.LIB): ConfigManager.o ConfigValidator.o CoordController.o CoordHandle.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o InstrumentController.o logger.o CircleGenerator.o GCodeParser.o GCodeWriter.o GraphBuilder.o MotorController.o CoordPlaneLinearizer.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o RequestResolver.o StandartRequestResolvers.o SystemManager.o CoordTask.o CoordTaskWrapper.o GCodeCoordTask.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o CLI.o DevCLI.o Device.o DeviceManager.o
	@mkdir -p $(BUILD)
	$(CC) -shared -o $(BUILD)/$(OUTPUT.LIB) ConfigManager.o ConfigValidator.o CoordController.o CoordHandle.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o InstrumentController.o logger.o CircleGenerator.o GCodeParser.o GCodeWriter.o GraphBuilder.o MotorController.o CoordPlaneLinearizer.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o RequestResolver.o StandartRequestResolvers.o SystemManager.o CoordTask.o CoordTaskWrapper.o GCodeCoordTask.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o CLI.o DevCLI.o Device.o DeviceManager.o  $(LDFLAGS) $(LIBLDFLAGS)

langs:
ifdef MSGFMT
	mkdir -p $(BUILD)/lang
endif
$(OUTPUT_UI): langs CalxActionQueue.o CalxApp.o CalxAutoconfDialog.o CalxConfigEditor.o CalxConsoleWidget.o CalxDebugConsole.o CalxErrorHandler.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordFilter.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCoordPlaneWatcher.o CalxVirtualPlane.o CalxCOMSelectDialog.o CalxDevicePanel.o CalxInstrumentCtrl.o CalxMotorCtrl.o CalxGcodeHandle.o CalxGcodeLoader.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxTaskStepHandle.o
	@mkdir -p $(BUILD)
ifneq ($(WXLIB),)
	cp $(WX)/$(WXLIB) $(BUILD)
endif
	$(MAKE) icon
	$(MAKE) lang
	`$(WX)/wx-config --rescomp` winbuild/calxui.rc -O coff -o $(ICON)
	$(CC) -o $(BUILD)/$(OUTPUT_UI) CalxActionQueue.o CalxApp.o CalxAutoconfDialog.o CalxConfigEditor.o CalxConsoleWidget.o CalxDebugConsole.o CalxErrorHandler.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordFilter.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCoordPlaneWatcher.o CalxVirtualPlane.o CalxCOMSelectDialog.o CalxDevicePanel.o CalxInstrumentCtrl.o CalxMotorCtrl.o CalxGcodeHandle.o CalxGcodeLoader.o CalxProgrammedTaskHandle.o CalxTaskPanel.o CalxTaskStepHandle.o  $(ICON) $(LDFLAGS) -Wl,--library-path=\$(BUILD) -lcalx `$(WX)/wx-config --libs`

all:
	$(MAKE) $(OUTPUT).dll
	$(MAKE) dev_standart.dll
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
	zip -r $(BUILD)/src.zip *.md Makefile platform.h config.ini device ctrl-lib ui winbuild misc cli tests COPYING COPYING.LESSER NOTICE auto.conf
	cp config.ini $(BUILD)
	cp COPYING $(BUILD)
	cp COPYING.LESSER $(BUILD)
	cp NOTICE $(BUILD)
	cp auto.conf $(BUILD)