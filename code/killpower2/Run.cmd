@echo off
@cd \Users\phili\code\killpower2
echo.
echo What do you want to do?
echo   1. Build project
echo   2. Run project
echo (Anything else will exit)
set /P input="Input: "
if "%input%"=="1" goto build
if "%input%"=="2" goto run
exit

:run
start cmd.exe /k client.exe 127.0.0.1 TestUser . phatDick101BananaHammick
server.exe
goto end2

:build
echo Building...
start cmd.exe /c Build.cmd
pause
goto end

:end2
pause
exit

:end
