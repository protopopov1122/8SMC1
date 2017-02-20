CC=i686-w64-mingw32-g++
LD=i686-w64-mingw32-ld

OUTPUT=prog.exe
BUILD=./build
HEADERS=./res
LIB=./res

LANG=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-attributes
DBG=-O0
INCLUDES=-Idevice -Icli -Ictrl-lib
EXTRA=
CFLAGS=$(LANG) $(DBG) -I$(HEADERS) $(INCLUDES) $(EXTRA)

LFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
LIBS=-Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL

all: $(OUTPUT) stub


CLI.o:
	$(CC) $(CFLAGS) -c ./cli/CLI.cpp

DevCLI.o:
	$(CC) $(CFLAGS) -c ./cli/DevCLI.cpp

main.o:
	$(CC) $(CFLAGS) -c ./cli/main.cpp

CircleGenerator.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CircleGenerator.cpp

CoordController.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordController.cpp

CoordPlaneLog.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordPlaneLog.cpp

CoordPlaneMap.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordPlaneMap.cpp

CoordPlaneStack.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordPlaneStack.cpp

CoordPlaneValidator.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordPlaneValidator.cpp

CoordTask.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordTask.cpp

CoordTaskWrapper.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordTaskWrapper.cpp

CoordTranslator.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordTranslator.cpp

DeviceController.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/DeviceController.cpp

GCodeParser.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/GCodeParser.cpp

AST.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/graph/AST.cpp

DefaultFunctions.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/graph/DefaultFunctions.cpp

FunctionEngine.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/graph/FunctionEngine.cpp

FunctionLexer.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/graph/FunctionLexer.cpp

FunctionParser.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/graph/FunctionParser.cpp

SystemManager.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/SystemManager.cpp

Device.o:
	$(CC) $(CFLAGS) -c ./device/Device.cpp

DeviceManager.o:
	$(CC) $(CFLAGS) -c ./device/DeviceManager.cpp

MotorTask.o:
	$(CC) $(CFLAGS) -c ./device/MotorTask.cpp

Stub.o:
	$(CC) $(CFLAGS) -c ./misc/Stub.cpp

stub: Stub.o
	$(CC) -shared -o $(BUILD)/USMCDLL.dll Stub.o -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread

$(OUTPUT): Device.o DeviceManager.o MotorTask.o CLI.o DevCLI.o main.o CircleGenerator.o CoordController.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o CoordTask.o CoordTaskWrapper.o CoordTranslator.o DeviceController.o GCodeParser.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o SystemManager.o
	mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(OUTPUT) Device.o DeviceManager.o MotorTask.o CLI.o DevCLI.o main.o CircleGenerator.o CoordController.o CoordPlaneLog.o CoordPlaneMap.o CoordPlaneStack.o CoordPlaneValidator.o CoordTask.o CoordTaskWrapper.o CoordTranslator.o DeviceController.o GCodeParser.o AST.o DefaultFunctions.o FunctionEngine.o FunctionLexer.o FunctionParser.o SystemManager.o  -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL
	@cp $(LIB)/USMCDLL.dll $(BUILD)

clean:
	@rm -f *.o
	@rm -rf $(BUILD)
	@mkdir -p $(BUILD)
