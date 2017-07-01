#!/bin/bash

# Append header to Makefile
cat misc/old-build/mk-header 

ALL='\t$(MAKE) -f ./misc/old-build/Makefile $(OUTPUT_LIB) BUILD=$(BUILD)\n'

# Generate Device API handlers

function GenDevAPI {
	find device/$1 -name "*.cpp" -and -not -name "$2" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o:\n\t$(CC) $(CFLAGS_LIB) $(CFLAGS) '$3' -c "$0"\n"}'
	OBJS=`find device/$1 -name "*.cpp" -and -not -name "$2" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o"}' | tr '\n' ' '`
	printf "devapi_$1: $OBJS \$(OUTPUT_LIB)\n\t\$(CC) -shared $OBJS $4 \$(LDFLAGS) \$(LDFLAGS_LIB) -Wl,--library-path=\$(BUILD) -lcalx\n"
	ALL=$ALL"\t\$(MAKE) -f ./misc/old-build/Makefile devapi_$1 BUILD=\$(BUILD)\n"
}

GenDevAPI "standart" "Stub.cpp" "-Ires" " -o \$(BUILD)/dev_standart.dll -Wl,-Bdynamic,--library-path=\$(BUILD) -lUSMCDLL  -Wl,--out-implib,\$(BUILD)/dev_standart.a"
GenDevAPI "emulated" "" "" " -o \$(BUILD)/libdev_emulated.so"

function GenExtEng {
	find ext/$1 -name "*.cpp" -and -not -name "$2" -not -iwholename "$5" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o:\n\t$(CC) $(CFLAGS_LIB) $(CFLAGS) '$3' -c "$0"\n"}'
	OBJS=`find ext/$1 -name "*.cpp" -and -not -name "$2" -not -iwholename "$5" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o"}' | tr '\n' ' '`
	printf "exteng_$1: $OBJS \$(OUTPUT_LIB)\n\t\$(CC) -shared $OBJS $4 \$(LDFLAGS) \$(LDFLAGS_LIB) -Wl,--library-path=\$(BUILD) -lcalx\n"
	ALL=$ALL"\t\$(MAKE) -f ./misc/old-build/Makefile exteng_$1 BUILD=\$(BUILD)\n"
}

GenExtEng "sample" "" "" " -o \$(BUILD)/ext_sample.dll" ""

ALL=$ALL'\t$(MAKE) -f ./misc/old-build/Makefile $(OUTPUT) BUILD=$(BUILD)\n\t$(MAKE) -f ./misc/old-build/Makefile $(OUTPUT_LIB) $(OUTPUT_UI) BUILD=$(BUILD)\n'

# Generate library, stub and cli object file build targets
printf "\n"
find device/standart -name "Stub.cpp" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o:\n\t$(CC) $(CFLAGS_LIB) $(CFLAGS) -Ires -c "$0"\n"}'
find ctrl-lib cli -name "*.cpp" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o:\n\t$(CC) $(CFLAGS_LIB) $(CFLAGS) -c "$0"\n"}'
 
# Generate ui object file build targets
printf "\n" 
find ui -name "*.cpp" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o:\n\t$(CC) $(CFLAGS) -I ui `$(WX)/wx-config --cxxflags` -c "$0"\n"}' 

# Generate 8SMC1 Stub link target
printf "stub: Stub.o\n\t\$(CC) -shared -o \$(BUILD)/USMCDLL.dll Stub.o \$(LDFLAGS)\n\n" 

# Generate console program link target
printf "\$(OUTPUT): \$(OUTPUT_LIB) devapi_\$(DEVAPI) " 
OBJS=`find cli -name "*.cpp" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o"}' | tr '\n' ' '`
echo $OBJS 
printf "\t@mkdir -p \$(BUILD)\n"
printf "\t\$(CC) -o \$(BUILD)/\$(OUTPUT) " 
printf "$OBJS" 
printf " \$(LDFLAGS) -Wl,--library-path=\$(BUILD) -ldev_\$(DEVAPI) -l\$(NAME)\n\n" 

#Generate Ctrl-lib link target
printf "\$(OUTPUT_LIB): "
OBJS=`find ctrl-lib -name "*.cpp" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o"}' | tr '\n' ' '`
echo $OBJS 
printf "\t@mkdir -p \$(BUILD)\n"
printf "\t\$(CC) -shared -o \$(BUILD)/\$(OUTPUT_LIB) " 
printf "$OBJS" 
printf " \$(LDFLAGS) \$(LDFLAGS_LIB)\n\n" 


#Generate UI link target
printf "\$(OUTPUT_UI): "
OBJS1=`find ui -name "*.cpp" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o"}' | tr '\n' ' '`
OBJS2=`find cli -not -name "main.cpp" -and -name "*.cpp" | awk -F/ '{split($NF, arr, "\\."); print arr[1]".o"}' | tr '\n' ' '`
OBJS=$OBJS1$OBJS2
echo $OBJS
printf "\t@mkdir -p \$(BUILD)\n"
printf "ifneq (\$(WXLIB),)\n"
printf "\tcp \$(WX)/\$(WXLIB) \$(BUILD)\n"
printf "endif\n"
printf "\t\$(MAKE) -f ./misc/old-build/Makefile icon BUILD=\$(BUILD)\n"
printf "\t\$(CC) -o \$(BUILD)/\$(OUTPUT_UI) " 
printf "$OBJS \$(ICON_RES)"
printf ' $(LDFLAGS) -Wl,--library-path=\$(BUILD) -l$(NAME) `$(WX)/wx-config --libs`\n\n'

printf "all:\n$ALL\t\$(MAKE) -f ./misc/old-build/Makefile copy BUILD=\$(BUILD)\n\n"

# Append footer to Makefile
cat misc/old-build/mk-footer 