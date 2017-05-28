@echo off
@cd \Users\phili\code\killpower2
:menu
cls
set input=""
echo.
echo What do you want to do?
echo   1. Build project
echo   2. Clean project
echo   3. Run project
echo (Anything else will exit)
set /P input="Input: "
if "%input%"=="1" goto build
if "%input%"=="2" goto clean
if "%input%"=="3" goto run
exit

:run
start cmd.exe /k client.exe 127.0.0.1 guest . perieur338
server.exe
rem "C:\Windows\System32\shutdown.exe /s"
pause
goto menu

:build
cls
echo Building...
set PATH=C:\Program Files\mingw-w64\x86_64-7.1.0-posix-seh-rt_v5-rev0\mingw64\bin;%PATH%
mingw32-make
pause
goto menu

:clean
cls
echo Cleaning...
set PATH=C:\Program Files\mingw-w64\x86_64-7.1.0-posix-seh-rt_v5-rev0\mingw64\bin;%PATH%
mingw32-make clean
pause
goto menu

:end
