@ECHO OFF
echo Setting VC Vars from Visual Studio 14.0
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"

echo.
echo "================================================================="
echo "=           DLL Injection Project Version 3.2                   ="
echo "================================================================="
echo.

if not exist bin goto skipCleanup
echo Cleaning project...
del bin\*.*
rmdir bin
echo Done cleaning.. starting build.
pause

:skipCleanup
echo Building test..
cl test.c /DEFAULTLIB User32.lib /DEFAULTLIB Kernel32.lib /link /SUBSYSTEM:Console
echo Done..
pause
echo Building dllInjector.dll
cl /D_USRDLL /D_WINDLL dllInjector.c objbase.c /DEFAULTLIB User32.lib /DEFAULTLIB Kernel32.lib /link /SUBSYSTEM:Windows /DLL /OUT:dllInjector.dll
echo Done..
pause
echo Building testDLL..
cl /D_USRDLL /D_WINDLL testDLL.c objbase.c /DEFAULTLIB User32.lib /link /SUBSYSTEM:Windows /DLL /OUT:test.dll
echo Done..
pause
echo Building injectorExe..
cl injectExe.c /DEFAULTLIB User32.lib /DEFAULTLIB Kernel32.lib dllInjector.lib /link /SUBSYSTEM:Console
echo "Done.."
del *.obj
mkdir bin
echo Moving dllInjector binary and lib files to release..
move dllInjector.dll bin\
move dllInjector.lib bin\
move dllInjector.exp bin\
echo Moving test.dll binary and lib files to release..
move test.dll bin\
move test.lib bin\
move test.exp bin\
echo Moving Exe Binaries to release...
move *.exe bin\
echo Done..
pause

echo.
echo =================================================================
echo =                        Done Building                          =
echo =================================================================
echo.

echo Would you like to sign all binaries (Y/N) ?
set INPUT=
set /P INPUT=Type input: %=%
if %INPUT%=="y" or %INPUT%=="Y" goto yes
if %INPUT%=="n" or %INPUT%=="N" goto no
:no
pause
exit
:yes
signtool sign /i "DevCool Coders" /f devcoolC0d3r5.pfx /p decrypt dllInjector.dll
signtool sign /i "DevCool Coders" /f devcoolC0d3r5.pfx /p decrypt test.dll
signtool sign /i "DevCool Coders" /f devcoolC0d3r5.pfx /p decrypt injectExe.exe
signtool sign /i "DevCool Coders" /f devcoolC0d3r5.pfx /p decrypt test.exe
echo.
echo Done signing binaries...
echo.
pause
goto no