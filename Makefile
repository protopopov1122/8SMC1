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


CoordTask.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordTask.cpp
	@cp CoordTask.o $(BUILD)

DeviceController.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/DeviceController.cpp
	@cp DeviceController.o $(BUILD)

CoordTranslator.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordTranslator.cpp
	@cp CoordTranslator.o $(BUILD)

SystemManager.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/SystemManager.cpp
	@cp SystemManager.o $(BUILD)

CoordController.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordController.cpp
	@cp CoordController.o $(BUILD)

Stub.o:
	$(CC) $(CFLAGS) -c ./misc/Stub.cxx
	@cp Stub.o $(BUILD)

DevCLI.o:
	$(CC) $(CFLAGS) -c ./cli/DevCLI.cpp
	@cp DevCLI.o $(BUILD)

main.o:
	$(CC) $(CFLAGS) -c ./cli/main.cpp
	@cp main.o $(BUILD)

CLI.o:
	$(CC) $(CFLAGS) -c ./cli/CLI.cpp
	@cp CLI.o $(BUILD)

MotorTask.o:
	$(CC) $(CFLAGS) -c ./device/MotorTask.cpp
	@cp MotorTask.o $(BUILD)

Device.o:
	$(CC) $(CFLAGS) -c ./device/Device.cpp
	@cp Device.o $(BUILD)

DeviceManager.o:
	$(CC) $(CFLAGS) -c ./device/DeviceManager.cpp
	@cp DeviceManager.o $(BUILD)

GCodeParser.o:
	$(CC) $(CFLAGS) -c ./gcode/GCodeParser.cpp
	@cp GCodeParser.o $(BUILD)

stub: Stub.o
	$(CC) -shared -o $(BUILD)/USMCDLL.dll Stub.o -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread

$(OUTPUT): MotorTask.o Device.o DeviceManager.o DevCLI.o main.o CLI.o CoordTask.o DeviceController.o CoordTranslator.o SystemManager.o CoordController.o
	$(CC) -o $(BUILD)/$(OUTPUT) MotorTask.o Device.o DeviceManager.o DevCLI.o main.o CLI.o CoordTask.o DeviceController.o CoordTranslator.o SystemManager.o CoordController.o  -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL
	@cp $(LIB)/USMCDLL.dll $(BUILD)

gcode: GCodeParser.o
	$(CC) -o $(BUILD)/$(OUTPUT) GCodeParser.o  -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL
	@cp $(LIB)/USMCDLL.dll $(BUILD)

clean:
	@rm -f *.o
	@rm -rf $(BUILD)
	@mkdir -p $(BUILD)
