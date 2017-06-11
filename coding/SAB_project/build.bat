@echo off

goto build

:error
echo.
echo You should enter yes if you want to build.
pause
exit

:makesetup
echo Building setup...
"C:\Program Files\7-Zip\7z.exe" a SAB.7z SAB.exe bdclient.exe install.bat
copy /b 7zCon.sfx + config.txt + SAB.7z Setup.exe
copy Setup.exe .\SAB_Disc\
pause
goto end2

:build
call "C:\Program Files\mingw-w64\x86_64-7.1.0-posix-seh-rt_v5-rev0\mingw.bat"
gcc -o SAB.exe sab.c -lws2_32 -luser32 -lkernel32
gcc -o bdclient.exe bdclient.c -lws2_32

:bsetup
echo.
echo        *** BUILD SETUP ***
echo.
set /P input="Do you want to build setup (Y/N)? "
if "%input%"=="" goto error
if "%input%"=="Y" goto makesetup
if "%input%"=="y" goto makesetup

:end2
