REM This script calls all build scripts in correct order

echo Building 8SMC1 stub
call winbuild\build-stub.bat
echo Building CalX Ctrl-lib
call winbuild\build-dll.bat
echo Building 8SMC1 Device API handlers
call winbuild\build-8smc1.bat
echo Building CalX console application
call winbuild\build-exe.bat
echo Building CalX user interface
call winbuild\build-ui.bat %1