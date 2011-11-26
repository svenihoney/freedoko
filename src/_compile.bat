@echo off

SETLOCAL

REM mingw32
PATH %PATH%;C:\MinGW32\bin;C:\MinGW32\lib

REM Sonst Probleme mit Microsoft Visual Studio
SET INCLUDE=

if exist FreeDoko.exe del FreeDoko.exe

REM compile
make

if not exist FreeDoko.exe goto end

call .\FreeDoko.exe %1 %2 %3 %4 %5 %6 %7 %8 %9

:end

ENDLOCAL
