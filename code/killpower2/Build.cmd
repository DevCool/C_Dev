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
rem This makes client connect using CreateRemoteProcess()
start cmd.exe /k client.exe 127.0.0.1 Test . nipTuck88!@#
rem This is normal server using CreateRemoteProcess()
server.exe
pause
goto run

:talker
rem This makes client connect using talker()
start cmd.exe /k client.exe -t 127.0.0.1 "Hello world this is a test."
rem This is server using server_talker()
server.exe -t
pause
goto run

:shutdown
rem This makes client connect using LaunchApp()
start cmd.exe /c client.exe 127.0.0.1
rem This is server using LaunchApp()
server.exe -p "C:\Windows\System32\shutdown.exe /s"
exit

:run
cls
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
