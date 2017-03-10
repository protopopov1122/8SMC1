CC=i686-w64-mingw32-g++

OUTPUT=8smc1
UI=ui
BUILD=./build
HEADERS=./res
LIB=./res
WX=.

TARGET=debug
LANG=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-attributes
DBG_debug=-O0
DBG_release=-O2
DBG=$(DBG_$(TARGET))
INCLUDES=-I.
EXTRA=
CFLAGS=$(LANG) $(DBG) -I$(HEADERS) $(INCLUDES) $(EXTRA)

LFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
LIBS=-Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL

all: $(OUTPUT) stub


Stub.o:
	$(CC) $(CFLAGS) -c misc/Stub.cpp

8SMC1Device.o:
	$(CC) $(CFLAGS) -c device/8smc1/8SMC1Device.cpp

8SMC1DeviceManager.o:
	$(CC) $(CFLAGS) -c device/8smc1/8SMC1DeviceManager.cpp

Device.o:
	$(CC) $(CFLAGS) -c device/Device.cpp

DeviceManager.o:
	$(CC) $(CFLAGS) -c device/DeviceManager.cpp

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
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/CalxActionQueue.cpp

CalxApp.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/CalxApp.cpp

CalxConsoleWidget.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/CalxConsoleWidget.cpp

CalxFrame.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/CalxFrame.cpp

CalxPanel.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/CalxPanel.cpp

CalxCoordArcCtrl.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordArcCtrl.cpp

CalxCoordCtrl.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordCtrl.cpp

CalxCoordDialog.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordDialog.cpp

CalxCoordGraphCtrl.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordGraphCtrl.cpp

CalxCoordLinearCtrl.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordLinearCtrl.cpp

CalxCoordMiscCtrl.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordMiscCtrl.cpp

CalxCoordOtherCtrl.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordOtherCtrl.cpp

CalxCoordPanel.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordPanel.cpp

CalxCoordTranslator.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/coord/CalxCoordTranslator.cpp

CalxDeviceCtrl.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/dev/CalxDeviceCtrl.cpp

CalxDevicePanel.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/dev/CalxDevicePanel.cpp

CalxGcodeHandle.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/task/CalxGcodeHandle.cpp

CalxGcodeLoader.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/task/CalxGcodeLoader.cpp

CalxTaskPanel.o:
	$(CC) $(CFLAGS) -I ui -I $(WX)/include -I $(WX)/lib/wx/include/i586-mingw32msvc-msw-unicode-3.1 -c ui/task/CalxTaskPanel.cpp

stub: Stub.o
	$(CC) -shared -o $(BUILD)/USMCDLL.dll Stub.o -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread

$(OUTPUT).exe: 8SMC1Device.o 8SMC1DeviceManager.o Device.o DeviceManager.o CLI.o DevCLI.o main.o CoordController.o CoordHandle.o DeviceController.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o CircleGenerator.o GCodeParser.o GraphBuilder.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o SystemManager.o CoordTask.o CoordTaskWrapper.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o
	mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(OUTPUT).exe 8SMC1Device.o 8SMC1DeviceManager.o Device.o DeviceManager.o CLI.o DevCLI.o main.o CoordController.o CoordHandle.o DeviceController.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o CircleGenerator.o GCodeParser.o GraphBuilder.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o SystemManager.o CoordTask.o CoordTaskWrapper.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o  -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL

$(OUTPUT).dll: 8SMC1Device.o 8SMC1DeviceManager.o Device.o DeviceManager.o CLI.o DevCLI.o main.o CoordController.o CoordHandle.o DeviceController.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o CircleGenerator.o GCodeParser.o GraphBuilder.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o SystemManager.o CoordTask.o CoordTaskWrapper.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o
	mkdir -p $(BUILD)
	$(CC) -shared -o $(BUILD)/$(OUTPUT).dll 8SMC1Device.o 8SMC1DeviceManager.o Device.o DeviceManager.o CLI.o DevCLI.o main.o CoordController.o CoordHandle.o DeviceController.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o CircleGenerator.o GCodeParser.o GraphBuilder.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o VirtualCoordPlane.o SystemManager.o CoordTask.o CoordTaskWrapper.o BasicCoordTranslator.o ComplexCoordTranslator.o LinearCoordTranslator.o LogarithmicCoordTranslator.o PolarCoordTranslator.o  -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL -Wl,--out-implib,$(BUILD)/$(OUTPUT).a

$(UI).exe: CalxActionQueue.o CalxApp.o CalxConsoleWidget.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCoordTranslator.o CalxDeviceCtrl.o CalxDevicePanel.o CalxGcodeHandle.o CalxGcodeLoader.o CalxTaskPanel.o
	mkdir -p $(BUILD)
	cp $(WX)/lib/wxmsw310u_gcc_custom.dll $(BUILD)
	$(CC) -o $(BUILD)/$(UI).exe CalxActionQueue.o CalxApp.o CalxConsoleWidget.o CalxFrame.o CalxPanel.o CalxCoordArcCtrl.o CalxCoordCtrl.o CalxCoordDialog.o CalxCoordGraphCtrl.o CalxCoordLinearCtrl.o CalxCoordMiscCtrl.o CalxCoordOtherCtrl.o CalxCoordPanel.o CalxCoordTranslator.o CalxDeviceCtrl.o CalxDevicePanel.o CalxGcodeHandle.o CalxGcodeLoader.o CalxTaskPanel.o  -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic,--library-path=$(BUILD) -lUSMCDLL -l8smc1 -lwxmsw310u_gcc_custom


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