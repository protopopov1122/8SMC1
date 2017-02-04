CC=i686-w64-mingw32-g++
LANG=-std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter
DBG=-g3 -ggdb -O3
EXTRA=
CFLAGS=$(LANG) $(DBG) -Ires -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread $(EXTRA)
OUTPUT=prog.exe

all: $(OUTPUT)

$(OUTPUT):
	$(CC) -o $(OUTPUT) *.cpp $(CFLAGS) -Wl,-Bdynamic -Lres -lUSMCDLL
stub:
	$(CC) -shared -o USMCDLL.dll *.cxx $(CFLAGS) -Wno-attributes
clean:
	@rm -f USMCDLL.dll
	@rm -f $(OUTPUT)
