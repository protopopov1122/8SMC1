CC=i686-w64-mingw32-g++
LANG=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter
DBG=-g3 -ggdb -O3
EXTRA=
BUILD=./build
HEADERS=./res
LIB=./res
CFLAGS=$(LANG) $(DBG) -I$(HEADERS) -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread $(EXTRA)
OUTPUT=prog.exe

all: $(OUTPUT)

$(OUTPUT):
	$(CC) -o $(BUILD)/$(OUTPUT) -Idevice -Icli -Ictrl-lib device/*.cpp cli/*.cpp ctrl-lib/*.cpp $(CFLAGS) -Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL
	@cp $(LIB)/USMCDLL.dll $(BUILD)
stub:
	$(CC) -shared -o $(BUILD)/USMCDLL.dll misc/Stub.cxx $(CFLAGS) -Wno-attributes
gcode:
	$(CC) -o $(BUILD)/$(OUTPUT) gcode/*.cpp $(CFLAGS)
clean:
	@rm -rf $(BUILD)
	@mkdir -p $(BUILD)
