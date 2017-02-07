CC=i686-w64-mingw32-g++
LANG=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter
DBG=-g3 -ggdb -O3
EXTRA=
HEADERS=./res
LIB=./res
CFLAGS=$(LANG) $(DBG) -I$(HEADERS) -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread $(EXTRA)
OUTPUT=prog.exe

all: $(OUTPUT)

$(OUTPUT):
	$(CC) -o $(OUTPUT) -Idevice -Icli device/*.cpp cli/*.cpp $(CFLAGS) -Wl,-Bdynamic,--library-path=$(LIB) -lUSMCDLL
	@cp $(LIB)/USMCDLL.dll .
stub:
	$(CC) -shared -o USMCDLL.dll misc/Stub.cxx $(CFLAGS) -Wno-attributes
gcode:
	$(CC) -o $(OUTPUT) gcode/*.cpp $(CFLAGS)
clean:
	@rm -f USMCDLL.dll
	@rm -f $(OUTPUT)
	@rm -f USMCDLL.dll
