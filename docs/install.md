## Installation and building
As soon as it will be possible, binary project builds will be available on GitHub, but currently only build from source is possible.

CalX system supports two build systems. First one is CMake: it is easier to use on Linux and newer Windows platforms, more clear and standart. However older Windows versions(which is used in university physics lab) require other type of building - custom written Makefile for cross-compilation from Linux using MinGW. This build way is complex, 'dirty' and not recommended for use.

After build with both build systems 'build' directory will be generated. It is basically ready for use system.
#### CMake build
It's recommended to build out-of-source. I suggest creating separate build directory. You should also have compiled wxWidgets library(project UI uses dynamic monolithic unicode version of it). Commands are similar for Windows and Linux:
```bash
mkdir build # Execute from project top directory
cd build
cmake ..
```
Now you have solution for MSVC or Makefile for Linux. Just compile it:
```bash
msbuild CalX.sln /p:Configuration=Release /m # On Windows
make -jN # On Linux. Replace N with your processor core count
```
CPack installator can be generated for Windows:
```bash
cpack
```

#### Custom build system
It is used to prepare builds for older Windows versions, because it includes runtime statically. Build system relies on Makefile generating by bash script. It works on both Linux and WSL.

To regenerate Makefile you should execute:
```bash
./misc/old-build/gen-makefile.sh > ./misc/old-build/Makefile # From the top project directory
```
And to get build:
```bash
make -f ./misc/old-build/Makefile all BUILD=[BUILD] -jN WX=[WX] WXLIB=[WXLIB] # BUILD - build directory, N - processor core count, WX - path to wxWidgets library, WXLIB - path wxWidgets DLL relatively to [WX]
```
