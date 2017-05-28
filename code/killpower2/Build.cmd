@echo off
set PATH=C:\Program Files\mingw-w64\x86_64-7.1.0-posix-seh-rt_v5-rev0\mingw64\bin;%PATH%
rem echo %PATH%
rem cd "C:\Program Files\mingw-w64\x86_64-7.1.0-posix-seh-rt_v5-rev0\mingw64\bin"
cd "C:\Users\phili\code\killpower2"

set /P input="Do you want to build (y/n)? "
if "%input%"=="y" goto build
if "%input%"=="Y" goto build
if "%input%"=="n" goto clean
if "%input%"=="N" goto clean
exit

:clean
@echo on
mingw32-make clean
goto end

:build
@echo on
mingw32-make

:end
pause