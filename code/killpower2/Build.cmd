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

:login
start cmd.exe /k server.exe
start cmd.exe /k Run.cmd a
goto run

:talker
start cmd.exe /k server.exe -t
start cmd.exe /k Run.cmd b
goto run

:shutdown
start cmd.exe /c server.exe -p "C:\Windows\System32\shutdown.exe /s"
start cmd.exe /k Run.cmd c
exit

:run
cls
set decision=""
echo What would you like to do?
echo    1. Run program as different user.
echo    2. Server/client communication.
echo    3. Shutdown system.
echo (Anything else will go back to menu)
set /P decision=">> "
if "%decision%"=="1" goto login
if "%decision%"=="2" goto talker
if "%decision%"=="3" goto shutdown
goto menu

:build
cls
echo Building...
set PATH=C:\mingw-w64\mingw64\bin;C:\mingw-w64\mingw64\x86_64-w64-mingw64\bin;%PATH%
mingw32-make
pause
goto menu

:clean
cls
echo Cleaning...
set PATH=C:\mingw-w64\mingw64\bin;C:\mingw-w64\mingw64\x86_64-w64-mingw64\bin;%PATH%
mingw32-make clean
pause
goto menu

:end
