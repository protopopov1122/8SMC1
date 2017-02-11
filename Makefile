CC=i686-w64-mingw32-g++
LD=i686-w64-mingw32-ld

OUTPUT=prog.exe
BUILD=./build
HEADERS=./res
LIB=./res

LANG=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter
DBG=-g3 -ggdb -O3
INCLUDES=-Idevice -Icli -Ictrl-lib
EXTRA=
CFLAGS=$(LANG) $(DBG) -I$(HEADERS) $(INCLUDES) $(EXTRA)

LFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
LIBS=-Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL

all: $(OUTPUT) stub


CoordTask.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordTask.cpp

DeviceController.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/DeviceController.cpp

CoordTranslator.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordTranslator.cpp

SystemManager.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/SystemManager.cpp

CoordController.o:
	$(CC) $(CFLAGS) -c ./ctrl-lib/CoordController.cpp

Stub.o:
	$(CC) $(CFLAGS) -c ./misc/Stub.cxx

DevCLI.o:
	$(CC) $(CFLAGS) -c ./cli/DevCLI.cpp

main.o:
	$(CC) $(CFLAGS) -c ./cli/main.cpp

CLI.o:
	$(CC) $(CFLAGS) -c ./cli/CLI.cpp

MotorTask.o:
	$(CC) $(CFLAGS) -c ./device/MotorTask.cpp

Device.o:
	$(CC) $(CFLAGS) -c ./device/Device.cpp

DeviceManager.o:
	$(CC) $(CFLAGS) -c ./device/DeviceManager.cpp

GCodeParser.o:
	$(CC) $(CFLAGS) -c ./gcode/GCodeParser.cpp

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
